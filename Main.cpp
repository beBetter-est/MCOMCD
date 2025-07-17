/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-07 22:34:45
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-05-29 09:45:43
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include <iostream>
#include <chrono>
#include <fstream>

#include "Constant.h"
#include "Preprocessing.h"
#include "DataWarehouse.h"
#include "BilevelProgramming.h"
// #include "ConsensusCommunity.h"
#include "CalculateIndex.h"

using namespace std;
using namespace std::chrono;

int main()
{
    auto startTime = steady_clock::now();
    for (int i = 0; i < KAPFERER_TAILOR_SHOP_LAYERS; i++)
    {
        cout << endl << "Layer: " << i << endl;

        // 1. Read and store data for each layer.
        cout << "Storage starts..." << endl;
        Preprocessing(KAPFERER_TAILOR_SHOP, i);
        cout << "End of storage..." << endl;

        // 2. The evolutionary algorithm solves the bilevel programming problem.
        cout << "Bilevel programming start..." << endl;
        BilevelProgramming(i);
        cout << "End of bilevel programming..." << endl;
    }

    // 3. Calculation and evaluation index.
    cout << endl << "Calculation and evaluation index starts..." << endl;
    CalculateIndex();
    cout << "End of calculation and evaluation index..." << endl;

    // 4. Clear the data.
    edges.clear();
    adjacencyMatrices.clear();
    nodes.clear();
    layerBestPopulation.clear();
    // consensusPopulation.clear();
    layerBestCommunities.clear();

    duration<double> diff = steady_clock::now() - startTime;
    cout << "The program runs for " << diff.count() << " seconds." << endl;

    string layerPath = layerResultsPath + "/Time.txt";
    ofstream timeFile(layerPath);
    timeFile << "The program runs for " << diff.count() << " seconds." << endl;
    timeFile.close();
    
    return 0;
}