/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-15 15:26:52
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-05-29 09:24:31
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "ConsensusCommunity.h"
#include "DataWarehouse.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool cmp(pair<int,int> a, pair<int,int> b)
{
    if (a.first != b.first)
        return a.first < b.first;
    else
        return a.second < b.second;
}

void ConsensusCommunity()
{
    vector<pair<int,int>> allPairs;
    for (int i = 0; i < layerBestPopulation.size(); i ++ )
    {
        vector<pair<int,int>> temp = layerBestPopulation[i].getGene();
        for (int j = 0; j < temp.size(); j ++ )
        {
            allPairs.push_back(temp[j]);
        }
        temp.clear();
    }
    sort(allPairs.begin(), allPairs.end(), cmp);

    vector<pair<int,int>> consensusResult;


    // Calculation and evaluation index.


    consensusResult.clear();
    allPairs.clear();
}