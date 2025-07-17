/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-19 11:44:49
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-05-29 09:29:01
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "ConsensusGene.h"
#include "DataWarehouse.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <tr1/unordered_map>
#include <random> 
#include <omp.h>

using namespace std;
using namespace std::tr1;

int static findElement(vector<int> node)
{    
    unordered_map<int, int> nodeCount;
    for (int i = 0; i < node.size(); i ++ )
    {
        nodeCount[node[i]] ++;
    }

    int maxCount = 0;
    vector<int> maxElements;
    for (auto it = nodeCount.begin(); it != nodeCount.end(); it ++ )
    {
        if (it->second > maxCount)
        {
            maxCount = it->second;
            maxElements.clear();
            maxElements.push_back(it->first);
        }
        else if (it->second == maxCount)
        {
            maxElements.push_back(it->first);
        }
    }

    int maxElement = 0;
    if (maxElements.size() == 1)
    {
        maxElement = maxElements[0];
    }
    else
    {
        // Strategy 1: Immediately return
        random_device rd;
        mt19937 gen(rd());
        shuffle(maxElements.begin(), maxElements.end(), gen);
        maxElement = maxElements[0];

        // Strategy 2: Return NMI based on the optimal individual in each layer
    }

    maxElements.clear();
    nodeCount.clear();
    return maxElement;
}

vector<pair<int,int>> ConsensusGene()
{
    vector<int> nodesAll;
    unordered_map<int, int> nodeToIndex;
    for (int i = 0; i < nodes.size(); i ++ )
    {
        for (int j = 0; j < nodes[i].size(); j ++ )
        {
            int nodeId = nodes[i][j].getNodeId();
            auto it = nodeToIndex.find(nodeId);
            if (it == nodeToIndex.end())
            {
                nodesAll.push_back(nodeId);
                nodeToIndex[nodeId] = nodesAll.size() - 1;
            }
        }
    }
    sort(nodesAll.begin(), nodesAll.end());

    vector<pair<int, int>> consensusGene;
    #pragma omp parallel for
    for (int i = 0; i < nodesAll.size(); i++) {
        vector<int> appear;
        for (int j = 0; j < layerBestPopulation.size(); j++) {
            for (int k = 0; k < layerBestPopulation[j].getGene().size(); k++) {
                if (layerBestPopulation[j].getGene()[k].first == nodesAll[i])
                {
                    appear.push_back(layerBestPopulation[j].getGene()[k].second);
                }
                if (layerBestPopulation[j].getGene()[k].second == nodesAll[i])
                {
                    appear.push_back(layerBestPopulation[j].getGene()[k].first);
                }
            }
        }
        int consensusValue = findElement(appear);
        #pragma omp critical
        {
            consensusGene.emplace_back(nodesAll[i], consensusValue);
        }
        appear.clear();
    }
    
    nodesAll.clear();
    nodeToIndex.clear();
    return consensusGene;
}