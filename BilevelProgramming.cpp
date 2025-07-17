/*
 * @Author: GCJ 1208651874@q.com
 * @Date: 2024-04-08 20:12:28
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-07-14 11:33:05
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "BilevelProgramming.h"
#include "DataWarehouse.h"
#include "Constant.h"
#include "Main Process/Initialize.h"
#include "Main Process/Selection.h"
#include "Main Process/Crossover.h"
#include "Main Process/Mutation.h"
#include "Main Process/CalculateProgenyFitness.h"
#include "Main Process/UpdatePopulation.h"
#include "Main Process/Interaction.h"
#include "Main Process/ExportResults.h"

#include <iostream>

using namespace std;

void BilevelProgramming(int layer)
{
    // 2.1 Initialize the population.
    cout << "Initialize the population start..." << endl;
    Initialize(layer);
    cout << "End of initializing the population..." << endl;

    // 2.2 Evolutionary iteration.
    for (int i = 0; i < Generation; i ++ )
    {
        cout << endl << "Layer: " << layer << " Generation: " << i << endl;

        // 2.2.1 Selection.
        cout << "Selection starts..." << endl;
        Selection();
        cout << "End of selection..." << endl;

        // 2.2.2 Crossover.
        cout << "Crossover starts..." << endl;
        Crossover(layer);
        cout << "End of crossover..." << endl;

        // 2.2.3 Mutation.
        cout << "Mutation starts..." << endl;
        Mutation(layer);
        cout << "End of mutation..." << endl;

        // 2.2.4 Calculate fitness.
        cout << "Calculate fitness starts..." << endl;
        CalculateProgenyFitness(layer);
        cout << "End of calculating fitness..." << endl;

        // 2.2.5 Updating population and community divisions.
        cout << "Update the population and community divisions starts..." << endl;
        UpdatePopulation(layer, i);
        cout << "End of updating the population and community divisions..." << endl;
        
        // 2.2.6 Interact with upper-level questions to adjust weight values.
        cout << "Interact with upper-level questions to adjust weight values..." << endl;
        Interaction(layer, i);
        cout << "End of interacting with upper-level questions to adjust weight values..." << endl;
        
        // 2.2.8 Clear the data.
        parentPopulation.clear();
        progenyPopulation.clear();
        progenyCommunities.clear();
    }

    // 2.2.7 Store all the information for each layer and the consensus community into a file.
    cout << "Export and store the results..." << endl;
    ExportResults(layer);
    cout << "End of export and store the results..." << endl;

    // Clear the data.
    population.clear();
    communities.clear();
    layerConsensus.clear();
}