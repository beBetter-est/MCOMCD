/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-09 19:57:20
 * @LastEditors: GCJ 1208651874@q.com
 * @LastEditTime: 2024-04-11 15:15:17
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */

#ifndef CALCULATEFITNESS_H
#define CALCULATEFITNESS_H

#include <vector>
#include <utility>

using namespace std;

vector<double> CalculateFitness(int layer, int geneId, vector<pair<int,int>> gene, int type);

#endif