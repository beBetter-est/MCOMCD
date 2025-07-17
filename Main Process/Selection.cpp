/*
 * @Author: GCJ 1208651874@q.com
 * @Date: 2024-04-10 18:57:54
 * @LastEditors: GCJ 1208651874@q.com
 * @LastEditTime: 2024-04-10 19:57:54
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "Selection.h"
#include "../Constant.h"
#include "../DataWarehouse.h"

#include <iostream>

using namespace std;

void Selection()
{
    // Binary tournament selection strategy.
    srand(time(NULL));
    for (int i = 0; i < Population_Size; i ++ )
    {
        int num_1 = rand() % Population_Size;
        int num_2 = rand() % Population_Size;
        if (population[num_1].getFitness() > population[num_2].getFitness())
        {
            parentPopulation.push_back(population[num_1]);
        }
        else
        {
            parentPopulation.push_back(population[num_2]);
        }
    }
}