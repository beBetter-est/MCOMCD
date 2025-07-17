/*
 * @Author: GCJ 1208651874@q.com
 * @Date: 2024-04-10 21:47:19
 * @LastEditors: GCJ 1208651874@q.com
 * @LastEditTime: 2024-04-11 15:25:28
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "CalculateProgenyFitness.h"
#include "../Constant.h"
#include "../DataWarehouse.h"
#include "CalculateFitness.h"
#include "Decode.h"

#include <iostream>

using namespace std;

void CalculateProgenyFitness(int layer)
{
    for (int i = 0; i < Population_Size; i ++ )
    {
        Decode(layer, i, progenyPopulation[i].getGene(), 2);
        vector<double> allFitness = CalculateFitness(layer, i, progenyPopulation[i].getGene(), 2);
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
        progenyPopulation[i].setFitness(fitness);
        progenyPopulation[i].setQ(Q);
        progenyPopulation[i].setNMI(NMI);
        progenyPopulation[i].setDensity(modularityDensity);
    }
}
