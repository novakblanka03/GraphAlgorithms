//
// Created by novak on 10/13/2023.
//

#ifndef LAB_1_GIRVANNEWMANNALGORITHM_H
#define LAB_1_GIRVANNEWMANNALGORITHM_H

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef struct Point {
    vector<int> neighbours;
} Point;

void bfs(vector<Point> neighList, int start, vector<vector<double>> &flux);

void removeEdge(vector<Point> &neighList, int node1, int node2);

vector<Point> neighListFromFile(int &numberOfPoints, int &numberOfEdges, istream &input = cin);

void printNeighList(vector<Point> neighList, int &numberOfPoints, int &numberOfEdges, ostream &output = cout);

double getMaxBetweenness(vector<vector<double>> betweenness);

#endif //LAB_1_GIRVANNEWMANNALGORITHM_H
