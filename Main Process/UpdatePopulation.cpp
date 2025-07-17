/*
 * @Author: GCJ 1208651874@q.com
 * @Date: 2024-04-11 10:51:08
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-05-29 09:41:42
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "UpdatePopulation.h"
#include "Decode.h"
#include "../DataWarehouse.h"
#include "../Constant.h"

#include <iostream>
#include <algorithm>

using namespace std;

void UpdatePopulation(int layId, int generationId)
{
    population.clear();
    
    sort(parentPopulation.begin(), parentPopulation.end(), [](Gene a, Gene b) { return a.getFitness() > b.getFitness(); });
    sort(progenyPopulation.begin(), progenyPopulation.end(), [](Gene a, Gene b) { return a.getFitness() > b.getFitness(); });

    int parentIndex = 0, progenyIndex = 0;
    for (int i = 0; i < Population_Size; i ++ )
    {
        if (parentPopulation[parentIndex].getFitness() > progenyPopulation[progenyIndex].getFitness())
        {
            population.push_back(parentPopulation[parentIndex]);
            population[i].setGeneId(i);
            parentIndex ++ ;
        }
        else
        {
            population.push_back(progenyPopulation[progenyIndex]);
            population[i].setGeneId(i);
            progenyIndex ++ ;
        }
    }

    // Store the optimal individuals for each layer and their community partitioning results.
    if (generationId == 0)
    {
        layerBestPopulation.push_back(population[0]);
    }
    else
    {
        layerBestPopulation[layId] = population[0];
    }

    if (generationId == Generation - 1)
    {
        Decode(layId, generationId, population[0].getGene(), 4);
    }
}