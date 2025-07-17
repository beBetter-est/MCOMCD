/*
 * @Author: GCJ 1208651874@q.com
 * @Date: 2024-04-09 22:50:04
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-06-11 22:10:35
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "Decode.h"
#include "../DataWarehouse.h"

#include <vector>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <omp.h>

using namespace std;

int FindRoot(int node, unordered_map<int, int>& roots)
{
    if (node != roots[node])
    {
        roots[node] = FindRoot(roots[node], roots);
    }
    return roots[node];
}
void Decode(int layer, int geneId, vector<pair<int,int>> gene, int type)
{
    int geneLength = gene.size();
    int nodeNum = geneLength;
    if (type != 5)
    {
        nodeNum = nodes[layer].size();
    }

    unordered_map<int, int> roots;
    unordered_map<int, int> rank;
    unordered_map<int, int> visited;
    vector<Community> temporaryCommunities;

    for (int i = 0; i < nodeNum; i++)
    {
        int nodeId = (type == 5) ? gene[i].first : nodes[layer][i].getNodeId();
        roots[nodeId] = nodeId;
        rank[nodeId] = 0;
        visited[nodeId] = 0;
    }

    for (const auto& g : gene)
    {
        int root_1 = FindRoot(g.first, roots);
        int root_2 = FindRoot(g.second, roots);

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
        int temporaryNode_1 = (type == 5) ? gene[i].first : nodes[layer][i].getNodeId();
        if (!visited[temporaryNode_1])
        {
            visited[temporaryNode_1] = 1;
            index++;
            vector<int> temporaryCommunity = {temporaryNode_1};
            int root_1 = FindRoot(temporaryNode_1, roots);
            for (int j = 0; j < nodeNum; j++)
            {
                int temporaryNode_2 = (type == 5) ? gene[j].second : nodes[layer][j].getNodeId();
                if (!visited[temporaryNode_2] && FindRoot(temporaryNode_2, roots) == root_1)
                {
                    visited[temporaryNode_2] = 1;
                    temporaryCommunity.push_back(temporaryNode_2);
                }
            }
            temporaryCommunities.push_back(Community(geneId, index, temporaryCommunity));
        }
    }

    // 看看生成的社区
    // for (int i = 0; i < temporaryCommunities.size(); i++)
    // {
    //     cout << "Community " << i << ": ";
    //     for (int j = 0; j < temporaryCommunities[i].getNodes().size(); j++)
    //     {
    //         cout << temporaryCommunities[i].getNodes()[j] << " ";
    //     }
    //     cout << endl;
    // }
    // exit(0);

    switch(type)
    {
        case 1:
            communities.push_back(temporaryCommunities);
            break;
        case 2:
            progenyCommunities.push_back(temporaryCommunities);
            break;
        case 3:
            layerConsensus[geneId].setCommunities(temporaryCommunities);
            break;
        case 4:
            layerBestCommunities.push_back(temporaryCommunities);
            break;
        case 5:
            consensusCommunities = temporaryCommunities;
            break;
    }
}