//
// Created by novak on 10/13/2023.
//

#include "girvanNewmannAlgorithm.h"
#include <queue>
#include <stack>

vector<Edge> edgeListFromFile(int numberOfPoints, int numberOfEdges, istream &input) {
    if (!(input >> numberOfPoints >> numberOfEdges)) {
        cerr << "Error reading numberOfPoints and numberOfEdges." << endl;
        // Handle error or return an appropriate value
        // For example, you can throw an exception or return an empty vector.
        return vector<Edge>();
    }
    vector<Edge> result;
    for (int i = 0; i < numberOfEdges; ++i) {
        int start, end;
        if (!(input >> start >> end)) {
            cerr << "Error reading edge " << i << "." << endl;
            // Handle error or return an appropriate value
            // For example, you can throw an exception or return the partially constructed vector.
            return result;
        }
        result.push_back({--start, --end, 0, 0});
    }
    return result;
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

void nrOfShortestPathsFromFirst(vector<Point> neighList, vector<Edge> &edgeList, int start) {
//    for (int i = 0; i < edgeList.size(); ++i) {
//        edgeList[i].betweenness = 1;
//    }
    vector<int> level(neighList.size(), -1);
    vector<int> nrShortest(neighList.size(), 0);
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
//    for (int i = 0; i < neighList.size(); ++i) {
//        cout << i + 1 << " Level: " << level[i] << " nrShortest: " << nrShortest[i] << endl;
//    }
    while (!betwStack.empty()) {
        int a = betwStack.top();
        double sum = 0;
        for (int j = 0; j < neighList[a].neighbours.size(); ++j) {
            int c = neighList[a].neighbours[j];
            if (level[c] > level[a]) {
                int pos = findEdgePosition(edgeList, a, c);
                sum += edgeList[pos].betweenness;
            }
        }
        for (int i = 0; i < neighList[a].neighbours.size(); ++i) {
            int b = neighList[a].neighbours[i];
            if (level[b] < level[a]) {
                double by = (double) nrShortest[b] / (double) nrShortest[a];
                double endSum = (1 + sum) * by;
                int pos1 = findEdgePosition(edgeList, a, b);
                edgeList[pos1].betweenness += endSum;
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

double determineMaxBetweenness(vector<Edge> edgeList) {
    double max = -1;
    for (int i = 0; i < edgeList.size(); ++i) {
        if (edgeList[i].final_betweenness > max) {
            max = edgeList[i].final_betweenness;
        }
    }
    return max;
}

void printEdgeList(vector<Edge> vector1) {
    for (int i = 0; i < vector1.size(); ++i) {
        cout << vector1[i].start + 1 << " " << vector1[i].end + 1 << " " << vector1[i].betweenness << " "
             << vector1[i].final_betweenness << endl;
    }
    cout << endl;
}

int findEdgePosition(vector<Edge> edgeVector, int targetStart, int targetEnd) {
    auto condition = [targetStart, targetEnd](const Edge &edge) {
        return edge.start == targetStart && edge.end == targetEnd;
    };

    auto it = std::find_if(edgeVector.begin(), edgeVector.end(), condition);

    if (it != edgeVector.end()) {
        return (int) distance(edgeVector.begin(), it); // Element found, return its position
    } else {
        return -1; // Element not found
    }
}

void removeEdgeFromEdgeList(vector<Edge> &edgeList, int index) {
    if (index >= 0 && index < edgeList.size()) {
        edgeList.erase(edgeList.begin() + index);
    } else {
        cerr << "Invalid index. Edge not removed." << endl;
    }
}
