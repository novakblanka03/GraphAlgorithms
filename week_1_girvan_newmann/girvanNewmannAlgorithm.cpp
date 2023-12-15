//
// Created by novak on 10/13/2023.
//

#include "girvanNewmannAlgorithm.h"
#include <queue>
#include <stack>

void bfs(vector<Point> neighList, int start, vector<vector<double>> &flux) {
    vector<int> nrShortest(neighList.size(), 0);
    vector<int> level(neighList.size(), 0);
    vector<int> visited(neighList.size(), 0);
    queue<int> bfsQueue;
    stack<int> auxStack;
    visited[start] = 1;
    level[start] = 0;
    nrShortest[start] = 1;
    bfsQueue.push(start);
    auxStack.push(start);
    while (!bfsQueue.empty()) {
        int startPoint = bfsQueue.front();
        for (int i = 0; i < neighList[startPoint].neighbours.size(); ++i) {
            int neighbour = neighList[startPoint].neighbours[i];
            if (level[neighbour] > level[startPoint] + 1 || level[neighbour] == 0) {
                nrShortest[neighbour] = nrShortest[startPoint];
            } else if (level[neighbour] == level[startPoint] + 1) {
                nrShortest[neighbour] += nrShortest[startPoint];
            }
            if (visited[neighbour] == 0) {
                visited[neighbour] = 1;
                level[neighbour] = level[startPoint] + 1;
                bfsQueue.push(neighbour);
                auxStack.push(neighbour);
            }
        }
        bfsQueue.pop();
    }

    while (!auxStack.empty()) {
        int a = auxStack.top();
        double sum = 0;
        for (int j = 0; j < neighList[a].neighbours.size(); ++j) {
            int c = neighList[a].neighbours[j];
            if (level[c] > level[a]) {
                sum += flux[a][c];
            }
        }
        for (int i = 0; i < neighList[a].neighbours.size(); ++i) {
            int b = neighList[a].neighbours[i];
            if (level[b] < level[a]) {
                double by = (double) nrShortest[b] / (double) nrShortest[a];
                double endSum = (1 + sum) * by;
                flux[a][b] += endSum;
                flux[b][a] += endSum;
            }
        }
        auxStack.pop();
    }
}

double getMaxBetweenness(vector<vector<double>> betweenness) {
    double max = -1;
    for (int i = 0; i < betweenness.size(); ++i) {
        for (int j = 0; j < betweenness[i].size(); ++j) {
            if (betweenness[i][j] > max) {
                max = betweenness[i][j];
            }
        }
    }
    return max;
}

vector<Point> neighListFromFile(int &numberOfPoints, int &numberOfEdges, istream &input) {
    if (!(input >> numberOfPoints >> numberOfEdges)) {
        cerr << "Error reading numberOfPoints and numberOfEdges." << endl;
        // Handle error or return an appropriate value
        // For example, you can throw an exception or return an empty vector.
        return vector<Point>();
    }
    vector<Point> result(numberOfPoints);
    for (int i = 0; i < numberOfEdges; ++i) {
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

void removeEdge(vector<Point> &neighList, int node1, int node2) {
    neighList[node1].neighbours.erase(
            remove(neighList[node1].neighbours.begin(), neighList[node1].neighbours.end(), node2),
            neighList[node1].neighbours.end());
    neighList[node2].neighbours.erase(
            remove(neighList[node2].neighbours.begin(), neighList[node2].neighbours.end(), node1),
            neighList[node2].neighbours.end());
}