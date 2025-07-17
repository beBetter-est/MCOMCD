/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-15 10:28:05
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-04-15 15:22:20
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#include "ExportResults.h"
#include "../Constant.h"
#include "../DataWarehouse.h"

#include <iostream>
#include <chrono>
#include <ctime> 
#include <fstream>
#include <direct.h>

using namespace std;
using namespace std::chrono;

void ExportResults(int layer)
{
    if (layer == 0)
    {
        auto now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);
        tm *local_time = localtime(&now_time);

        char timeStr[100];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H-%M-%S", local_time);
        string folderName(timeStr);

        string resultPath = "./" + KAPFERER_TAILOR_SHOP_Results + folderName;
        if(mkdir(resultPath.c_str()) == -1)
        {
            cerr << "mkdir failed!" << endl;
            exit(1);
        }

        layerResultsPath = resultPath;
    }

    string layerPath = layerResultsPath + "/Layer_" + to_string(layer) + "_BestPopulation.txt";
    ofstream outputFile(layerPath);
    if (!outputFile.is_open())
    {
        cerr << "outputFile open failed!" << endl;
        exit(1);
    }

    outputFile << "Layer : " << layer << endl;
    outputFile << "Fitness : " << layerBestPopulation[layer].getFitness() << endl;
    outputFile << "Q : " << layerBestPopulation[layer].getQ() << endl;
    outputFile << "NMI : " << layerBestPopulation[layer].getNMI() << endl;
    outputFile << "modularityDensity : " << layerBestPopulation[layer].getDensity() << endl;
    outputFile << "Gene : " << endl;
    for (int i = 0; i < layerBestPopulation[layer].getGene().size(); i++)
    {
        outputFile << layerBestPopulation[layer].getGene()[i].first << " " << layerBestPopulation[layer].getGene()[i].second << endl;
    }

    if (outputFile.is_open())
    {
        outputFile.close();
    }

    string communityPath = layerResultsPath + "/Layer_" + to_string(layer) + "_BestCommunities.txt";
    ofstream communityFile(communityPath);
    if (!communityFile.is_open())
    {
        cerr << "communityFile open failed!" << endl;
        exit(1);
    }

    communityFile << "Layer : " << layer << endl;
    for (int i = 0; i < layerBestCommunities[layer].size(); i++)
    {
        communityFile << "Community_" << layerBestCommunities[layer][i].getCommunityId() << " : ";
        for (int j = 0; j < layerBestCommunities[layer][i].getNodes().size(); j++)
        {
            communityFile << layerBestCommunities[layer][i].getNodes()[j] << " ";
        }
        communityFile << endl;
    }

    if (communityFile.is_open())
    {
        communityFile.close();
    }

    string layerConsensusPath = layerResultsPath + "/Layer_" + to_string(layer) + "_ConsensusPopulation.txt";
    ofstream consensusFile(layerConsensusPath);
    if (!consensusFile.is_open())
    {
        cerr << "consensusFile open failed!" << endl;
        exit(1);
    }

    int tempId = Generation - 1;

    consensusFile << "Layer : " << layer << endl;
    consensusFile << "Fitness : " << layerConsensus[tempId].getFitness() << endl;
    consensusFile << "Q : " << layerConsensus[tempId].getQ() << endl;
    consensusFile << "NMI : " << layerConsensus[tempId].getNMI() << endl;
    consensusFile << "Jaccard : " << layerConsensus[tempId].getJaccard() << endl;
    consensusFile << "weight_a : " << layerConsensus[tempId].getWeight_a() << endl;
    consensusFile << "weight_b : " << layerConsensus[tempId].getWeight_b() << endl;  

    consensusFile << "Gene : " << endl;
    for (int i = 0; i < layerConsensus[tempId].getGene().size(); i++)
    {
        consensusFile << layerConsensus[tempId].getGene()[i].first << " " << layerConsensus[tempId].getGene()[i].second << endl;
    }

    consensusFile << "Community : " << endl;
    for (int i = 0; i < layerConsensus[tempId].getCommunities().size(); i++)
    {
        consensusFile << "Community_" << layerConsensus[tempId].getCommunities()[i].getCommunityId() << " : ";
        for (int j = 0; j < layerConsensus[tempId].getCommunities()[i].getNodes().size(); j++)
        {
            consensusFile << layerConsensus[tempId].getCommunities()[i].getNodes()[j] << " ";
        }
        consensusFile << endl;
    }

    if (consensusFile.is_open())
    {
        consensusFile.close();
    }
}