/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-08 16:00:25
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-04-09 11:33:38
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "Preprocessing.h"
#include "DataWarehouse.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <set>

using namespace std;

void Preprocessing(string dataset, int layer)
{
    // 1.1 Store edge data for each layer.
    ifstream inputFile(dataset + "/Clean_Data/" + to_string(layer + 1) + ".txt");
    if (!inputFile.is_open())
    {
        cerr << "inputFile open failed!" << endl;
    }

    vector<Edge> edgesData;
    int firstNode, secondNode, maxNode = 0;
    set<int> nodesSet;
    while (inputFile >> firstNode >> secondNode)
    {
        edgesData.push_back(Edge(layer + 1, firstNode, secondNode));
        maxNode = max(maxNode, max(firstNode, secondNode));
        nodesSet.insert(firstNode);
        nodesSet.insert(secondNode);
    }
    edges.push_back(edgesData);

    if (inputFile.is_open())
    {
        inputFile.close();
    }
    
    edgesData.clear();

    // 1.2 Store adjacency matrix data for each layer.
    vector<vector<int>> matrix(maxNode + 1, vector<int>(maxNode + 1, 0));
    for (int i = 0; i < edges[layer].size(); i++)
    {
        firstNode = edges[layer][i].getFirstNode();
        secondNode = edges[layer][i].getSecondNode();
        matrix[firstNode][secondNode] = 1;
        matrix[secondNode][firstNode] = 1;
    }
    adjacencyMatrices.push_back(Adjacency_Matrices(layer + 1, matrix));

    // 1.3 Store node data for each layer.
    vector<Node> nodesData;
    for (int i = 0; i < nodesSet.size(); i++)
    {
        vector<int> neighbor;
        auto it = nodesSet.begin();
        advance(it, i);
        for (int j = 0; j < matrix[*it].size(); j++)
        {
            if (matrix[*it][j] == 1)
            {
                neighbor.push_back(j);
            }
        }
        nodesData.push_back(Node(layer + 1, *it, neighbor));
        neighbor.clear();
    }
    nodes.push_back(nodesData);

    matrix.clear();
    nodesData.clear();
    nodesSet.clear();
}