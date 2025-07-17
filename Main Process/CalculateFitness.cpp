/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-09 19:57:24
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2025-07-17 18:25:14
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "CalculateFitness.h"
#include "../DataWarehouse.h"
#include "../Constant.h"

#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <set>
#include <cmath>
#include <cstring>
#include <map>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

vector<int> union_set_CF(const vector<int> &v1, const vector<int> &v2)
{
    vector<int> result;
    set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(result));
    return result;
}

double static calculateNMI(vector<Community>& community1, vector<Community>& community2)
{
    size_t communitiesNum1 = community1.size();
    size_t communitiesNum2 = community2.size();

    vector<vector<int>> confusionMatrix(communitiesNum1, vector<int>(communitiesNum2, 0));
    vector<int> rowSummation(communitiesNum1, 0);
    vector<int> columnSummation(communitiesNum2, 0);
    vector<int> unionAll;
    
    auto startTime_3 = steady_clock::now();
    vector<vector<int>> unionAllLocal(omp_get_max_threads());
    #pragma omp parallel for
    for (size_t i = 0; i < communitiesNum1; ++i) {
        vector<int> temp_1 = community1[i].getNodes();
        vector<int> rowSumLocal(communitiesNum1, 0);
        vector<int> colSumLocal(communitiesNum2, 0);
        
        for (size_t j = 0; j < communitiesNum1; ++j) {

            vector<int> temp_2 = community2[j].getNodes();
            vector<int> tempIntersection;
            set_intersection(temp_1.begin(), temp_1.end(), temp_2.begin(), temp_2.end(), back_inserter(tempIntersection));
            int tempIntersectionSize = tempIntersection.size();

            vector<int> tempUnion;
            set_union(temp_1.begin(), temp_1.end(), temp_2.begin(), temp_2.end(), back_inserter(tempUnion));

            #pragma omp critical
            {
                confusionMatrix[i][j] = tempIntersectionSize;
            }

            unionAllLocal[omp_get_thread_num()] = union_set_CF(unionAllLocal[omp_get_thread_num()], tempUnion);

            rowSumLocal[i] += tempIntersectionSize;
            colSumLocal[j] += tempIntersectionSize;

            temp_2.clear();
            tempUnion.clear();
            tempIntersection.clear();
        }

        #pragma omp critical
        {
            rowSummation[i] += rowSumLocal[i];
            for (size_t j = 0; j < communitiesNum2; ++j) {
                columnSummation[j] += colSumLocal[j];
            }
        }
        
        temp_1.clear();
        rowSumLocal.clear();
        colSumLocal.clear();
    }
    for (const auto& localUnion : unionAllLocal) {
        unionAll = union_set_CF(unionAll, localUnion);
    }
    unionAllLocal.clear();
    duration<double> diff_3 = steady_clock::now() - startTime_3;
    cout << "The time to NMI preprocessing 1 is : " << diff_3.count() << " seconds." << endl;

    double numerator = 0.0;
    double denominator = 0.0;
    auto startTime_4 = steady_clock::now();
    for (size_t i = 0; i < communitiesNum2; ++i) {
        if (rowSummation[i] > 0) {
            denominator += rowSummation[i] * log((double)rowSummation[i] / unionAll.size());
        }
        for (size_t j = 0; j < communitiesNum2; ++j) {
            if (confusionMatrix[i][j] > 0 && rowSummation[i] > 0 && columnSummation[j] > 0) {
                numerator += confusionMatrix[i][j] * log((double)confusionMatrix[i][j] * unionAll.size() / (rowSummation[i] * columnSummation[j]));
            }
        }
    }
    duration<double> diff_4 = steady_clock::now() - startTime_4;
    cout << "The time to NMI preprocessing 2 is : " << diff_4.count() << " seconds." << endl;
    auto startTime_5 = steady_clock::now();
    for (size_t j = 0; j < communitiesNum2; ++j) {
        if (columnSummation[j] > 0) {
            denominator += columnSummation[j] * log((double)columnSummation[j] / unionAll.size());
        }
    }
    duration<double> diff_5 = steady_clock::now() - startTime_5;
    cout << "The time to NMI preprocessing 3 is : " << diff_5.count() << " seconds." << endl;

    confusionMatrix.clear();
    rowSummation.clear();
    columnSummation.clear();
    unionAll.clear();

    if (denominator == 0) return 0;

    return ((-2.0 * numerator) / denominator);
}

vector<double> CalculateFitness(int layer, int geneId, vector<pair<int,int>> gene, int type)
{
    vector<double> allFitness;
    double fitness = 0.0;

    // The first objective function module is degree Q.
    // C. Pizzuti, “Evolution computation for community detection in networks: A review,” IEEE Trans. Evol. Comput., vol. 22, no. 3, pp. 464–483, Jun. 2018.
    auto startTime_1 = steady_clock::now();

    int edgesNum = edges[layer].size();
    int nodesNum = nodes[layer].size();

    map<int, int> Node_Degree_Map;
    for (int i = 0; i < nodes[layer].size(); i ++ )
    {
        Node_Degree_Map[nodes[layer][i].getNodeId()] = nodes[layer][i].getNeighbors().size();
    }

    // 计算modularityDensity，对每个单层网络进行计算
    double modularityDensity = 0.0;

    //  Declare variables for the new TPR metric 
    double totalTPR = 0.0;
    int validCommunitiesForTprCount = 0;
    
    if (type == 1)
    {
        int numCommunities = communities[geneId].size();
        
        vector<int> edgesNumCom(communities[geneId].size());
        vector<int> nodeDegrees(communities[geneId].size(), 0);
        vector<vector<int>> adjMatrixValues = adjacencyMatrices[layer].getMatrix();

        #pragma omp parallel for
        for (int i = 0; i < communities[layer].size(); i++) {
            const auto& nodes_i = communities[geneId][i].getNodes();
            int tempCommSize = nodes_i.size();
            int tempEdgesNum = 0;

            for (int j = 0; j < tempCommSize; j++) {
                int nodeId_j = nodes_i[j];
                nodeDegrees[i] += Node_Degree_Map[nodeId_j];
                for (int k = j + 1; k < tempCommSize; k++) {
                    int nodeId_k = nodes_i[k];
                    if (adjMatrixValues[nodeId_j][nodeId_k] > 0) {
                        tempEdgesNum += 1;
                    }
                }
            }
            edgesNumCom[i] = tempEdgesNum;
        }

        // 【新增】在这里计算 
        for (int i = 0; i < numCommunities; i++) {
            const auto& current_nodes = communities[geneId][i].getNodes();
            int nodesInComm = current_nodes.size();

            // A triangle needs at least 3 nodes
            if (nodesInComm < 3) continue;

            set<int> participatingNodes;
            // Iterate through all unique triplets of nodes to find triangles
            for (int u_idx = 0; u_idx < nodesInComm - 1; ++u_idx) {
                for (int v_idx = u_idx + 1; v_idx < nodesInComm; ++v_idx) {
                    for (int w_idx = v_idx + 1; w_idx < nodesInComm; ++w_idx) {
                        int u = current_nodes[u_idx];
                        int v = current_nodes[v_idx];
                        int w = current_nodes[w_idx];

                        // Check if the three nodes form a triangle
                        if (adjMatrixValues[u][v] > 0 && adjMatrixValues[v][w] > 0 && adjMatrixValues[u][w] > 0) {
                            participatingNodes.insert(u);
                            participatingNodes.insert(v);
                            participatingNodes.insert(w);
                        }
                    }
                }
            }
            // Add this community's TPR to the total
            totalTPR += (double)participatingNodes.size() / nodesInComm;
            validCommunitiesForTprCount++;
        }

        edgesNumCom.clear();
        nodeDegrees.clear();
        adjMatrixValues.clear();     
    }
    else
    {
        int numCommunities = progenyCommunities[geneId].size();

        vector<int> edgesNumCom(progenyCommunities[geneId].size());
        vector<int> nodeDegrees(progenyCommunities[geneId].size() + 5, 0);
        vector<vector<int>> adjMatrixValues = adjacencyMatrices[layer].getMatrix();

        #pragma omp parallel for
        for (int i = 0; i < progenyCommunities[layer].size() - 1; i++) {
            const auto& nodes_i = progenyCommunities[geneId][i].getNodes();
            int tempCommSize = nodes_i.size();
            int tempEdgesNum = 0;

            for (int j = 0; j < tempCommSize; j++) {
                int nodeId_j = nodes_i[j];
                nodeDegrees[i] += Node_Degree_Map[nodeId_j];
                for (int k = j + 1; k < tempCommSize; k++) {
                    int nodeId_k = nodes_i[k];
                    if (adjMatrixValues[nodeId_j][nodeId_k] > 0) {
                        tempEdgesNum += 1;
                    }
                }
            }
            edgesNumCom[i] = tempEdgesNum;
        }

        // 【新增】在这里计算 
        for (int i = 0; i < numCommunities; i++) {
            const auto& current_nodes = progenyCommunities[geneId][i].getNodes();
            int nodesInComm = current_nodes.size();

            if (nodesInComm < 3) continue;

            set<int> participatingNodes;
            for (int u_idx = 0; u_idx < nodesInComm; ++u_idx) {
                for (int v_idx = u_idx + 1; v_idx < nodesInComm; ++v_idx) {
                    for (int w_idx = v_idx + 1; w_idx < nodesInComm; ++w_idx) {
                        int u = current_nodes[u_idx];
                        int v = current_nodes[v_idx];
                        int w = current_nodes[w_idx];

                        if (adjMatrixValues[u][v] > 0 && adjMatrixValues[v][w] > 0 && adjMatrixValues[u][w] > 0) {
                            participatingNodes.insert(u);
                            participatingNodes.insert(v);
                            participatingNodes.insert(w);
                        }
                    }
                }
            }
            totalTPR += (double)participatingNodes.size() / nodesInComm;
            validCommunitiesForTprCount++;
        }


        edgesNumCom.clear();
        nodeDegrees.clear();
        adjMatrixValues.clear();
    }

    Node_Degree_Map.clear();
    
    // fitness /= (2.0 * edgesNum);
    allFitness.push_back(fitness);

    duration<double> diff_1 = steady_clock::now() - startTime_1;
    cout << "The time to calculate Q is : " << diff_1.count() << " seconds." << endl;

    if (layer != 0) 
    {
        // The remaining layers also have an objective function, NMI.
        auto startTime_2 = steady_clock::now();
        vector<double> allNMI;
        for (int i = 0; i < layer; i ++ )
        {
            vector<Community> communities_1 = layerBestCommunities[i];
            vector<Community> communities_2;
            if (type == 1) {
                communities_2 = communities[geneId];
            }
            if (type == 2) {
                communities_2 = progenyCommunities[geneId];
            }
            double NMI = calculateNMI(communities_1, communities_2);
            allNMI.push_back(NMI);
        }
        duration<double> diff_2 = steady_clock::now() - startTime_2;
        cout << "The time to calculate NMI is : " << diff_2.count() << " seconds." << endl;
        
        vector<double> NMI_Weight;
        double totalWeight = 0.0;
        for (int i = 0; i < layer; i ++ )
        {
            double similarity = 0.0;
            for (int j = 0; j < edges[layer].size(); j ++)
            {
                for (int k = 0; k < edges[i].size(); k ++)
                {
                    if (edges[layer][j].getFirstNode() == edges[i][k].getFirstNode() && edges[layer][j].getSecondNode() == edges[i][k].getSecondNode())
                    {
                        similarity += 1.0;
                    }
                }
            }
            similarity /= edges[layer].size();
            NMI_Weight.push_back(exp(-1.0 * Decay_Rate * similarity));
            totalWeight += exp(-1.0 * Decay_Rate * similarity);
        }

        double NMI_Final = 0.0;
        for (int i = 0; i < layer; i ++ )
        {
            NMI_Final += allNMI[i] * NMI_Weight[i] / totalWeight;
        }

        allFitness.push_back(NMI_Final);

        fitness = weight_1 * fitness + weight_2 * NMI_Final;

        allNMI.clear();
        NMI_Weight.clear();
    }

    allFitness.push_back(fitness);

    // 输出modularityDensity
    double avgTPR = (validCommunitiesForTprCount > 0) ? (totalTPR / validCommunitiesForTprCount) : 0.0;
    allFitness.push_back(avgTPR);  // 将modularityDensity加入结果
    return allFitness;
}