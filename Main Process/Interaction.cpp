/*
 * @Author: GCJ 1208651874@q.com
 * @Date: 2024-04-11 16:02:12
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2025-07-17 18:26:12
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "Interaction.h"
#include "Decode.h"
#include "../DataWarehouse.h"
#include "../Constant.h"

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <map>

using namespace std;

vector<Community> temporaryCommunities;

int FindRoot_Consensus(int node, unordered_map<int, int>& roots)
{
    if (node != roots[node])
    {
        roots[node] = FindRoot_Consensus(roots[node], roots);
    }
    return roots[node];
}

void Decode_Consensus(int layer, int geneId, vector<pair<int,int>> gene)
{
    int geneLength = gene.size();
    int nodeNum = geneLength;

    unordered_map<int, int> roots;
    unordered_map<int, int> rank;
    unordered_map<int, int> visited;
    // vector<Community> temporaryCommunities;

    for (int i = 0; i < nodeNum; i++)
    {
        int nodeId = gene[i].first;
        roots[nodeId] = nodeId;
        rank[nodeId] = 0;
        visited[nodeId] = 0;
    }

    for (const auto& g : gene)
    {
        int root_1 = FindRoot_Consensus(g.first, roots);
        int root_2 = FindRoot_Consensus(g.second, roots);

        if (root_1 != root_2)
        {
            if (rank[root_1] > rank[root_2])
            {
                roots[root_2] = root_1;
            }
            else if (rank[root_1] < rank[root_2])
            {
                roots[root_1] = root_2;
            }
            else
            {
                roots[root_1] = root_2;
                rank[root_2]++;
            }
        }
    }

    int index = 0;

    for (int i = 0; i < nodeNum; i++)
    {
        int temporaryNode_1 = gene[i].first;
        if (!visited[temporaryNode_1])
        {
            visited[temporaryNode_1] = 1;
            index++;
            vector<int> temporaryCommunity = {temporaryNode_1};
            int root_1 = FindRoot_Consensus(temporaryNode_1, roots);
            for (int j = 0; j < nodeNum; j++)
            {
                int temporaryNode_2 = gene[j].second;
                if (!visited[temporaryNode_2] && FindRoot_Consensus(temporaryNode_2, roots) == root_1)
                {
                    visited[temporaryNode_2] = 1;
                    temporaryCommunity.push_back(temporaryNode_2);
                }
            }
            temporaryCommunities.push_back(Community(geneId, index, temporaryCommunity));
        }
    }
}

void Interaction(int layerId, int generationId)
{
    srand(time(NULL));

    int consensusId;
    vector<pair<int,int>> gene;
    vector<Community> communities;
    double fitness;
    double Q;
    double NMI;
    double Jaccard = 0.0;
    double weight_a;
    double weight_b;

    layerConsensus.push_back(Consensus(consensusId, gene, communities, fitness, Q, NMI, Jaccard, weight_a, weight_b));

    consensusId = generationId;
    layerConsensus[consensusId].setConsensusId(consensusId);

    vector<pair<int,int>> tempGene;
    int maxNodeId = 0;
    if (layerId > 0)
    {
        int tempCounter = 0;
        for (int i = 0; i < layerBestPopulation[generationId - 1].getGene().size(); i ++ )
        {
            while (layerBestPopulation[layerId - 1].getGene()[tempCounter].first < layerBestPopulation[layerId].getGene()[i].first)
            {
                tempCounter ++ ;
            }
            if (layerBestPopulation[layerId - 1].getGene()[tempCounter].first == layerBestPopulation[layerId].getGene()[i].first)
            {
                if (adjacencyMatrices[layerId].getMatrix()[layerBestPopulation[layerId - 1].getGene()[tempCounter].first][layerBestPopulation[layerId - 1].getGene()[tempCounter].second] == 1)
                {
                    tempGene.push_back(make_pair(layerBestPopulation[layerId - 1].getGene()[tempCounter].first, layerBestPopulation[layerId - 1].getGene()[tempCounter].second));
                }
                else
                {
                    tempGene.push_back(make_pair(layerBestPopulation[layerId].getGene()[i].first, layerBestPopulation[layerId].getGene()[i].second));
                }
            }
            else
            {
                tempGene.push_back(make_pair(layerBestPopulation[layerId].getGene()[i].first, layerBestPopulation[layerId].getGene()[i].second));
            }

            maxNodeId = max(maxNodeId, layerBestPopulation[layerId].getGene()[i].first);
            maxNodeId = max(maxNodeId, layerBestPopulation[layerId].getGene()[i].second);
        }
    }

    if (layerId > 0) {
        // gene = layerBestPopulation[layerId].getGene();
        gene = tempGene;
        layerConsensus[consensusId].setGene(gene);

        Decode_Consensus(layerId, generationId, gene);

        // 计算Q
        vector<vector<int>> tempMatrix(maxNodeId + 5, vector<int>(maxNodeId + 5, 0));
        map<int, int> Node_Degree_Map;
        for (int i = 0; i < tempGene.size(); i ++ )
        {
            int node_1 = tempGene[i].first;
            int node_2 = tempGene[i].second;
            tempMatrix[node_1][node_2] = 1;
            tempMatrix[node_2][node_1] = 1;

            Node_Degree_Map[node_1] ++ ;
            Node_Degree_Map[node_2] ++ ;
        }

        int edgesNum = edges[layerId].size();
        vector<int> edgesNumCom(temporaryCommunities.size(), 0);
        vector<int> nodeDegrees(temporaryCommunities.size(), 0);

        #pragma omp parallel for
        for (int i = 0; i < temporaryCommunities.size(); i ++ )
        {
            const auto& nodes_i = temporaryCommunities[i].getNodes();
            int tempCommSize = nodes_i.size();
            int tempEdgesNum = 0;

            for (int j = 0; j < tempCommSize; j ++ )
            {
                int node_j = nodes_i[j];
                nodeDegrees[i] += Node_Degree_Map[node_j];
                for (int k = j + 1; k < tempCommSize; k ++ )
                {
                    int node_k = nodes_i[k];
                    if (tempMatrix[node_j][node_k] > 0)
                    {
                        tempEdgesNum ++ ;
                    }
                }
            }

            edgesNumCom[i] = tempEdgesNum;
        }

        double tempQ = 0.0;
        
        #pragma omp parallel for reduction(+:fitness)
        for (int i = 0; i < temporaryCommunities.size(); i ++ )
        {
            tempQ += ((1.0 * edgesNumCom[i] / edgesNum) - ((nodeDegrees[i] * 1.0) / (2.0 * edgesNum)) * ((nodeDegrees[i] * 1.0) / (2.0 * edgesNum)));
        }
        
        Q = layerBestPopulation[layerId].getQ();
        layerConsensus[consensusId].setQ(tempQ);
    }
    else {
        gene = layerBestPopulation[layerId].getGene();
        layerConsensus[consensusId].setGene(gene);

        Q = layerBestPopulation[layerId].getQ();
        layerConsensus[consensusId].setQ(Q);
    }
    

    NMI = layerBestPopulation[layerId].getNMI();
    layerConsensus[consensusId].setNMI(NMI);

    Decode(layerId, generationId, gene, 3);

    if (layerId > 0)
    {
        // Calculate Jaccard and fitness.
        vector<Community> communities_1 = layerBestCommunities[layerId - 1];
        // vector<Community> communities_2 = layerConsensus[consensusId].getCommunities();
        vector<Community> communities_2 = temporaryCommunities;
        int communitiesNum_1 = communities_1.size();
        int communitiesNum_2 = communities_2.size();

        for (int i = 0; i < communitiesNum_1; i ++ )
        {
            double temporary = 0.0;
            for (int j = 0; j < communitiesNum_2; j ++ )
            {
                vector<int> intersectionSet;
                set_intersection(communities_1[i].getNodes().begin(), communities_1[i].getNodes().end(), communities_2[j].getNodes().begin(), communities_2[j].getNodes().end(), back_inserter(intersectionSet));
                vector<int> unionSet;
                set_union(communities_1[i].getNodes().begin(), communities_1[i].getNodes().end(), communities_2[j].getNodes().begin(), communities_2[j].getNodes().end(), back_inserter(unionSet));
                temporary += (double)intersectionSet.size() / unionSet.size();
            }
            Jaccard += temporary / communitiesNum_2;
        }
        Jaccard = Jaccard / communitiesNum_1;
        fitness = Q * weight_1 + Jaccard * weight_2;

        weight_a = weight_1;
        weight_b = weight_2;

        layerConsensus[consensusId].setWeight_a(weight_a);
        layerConsensus[consensusId].setWeight_b(weight_b);
        layerConsensus[consensusId].setJaccard(Jaccard);
        layerConsensus[consensusId].setFitness(fitness);
        
        // Change the weight value.
        if (fitness < layerConsensus[consensusId - 1].getFitness() || ((consensusId >= Max_Invariant - 1) && (fitness == layerConsensus[consensusId - Max_Invariant + 1].getFitness())))
        {
            if (weight_1 == 0.5 && weight_2 == 0.5)
            {
                double randWeight = rand() % (RandomSeed + 1) / (double)(RandomSeed + 1);
                while (weight_2 + randWeight >= 1 || weight_1 - randWeight <= 0)
                {
                    randWeight = rand() % (RandomSeed + 1) / (double)(RandomSeed + 1);
                }
                weight_1 = weight_1 - randWeight;
                weight_2 = weight_2 + randWeight;
            }
            else
            {
                double randNum = rand() % (RandomSeed + 1) / (double)(RandomSeed + 1);
                if (randNum < 0.3)
                {
                    double randWeight = rand() % (RandomSeed + 1) / (double)(RandomSeed + 1);
                    while (weight_1 + randWeight >= 1 || weight_2 - randWeight <= 0)
                    {
                        randWeight = rand() % (RandomSeed + 1) / (double)(RandomSeed + 1);
                    }
                    weight_1 = weight_1 + randWeight;
                    weight_2 = weight_2 - randWeight;
                }
                else
                {
                    double randWeight = rand() % (RandomSeed + 1) / (double)(RandomSeed + 1);
                    while (weight_1 - randWeight <= 0 || weight_2 + randWeight >= 1)
                    {
                        randWeight = rand() % (RandomSeed + 1) / (double)(RandomSeed + 1);
                    }
                    weight_1 = weight_1 - randWeight;
                    weight_2 = weight_2 + randWeight;
                }
            }
        }       
    }
    else
    {
        weight_a = weight_1;
        weight_b = weight_2;
        Jaccard = 0;
        fitness = Q;
        layerConsensus[consensusId].setWeight_a(weight_a);
        layerConsensus[consensusId].setWeight_b(weight_b);
        layerConsensus[consensusId].setJaccard(Jaccard);
        layerConsensus[consensusId].setFitness(fitness);
    }

    temporaryCommunities.clear();
}