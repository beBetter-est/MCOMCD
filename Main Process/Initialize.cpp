/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-08 20:27:14
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-05-29 09:38:58
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "Initialize.h"
#include "../Constant.h"
#include "../DataWarehouse.h"
#include "CalculateFitness.h"
#include "Decode.h"

#include <iostream>
#include <vector>
#include <utility>

using namespace std;

void Initialize(int layer)
{
    srand(time(NULL));
    for (int i = 0; i < Population_Size; i ++ )
    {
        cout << "Chromosome: " << i << endl;
        vector<pair<int,int>> gene;
        for (int j = 0; j < nodes[layer].size(); j ++ )
        {
            vector<int> neighbors = nodes[layer][j].getNeighbors();
            int chooseNeighbor = neighbors[rand() % neighbors.size()];
            gene.push_back(make_pair(nodes[layer][j].getNodeId(), chooseNeighbor));
            neighbors.clear();
        }
        // Decode.
        Decode(layer, i, gene, 1);
        
        // Calculate fitness.
        vector<double> allFitness = CalculateFitness(layer, i, gene, 1);
        double fitness = 0.0;
        double Q = 0.0;
        double NMI = 0.0;
        double modularityDensity = 0.0;

        if (allFitness.size() == 4)
        {
            Q = allFitness[0];
            NMI = allFitness[1];
            fitness = allFitness[2];
            modularityDensity = allFitness[3];
        }
        else
        {
            Q = allFitness[0];
            fitness = allFitness[1];
            modularityDensity = allFitness[2];
        }

        // Store the chromosome.
        population.push_back(Gene(i, gene, fitness, Q, NMI, modularityDensity));

        gene.clear();
    }
}