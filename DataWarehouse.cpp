/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-08 17:05:22
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-04-19 22:07:34
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */

#include "DataWarehouse.h"
#include "Constant.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

double weight_1 = 0.5; // The weight of the first objective.
double weight_2 = 0.5; // The weight of the second objective.

string layerResultsPath; // The path to store the results of each layer.

vector<vector<Edge>> edges; // Store the edges of each layer.
vector<Adjacency_Matrices> adjacencyMatrices; // Store the adjacency matrices of each layer.
vector<vector<Node>> nodes; // Store the nodes of each layer.

vector<Gene> population; // Store the population of each layer.
vector<Gene> parentPopulation; // Generated when selected and emptied at the end of each iteration.
vector<Gene> progenyPopulation; // Crossover and mutation are generated and emptied at the end of each iteration.
vector<Gene> layerBestPopulation; // Generated at the end of each iteration and emptied at the end of the program.
// vector<Gene> consensusPopulation; // Generated at the end of each iteration and emptied at the end of the program.
vector<Community> consensusCommunities; // This parameter is generated when the indicator is calculated.

vector<vector<Community>> communities; // Community division of each gene at each level.
// vector<vector<Community>> parentCommunities; // Generated when selected and emptied at the end of each iteration.
vector<vector<Community>> progenyCommunities; // Crossover and mutation are generated and emptied at the end of each iteration.
vector<vector<Community>> layerBestCommunities; // Generated at the end of each iteration and emptied at the end of the program.
// vector<vector<Community>> consensusCommunities; // Generated at the end of each iteration and emptied at the end of the program.

vector<Consensus> layerConsensus; // Generated at the end of each iteration and emptied at the end of all iterations of each layer.
// Consensus allConsensus; // Spawns at the end of all layers and empties at the end of the program.