/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-08 09:38:00
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-12-23 20:18:00
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */
#ifndef CONSTANT_H
#define CONSTANT_H

#include <string>

using namespace std;

const int RandomSeed = 999;

const int Max_Invariant = 5;

const double Decay_Rate = 0.3;

const string CS_Aarhus = "./Multi-layer Network Datasets/CS-Aarhus_Multiplex_Social";
const string CS_Aarhus_Results = "Multi-layer Network Results/CS-Aarhus_Multiplex_Social/";
const int CS_Aarhus_LAYERS = 5;

const string KAPFERER_TAILOR_SHOP = "./Multi-layer Network Datasets/KAPFERER TAILOR SHOP";
const string KAPFERER_TAILOR_SHOP_Results = "Multi-layer Network Results/KAPFERER TAILOR SHOP/";
const int KAPFERER_TAILOR_SHOP_LAYERS = 4;

const string KAPFERER_TAILOR_SHOP_BIG_CHANGE_10 = "./Multi-layer Network Datasets/KAPFERER TAILOR SHOP_BIG_CHANGE_10";
const string KAPFERER_TAILOR_SHOP_BIG_CHANGE_10_Results = "Multi-layer Network Results/KAPFERER TAILOR SHOP_BIG_CHANGE_10/";
const int KAPFERER_TAILOR_SHOP_BIG_CHANGE_10_LAYERS = 4;

const string London_Transport = "./Multi-layer Network Datasets/London transport";
const string London_Transport_Results = "Multi-layer Network Results/London_Transport/";
const int London_Transport_LAYERS = 3;

const string Arabidopsis_GPI = "./Multi-layer Network Datasets/Arabidopsis GPI";
const string Arabidopsis_GPI_Results = "Multi-layer Network Results/Arabidopsis GPI/";
const int Arabidopsis_GPI_LAYERS = 7;

const string Celegans_GPI = "./Multi-layer Network Datasets/Celegans GPI";
const string Celegans_GPI_Results = "Multi-layer Network Results/Celegans GPI/";
const int Celegans_GPI_LAYERS = 6;

const string CKM_Physicians_Innovation = "./Multi-layer Network Datasets/CKM Physicians Innovation";
const string CKM_Physicians_Innovation_Results = "Multi-layer Network Results/CKM Physicians Innovation/";
const int CKM_Physicians_Innovation_LAYERS = 3;

const int Population_Size = 100; // 200   50  100
const int Generation = 100; // 300   100   100
const double Crossover_Probability = 0.8;
const double Crossover_Probability_Layer = 0.4;
const double Mutation_Probability = 0.2;

#endif