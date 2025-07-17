/*
 * @Author: GCJ 1208651874@q.com
 * @Date: 2024-04-10 20:26:11
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-04-13 23:11:08
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "Mutation.h"
#include "../DataWarehouse.h"
#include "../Constant.h"

#include <iostream>
#include <vector>

using namespace std;

vector<int> FindNeighbors(int layerId, int nodeId)
{
    vector<int> result;
    int nodeSize = nodes[layerId].size();
    for (int i = 0; i < nodeSize; i ++ )
    {
        if (nodes[layerId][i].getNodeId() == nodeId)
        {
            result = nodes[layerId][i].getNeighbors();
            break;
        }
    }
    return result;
}

void Mutation(int layerId)
{
    // Random variation based on neighbors.
    srand(time(NULL));
    for (int i = 0; i < Population_Size; i ++ )
    {
        int geneLength = progenyPopulation[i].getGene().size();
        vector<pair<int,int>> temporaryGene;
        for (int j = 0; j < geneLength; j ++ )
        {
            double random = rand() % (RandomSeed + 1) / (double)(RandomSeed + 1);
            if (random > Mutation_Probability)
            {
                temporaryGene.push_back(progenyPopulation[i].getGene()[j]);
                continue;
            }
            int temporaryNode = progenyPopulation[i].getGene()[j].first;
            vector<int> temporaryNeighbors = FindNeighbors(layerId, temporaryNode);
            temporaryGene.push_back(make_pair(temporaryNode, temporaryNeighbors[rand() % temporaryNeighbors.size()]));
            temporaryNeighbors.clear();
        }
        progenyPopulation[i].setGene(temporaryGene);
        temporaryGene.clear();
    }
}