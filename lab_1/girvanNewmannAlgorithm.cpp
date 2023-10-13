//
// Created by novak on 10/13/2023.
//

#include "girvanNewmannAlgorithm.h"
#include <queue>
#include <stack>


vector<Point> neighListFromFile(int &numberOfPoints, int &numberOfEdges, istream &input) {
    if (!(input >> numberOfPoints >> numberOfEdges)) {
        cerr << "Error reading numberOfPoints and numberOfEdges." << endl;
        // Handle error or return an appropriate value
        // For example, you can throw an exception or return an empty vector.
        return vector<Point>();
    }
    vector<Point> result(numberOfPoints);
    for (int i = 0; i < numberOfEdges - 1; ++i) {
        int start, end;
        if (!(input >> start >> end)) {
            cerr << "Error reading edge " << i << "." << endl;
            // Handle error or return an appropriate value
            // For example, you can throw an exception or return the partially constructed vector.
            return result;
        }
        result[--start].neighbours.emplace_back(--end);
        result[end].neighbours.emplace_back(start);
    }
    return result;
}

void printNeighList(vector<Point> neighList, int &numberOfPoints, int &numberOfEdges, ostream &output) {
    for (int i = 0; i < neighList.size(); ++i) {
        output << i + 1 << " -> ";
        for (int neigh: neighList[i].neighbours) {
            output << neigh + 1 << " ";
        }
        output << endl;
    }
    output << endl;
}

void nrOfShortestPathsFromFirst(vector<Point> neighList, double **betweenness, int start) {
    vector<int> level(neighList.size(), -1);
    vector<int> nrShortest(neighList.size(), 0);
    start = --start;
    queue<int> bfsQueue;
    stack<int> betwStack;
    level[start] = 0;
    nrShortest[start] = 1;
    bfsQueue.push(start);
    betwStack.push(start);
    while (!bfsQueue.empty()) {
        int startPoint = bfsQueue.front();
        //cout << startPoint + 1 << " ";
        for (int i = 0; i < neighList[startPoint].neighbours.size(); ++i) {
            int neighbour = neighList[startPoint].neighbours[i];
            if (level[neighbour] == -1) {
                level[neighbour] = level[startPoint] + 1;
                bfsQueue.push(neighbour);
                betwStack.push(neighbour);
            }
        }
        if (level[startPoint] != 0) {
            for (int j = 0; j < neighList[startPoint].neighbours.size(); ++j) {
                if (level[neighList[startPoint].neighbours[j]] < level[startPoint]) {
                    nrShortest[startPoint] += nrShortest[neighList[startPoint].neighbours[j]];
                }
            }
        }
        bfsQueue.pop();
    }
    while (!betwStack.empty()) {
        int a = betwStack.top();
        for (int i = 0; i < neighList[a].neighbours.size(); ++i) {
            int b = neighList[a].neighbours[i];
            if (level[b] < level[a]) {
                double sum = 0;
                for (int j = 0; j < neighList[a].neighbours.size(); ++j) {
                    int c = neighList[a].neighbours[j];
                    if (level[c] > level[a]) {
                        sum += betweenness[a][c];
                    }
                }
                double by = (double) nrShortest[b] / (double) nrShortest[a];
                double endSum = (1 + sum) * by;
                betweenness[a][b] = endSum;
                betweenness[b][a] = endSum;
            }
        }
        betwStack.pop();
    }
}

void removeEdge(vector<Point> &neighList, int node1, int node2) {
    neighList[node1].neighbours.erase(
            remove(neighList[node1].neighbours.begin(), neighList[node1].neighbours.end(), node2),
            neighList[node1].neighbours.end());
    neighList[node2].neighbours.erase(
            remove(neighList[node2].neighbours.begin(), neighList[node2].neighbours.end(), node1),
            neighList[node2].neighbours.end());
}

double determineMaxBetweenness(double **final_betweenness, int numberOfPoints, int &maxEdgeBegin, int &maxEdgeEnd) {
    double max = -1;
    for (int i = 0; i < numberOfPoints; ++i) {
        for (int j = 0; j < numberOfPoints; ++j) {
            if (final_betweenness[i][j] > max) {
                max = final_betweenness[i][j];
                maxEdgeBegin = i;
                maxEdgeEnd = j;
            }
        }
    }
    return max;
}
