/*
 * @Author: GCJ 1208651874@qq.com
 * @Date: 2024-04-08 16:34:40
 * @LastEditors: GCJ 1208651874@qq.com
 * @LastEditTime: 2024-05-25 18:59:49
 * @Description: 
 * 
 * Copyright (c) 2024 by ${GCJ_1208651874@q.com}, All Rights Reserved. 
 */

#ifndef DATAWAREHOUSE_H
#define DATAWAREHOUSE_H

#include <vector>
#include <utility>
#include <string>

using namespace std;

extern double weight_1; // The weight of the first objective.
extern double weight_2; // The weight of the second objective.

extern string layerResultsPath; // The path to store the results of each layer.

// Store the edges of each layer.
class Edge
{
    private:
        int layerId;
        int firstNode;
        int secondNode;
    public:
        Edge(int layerId, int firstNode, int secondNode) {
            this->layerId = layerId;
            this->firstNode = firstNode;
            this->secondNode = secondNode;
        };
        int getLayerId() {
            return layerId;
        }
        int getFirstNode() {
            return firstNode;
        }
        int getSecondNode() {
            return secondNode;
        }
        void setLayerId(int newLayerId) {
            layerId = newLayerId;
        }
        void setFirstNode(int newFirstNode) {
            firstNode = newFirstNode;
        }
        void setSecondNode(int newSecondNode) {
            secondNode = newSecondNode;
        }
};
// The program starts to store, and clears at the end.
extern vector<vector<Edge>> edges;

// Store the adjacency matrices of each layer.
class Adjacency_Matrices {
    private:
        int layerId;
        vector<vector<int>> matrix;
    public:
        Adjacency_Matrices(int layerId, vector<vector<int>>& matrix) {
            this->layerId = layerId;
            this->matrix = matrix;
        };
        int getLayerId() {
            return layerId;
        }
        const vector<vector<int>>& getMatrix() {
            return matrix;
        }
        void setLayerId(int newLayerId) {
            layerId = newLayerId;
        }
        void setMatrix(vector<vector<int>>& newMatrix) {
            matrix = newMatrix;
        }
};
// Stored while preprocessing data and emptied at the end of the program.
extern vector<Adjacency_Matrices> adjacencyMatrices;

// Store the nodes of each layer.
class Node {
    private:
        int layerId;
        int nodeId;
        vector<int> neighbors;
    public:
        Node(int layerId, int nodeId, vector<int>& neighbors) {
            this->layerId = layerId;
            this->nodeId = nodeId;
            this->neighbors = neighbors;
        };
        int getLayerId() {
            return layerId;
        }
        int getNodeId() {
            return nodeId;
        }
        const vector<int>& getNeighbors() {
            return neighbors;
        }
        void setLayerId(int newLayerId) {
            layerId = newLayerId;
        }
        void setNodeId(int newNodeId) {
            nodeId = newNodeId;
        }
        void setNeighbors(vector<int>& newNeighbors) {
            neighbors = newNeighbors;
        }
};
// Stored while preprocessing data and emptied at the end of the program.
extern vector<vector<Node>> nodes;

// Store the population of each layer.
class Gene {
    private:
        int geneId;
        vector<pair<int,int>> gene;
        double fitness;
        double Q;
        double NMI;
        double density;
    public:
        Gene(int geneId, const vector<pair<int,int>>& gene, double fitness, double Q, double NMI, double density) {
            this->geneId = geneId;
            this->gene = gene;
            this->fitness = fitness;
            this->Q = Q;
            this->NMI = NMI;
            this->density = density;
        };
        int getGeneId() {
            return geneId;
        }
        const vector<pair<int,int>>& getGene() const {
            return gene;
        }
        double getFitness() {
            return fitness;
        }
        double getQ() {
            return Q;
        }
        double getNMI() {
            return NMI;
        }
        double getDensity() {
            return density;
        }
        void setGeneId(int newGeneId) {
            geneId = newGeneId;
        }
        void setGene(vector<pair<int,int>>& newGene) {
            gene = newGene;
        }
        void setFitness(double newFitness) {
            fitness = newFitness;
        }
        void setQ(double newQ) {
            Q = newQ;
        }
        void setNMI(double newNMI) {
            NMI = newNMI;
        }
        void setDensity(double newDensity) {
            density = newDensity;
        }
};
// Stored when the population is initialized and emptied at the end of evolution.
extern vector<Gene> population;
// Generated when selected and emptied at the end of each iteration.
extern vector<Gene> parentPopulation;
// Crossover and mutation are generated and emptied at the end of each iteration.
extern vector<Gene> progenyPopulation;
// Generated at the end of each iteration and emptied at the end of the program.
extern vector<Gene> layerBestPopulation;
// Generated at the end of each iteration and emptied at the end of the program.
// extern vector<Gene> consensusPopulation;

// Community division of each gene at each level.
class Community {
    private:
        int geneId;
        int communityId;
        vector<int> nodes;
    public:
        Community(int geneId, int communityId, vector<int>& nodes) {
            this->geneId = geneId;
            this->communityId = communityId;
            this->nodes = nodes;
        };
        int getGeneId() {
            return geneId;
        }
        int getCommunityId() const {
            return communityId;
        }
        const vector<int>& getNodes() const {
            return nodes;
        }
        void setGeneId(int newGeneId) {
            geneId = newGeneId;
        }
        void setCommunityId(int newCommunityId) {
            communityId = newCommunityId;
        }
        void setNodes(vector<int>& newNodes) {
            nodes = newNodes;
        }
};
// Stored when the population is initialized and emptied at the end of evolution.
extern vector<vector<Community>> communities;
// Generated when selected and emptied at the end of each iteration.
// extern vector<vector<Community>> parentCommunities;
// Crossover and mutation are generated and emptied at the end of each iteration.
extern vector<vector<Community>> progenyCommunities;
// Generated at the end of each iteration and emptied at the end of the program.
extern vector<vector<Community>> layerBestCommunities;
// Generated at the end of each iteration and emptied at the end of the program.
// extern vector<vector<Community>> consensusCommunities;
// This parameter is generated when the indicator is calculated.
extern vector<Community> consensusCommunities;

// Store consensus community information.
class Consensus {
    private:
        int consensusId;
        vector<pair<int,int>> gene;
        vector<Community> communities;
        double fitness;
        double Q;
        double NMI;
        double Jaccard;
        double weight_a;
        double weight_b;
    public:
        Consensus(int consensusId, vector<pair<int,int>>& gene, vector<Community>& communities, double fitness, double Q, double NMI, double Jaccard, double weight_a, double weight_b) {
            this->consensusId = consensusId;
            this->gene = gene;
            this->communities = communities;
            this->fitness = fitness;
            this->Q = Q;
            this->NMI = NMI;
            this->Jaccard = Jaccard;
            this->weight_a = weight_a;
            this->weight_b = weight_b;
        };
        int getConsensusId() {
            return consensusId;
        }
        const vector<pair<int,int>>& getGene() {
            return gene;
        }
        const vector<Community>& getCommunities() const {
            return communities;
        }
        double getFitness() {
            return fitness;
        }
        double getQ() {
            return Q;
        }
        double getNMI() {
            return NMI;
        }
        double getJaccard() {
            return Jaccard;
        }
        double getWeight_a() {
            return weight_a;
        }
        double getWeight_b() {
            return weight_b;
        }
        void setConsensusId(int newConsensusId) {
            consensusId = newConsensusId;
        }
        void setGene(vector<pair<int,int>>& newGene) {
            gene = newGene;
        }
        void setCommunities(vector<Community>& newCommunities) {
            communities = newCommunities;
        }
        void setFitness(double newFitness) {
            fitness = newFitness;
        }
        void setQ(double newQ) {
            Q = newQ;
        }
        void setNMI(double newNMI) {
            NMI = newNMI;
        }
        void setJaccard(double newJaccard) {
            Jaccard = newJaccard;
        }
        void setWeight_a(double newWeight_a) {
            weight_a = newWeight_a;
        }
        void setWeight_b(double newWeight_b) {
            weight_b = newWeight_b;
        }
};
// Generated at the end of each iteration and emptied at the end of all iterations of each layer.
extern vector<Consensus> layerConsensus;
// Spawns at the end of all layers and empties at the end of the program.
// extern Consensus allConsensus;
#endif