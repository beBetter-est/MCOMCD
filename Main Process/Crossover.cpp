/*
 * @Author: GCJ 1208651874@q.com
 * @Date: 2024-04-10 19:58:21
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-07-14 10:29:16
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "Crossover.h"
#include "../DataWarehouse.h"
#include "../Constant.h"

#include <iostream>
#include <vector>

using namespace std;

void Crossover(int layer)
{
    // Binary mask crossover operator.
    srand(time(NULL));
    for (int i = 0; i < Population_Size; i ++ )
    {
        double random = rand() % (RandomSeed + 1) / (double)(RandomSeed + 1);
        if (random > Crossover_Probability)
        {
            progenyPopulation.push_back(Gene(i, parentPopulation[i].getGene(), 0.0, 0.0, 0.0, 0.0));
            continue;
        }

        random = rand() % (RandomSeed + 1) / (double)(RandomSeed + 1);

        // int num_1 = rand() % Population_Size;
        // int num_2 = rand() % Population_Size;
        
        // vector<pair<int,int>> temporaryGene;
        // int geneLength = parentPopulation[num_1].getGene().size();

        // // 使用二进制掩码决定新个体的每个基因来自哪个父代
        // for (int j = 0; j < geneLength; j ++ )
        // {
        //     int mask = rand() % 2;
        //     if (mask == 1)
        //     {
        //         temporaryGene.push_back(parentPopulation[num_1].getGene()[j]);
        //     }
        //     else
        //     {
        //         temporaryGene.push_back(parentPopulation[num_2].getGene()[j]);
        //     }   
        // }
        
        // // 将生成的新基因添加到子代种群中
        // progenyPopulation.push_back(Gene(i, temporaryGene, 0.0, 0.0, 0.0, 0.0));
        
        // temporaryGene.clear();

        if (layer == 0 || random > Crossover_Probability_Layer)
        {
            int num_1 = rand() % Population_Size;
            int num_2 = rand() % Population_Size;
            
            vector<pair<int,int>> temporaryGene;
            int geneLength = parentPopulation[num_1].getGene().size();

            for (int j = 0; j < geneLength; j ++ )
            {
                int mask = rand() % 2;
                if (mask == 1)
                {
                    temporaryGene.push_back(parentPopulation[num_1].getGene()[j]);
                }
                else
                {
                    temporaryGene.push_back(parentPopulation[num_2].getGene()[j]);
                }   
            }
            
            progenyPopulation.push_back(Gene(i, temporaryGene, 0.0, 0.0, 0.0, 0.0));
            
            temporaryGene.clear();
        }
        else
        {
            int num_1 = rand() % Population_Size;
            vector<pair<int,int>> temporaryGene;
            int geneLength = parentPopulation[num_1].getGene().size();

            int tempCounter = 0;
            for (int j = 0; j < geneLength; j ++ ) 
            {
                while (layerBestPopulation[layer - 1].getGene()[tempCounter].first < parentPopulation[num_1].getGene()[j].first)
                {
                    tempCounter ++ ;
                }
                if (tempCounter <= layerBestPopulation[layer - 1].getGene().size() && layerBestPopulation[layer - 1].getGene()[tempCounter].first == parentPopulation[num_1].getGene()[j].first)
                {
                    if (adjacencyMatrices[layer].getMatrix()[layerBestPopulation[layer - 1].getGene()[tempCounter].first][layerBestPopulation[layer - 1].getGene()[tempCounter].second] == 1)
                    {
                        temporaryGene.push_back(make_pair(layerBestPopulation[layer - 1].getGene()[tempCounter].first, layerBestPopulation[layer - 1].getGene()[tempCounter].second));
                    }
                    else
                    {
                        temporaryGene.push_back(make_pair(parentPopulation[num_1].getGene()[j].first, parentPopulation[num_1].getGene()[j].second));
                    }
                }
                else
                {
                    temporaryGene.push_back(make_pair(parentPopulation[num_1].getGene()[j].first, parentPopulation[num_1].getGene()[j].second));
                }
            }

            progenyPopulation.push_back(Gene(i, temporaryGene, 0.0, 0.0, 0.0, 0.0));
            
            temporaryGene.clear();
        }

        // int num_1 = rand() % Population_Size;
        // int num_2 = rand() % Population_Size;
        
        // vector<pair<int,int>> temporaryGene;
        // int geneLength = parentPopulation[num_1].getGene().size();

        // for (int j = 0; j < geneLength; j ++ )
        // {
        //     int mask = rand() % 2;
        //     if (mask == 1)
        //     {
        //         temporaryGene.push_back(parentPopulation[num_1].getGene()[j]);
        //     }
        //     else
        //     {
        //         temporaryGene.push_back(parentPopulation[num_2].getGene()[j]);
        //     }   
        // }
        
        // progenyPopulation.push_back(Gene(i, temporaryGene, 0.0, 0.0, 0.0));
        
        // temporaryGene.clear();
    }
}