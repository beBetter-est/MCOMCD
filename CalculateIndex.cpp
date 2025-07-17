/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-15 21:09:51
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2025-07-17 18:23:10
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */

#include "CalculateIndex.h"
#include "DataWarehouse.h"
#include "Constant.h"
#include "ConsensusGene.h"

#include "Main Process/Decode.h"

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Use a hash table to cache the edge count and number of occurrences of a node pair.
unordered_map<pair<int, int>, int, boost::hash<pair<int, int>>> edgeCountCache;
unordered_map<pair<int, int>, int, boost::hash<pair<int, int>>> occurrenceCache;

int static findDegree(int nodeId, int layerId)
{
    int degree = 0;
    set<int> temp;

    for (int i = 0; i < layerBestPopulation[nodeId].getGene().size(); i++)
    {
        auto& gene = layerBestPopulation[nodeId].getGene()[i];
        if (gene.first == nodeId)
        {
            temp.insert(gene.second);
        }
        if (gene.second == nodeId)
        {
            temp.insert(gene.first);
        }
    }

    degree = temp.size();
    temp.clear();

    return degree;
}

int static countNumberEdges(int node1, int node2)
{
    auto key = make_pair(node1, node2);
    auto it = edgeCountCache.find(key);
    if (it != edgeCountCache.end()) {
        return it->second;
    }

    bool hasEdge = false;
    for (int i = 0; i < layerBestPopulation.size(); i++) {
        for (int j = 0; j < layerBestPopulation[i].getGene().size(); j++) {
            if ((layerBestPopulation[i].getGene()[j].first == node1 && layerBestPopulation[i].getGene()[j].second == node2) ||
                (layerBestPopulation[i].getGene()[j].first == node2 && layerBestPopulation[i].getGene()[j].second == node1)) {
                hasEdge = true;
                break;
            }
        }
        if (hasEdge) {
            break;
        }
    }

    edgeCountCache[key] = hasEdge ? 1 : 0;
    return edgeCountCache[key];
}

int static calculatedOccurrence(int node1, int node2)
{
    auto key = make_pair(node1, node2);
    auto it = occurrenceCache.find(key);
    if (it != occurrenceCache.end()) {
        return it->second;
    }

    int appearance = 0;
    for (int i = 0; i < layerBestPopulation.size(); i++) {
        for (int j = 0; j < layerBestPopulation[i].getGene().size(); j++) {
            if ((layerBestPopulation[i].getGene()[j].first == node1 && layerBestPopulation[i].getGene()[j].second == node2) ||
                (layerBestPopulation[i].getGene()[j].first == node2 && layerBestPopulation[i].getGene()[j].second == node1)) {
                appearance++;
                break;
            }
        }
    }

    occurrenceCache[key] = appearance;
    return occurrenceCache[key];
}

void CalculateIndex()
{
    // Calculate the modularity of the multiplex network.
    auto startTime_Q = steady_clock::now();

    // Pre-calculate the degrees of each node in each layer
    vector<vector<int>> degrees(nodes.size());
    for (int layer = 0; layer < nodes.size(); layer ++ ) 
    {
        degrees[layer].resize(nodes[layer].size());
        for (int i = 0; i < nodes[layer].size(); ++i) 
        {
            degrees[layer][i] = findDegree(nodes[layer][i].getNodeId(), layer);
        }
    }

    // Calculate totalSides and layerEdges
    double totalSides = 0.0;
    vector<int> layersEdges;

    for (int i = 0; i < edges.size(); i ++ )
    {
        totalSides += edges[i].size();
        layersEdges.push_back(edges[i].size());
    }

    // Use pre-calculated degrees to calculate Q_multiplex
    cout << "Calculate Q_multiplex..." << endl;
    double Q_multiplex = 0.0;
    #pragma omp parallel for reduction(+:Q_multiplex)
    for (int i = 0; i < nodes.size(); i ++ )
    {  
        double local_Q_multiplex = 0.0;
        double lamda = 1.0;
        for (int j = 0; j < nodes[i].size(); j ++ )
        {
            for (int k = 0; k < nodes[i].size(); k ++ ) //2E10
            {
                if (j != k)
                {
                    int node1_id = nodes[i][j].getNodeId();
                    int node2_id = nodes[i][k].getNodeId();
                    local_Q_multiplex += adjacencyMatrices[i].getMatrix()[node1_id][node2_id];
                    local_Q_multiplex -= lamda * (degrees[i][j] * degrees[i][k]) / (2.0 * layersEdges[i]);
                }
            }
        }
        #pragma omp atomic
        Q_multiplex += local_Q_multiplex;
    }

    Q_multiplex /= (2.0 * totalSides);
    
    cout << "Q_multiplex: " << Q_multiplex << endl;

    duration<double> diff_Q = steady_clock::now() - startTime_Q;
    cout << "The program runs for Q_multiplex is " << diff_Q.count() << " seconds." << endl;

    // Calculate consensus gene representation.
    vector<pair<int,int>> consensusGene = ConsensusGene();

    // Decode to get community partition.
    Decode(0, 0, consensusGene, 5);

    // Calculate the redundancy of the multiplex network.   
    cout << "Calculate the redundancy of the multiplex network..." << endl;    
    auto startTime_redundancy = steady_clock::now();

    double redundancy = 0.0;
    double totalLayers = layerBestPopulation.size();
    double effectiveCommunity = 0.0;
    
    for (int i = 0; i < consensusCommunities.size(); i++) 
    {
        if (consensusCommunities[i].getNodes().size() == 1)
            continue;

        effectiveCommunity++;

        double denominator = 0.0;
        vector<pair<int, int>> VC;
        
        for (int j = 0; j < consensusCommunities[i].getNodes().size(); j++) 
        {
            for (int k = j + 1; k < consensusCommunities[i].getNodes().size(); k++) 
            {
                if (countNumberEdges(consensusCommunities[i].getNodes()[j], consensusCommunities[i].getNodes()[k]) == 1) 
                {
                    denominator++;
                    VC.push_back(make_pair(consensusCommunities[i].getNodes()[j], consensusCommunities[i].getNodes()[k]));
                }
            }
        }
        denominator *= totalLayers;
        for (int j = 0; j < VC.size(); j++) 
        {
            double molecule = calculatedOccurrence(VC[j].first, VC[j].second);
            if (molecule < 2) continue;
            redundancy += molecule / denominator;   
        }
        VC.clear();
    }
    redundancy /= effectiveCommunity;

    cout << "Redundancy: " << redundancy << endl;

    duration<double> diff_redundancy = steady_clock::now() - startTime_redundancy;
    cout << "The program runs for Redundancy is " << diff_redundancy.count() << " seconds." << endl;

    // Store consensus genes and community divisions.
    string layerPath = layerResultsPath + "/Consensus_Gene.txt";
    ofstream outputConsensusGene(layerPath);
    if (!outputConsensusGene.is_open())
    {
        cerr << "outputFile open failed!" << endl;
        exit(1);
    }
    for (int i = 0; i < consensusGene.size(); i ++ )
    {
        outputConsensusGene << consensusGene[i].first << " " << consensusGene[i].second << endl;
    }
    if (outputConsensusGene.is_open())
    {
        outputConsensusGene.close();
    }

    layerPath = layerResultsPath + "/Consensus_Communities.txt";
    ofstream outputConsensusCommunities(layerPath);
    if (!outputConsensusCommunities.is_open())
    {
        cerr << "outputFile1 open failed!" << endl;
        exit(1);
    }
    for (int i = 0; i < consensusCommunities.size(); i ++ )
    {
        outputConsensusCommunities << "Community_" << consensusCommunities[i].getCommunityId() << ": ";
        for (int j = 0; j < consensusCommunities[i].getNodes().size(); j ++ )
        {
            outputConsensusCommunities << consensusCommunities[i].getNodes()[j] << " ";
        }
        outputConsensusCommunities << endl;
    }
    if (outputConsensusCommunities.is_open())
    {
        outputConsensusCommunities.close();
    }

    layerPath = layerResultsPath + "/Community_Index_Result.txt";
    ofstream outputCommunityIndex(layerPath);
    if (!outputCommunityIndex.is_open())
    {
        cerr << "outputFile2 open failed!" << endl;
        exit(1);
    }
    outputCommunityIndex << "Q_multiplex: " << Q_multiplex << endl;
    outputCommunityIndex << "Redundancy: " << redundancy << endl;
    if (outputCommunityIndex.is_open())
    {
        outputCommunityIndex.close();
    }

    layersEdges.clear();
    consensusGene.clear();
    degrees.clear();
}
