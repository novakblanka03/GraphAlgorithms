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

vector<Point> neighListFromFile(int &numberOfPoints, int &numberOfEdges, istream &input = cin);

void printNeighList(vector<Point> neighList, int &numberOfPoints, int &numberOfEdges, ostream &output = cout);

void nrOfShortestPathsFromFirst(vector<Point> neighList, double **betweenness, int start = 1);

void removeEdge(vector<Point> &neighList, int node1, int node2);

double determineMaxBetweenness(double **final_betweenness, int numberOfPoints, int &maxEdgeBegin, int &maxEdgeEnd);

#endif //LAB_1_GIRVANNEWMANNALGORITHM_H
