#include <iostream>

#include <vector>
#include <fstream>
#include <queue>

using namespace std;

typedef struct Point {
    vector<int> neighbours;
} Point;

vector<Point> neighListFromFile(int &numberOfPoints, int &numberOfEdges, istream &input = cin);

void printNeighList(vector<Point> neighList, int &numberOfPoints, int &numberOfEdges, ostream &output = cout);

void nrOfShortestPathsFromFirst(vector<Point> neighList, vector<int> &level, vector<int> &nrShortest, vector<int> &orderOfBFS, int start = 1);

void removeEdge(vector<Point>& neighList, int node1, int node2);

int main(int argc, char **argv) {
    ifstream graphIn(argv[1]);
    if (!graphIn) {
        cerr << "Error opening input file." << endl;
        return 1; // Return an error code
    }
    int numberOfPoints, numberOfEdges;
    vector<Point> neighList = neighListFromFile(numberOfPoints, numberOfEdges, graphIn);
    printNeighList(neighList, numberOfPoints, numberOfEdges);

    vector<int> level(neighList.size(), -1);
    vector<int> nrShortest(neighList.size(), 0);
    vector<int> orderOfBFS(0);
    nrOfShortestPathsFromFirst(neighList, level, nrShortest, orderOfBFS);

    cout << "The order of the breadth first search is: ";
    for (int i = 0; i < numberOfPoints; ++i) {
        cout << orderOfBFS[i] << " ";
    }
    cout << endl;

    cout << "\nLevels of nodes " << endl;
    for (int i = 0; i < neighList.size(); ++i) {
        cout << i + 1 << ": " << level[i] << endl;
    }

    cout << "\nNr of shortest paths from first node " << endl;
    for (int i = 0; i < neighList.size(); ++i) {
        cout << i + 1 << ": " << nrShortest[i] << endl;
    }

    graphIn.close();
    return 0;
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
            cerr << "Error reading edge " << i + 1 << "." << endl;
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

void nrOfShortestPathsFromFirst(vector<Point> neighList, vector<int> &level, vector<int> &nrShortest, vector<int> &orderOfBFS, int start) {
    start = --start;
    queue<int> bfsQueue;
    level[start] = 0;
    nrShortest[start] = 1;
    bfsQueue.push(start);
    while (!bfsQueue.empty()) {
        int startPoint = bfsQueue.front();
        orderOfBFS.emplace_back(startPoint + 1);
        //cout << startPoint + 1 << " ";
        for (int i = 0; i < neighList[startPoint].neighbours.size(); ++i) {
            int neighbour = neighList[startPoint].neighbours[i];
            if (level[neighbour] == -1) {
                level[neighbour] = level[startPoint] + 1;
                bfsQueue.push(neighbour);
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
}

void removeEdge(vector<Point>& neighList, int node1, int node2) {
    neighList[node1].neighbours.erase(remove(neighList[node1].neighbours.begin(), neighList[node1].neighbours.end(), node2), neighList[node1].neighbours.end());
    neighList[node2].neighbours.erase(remove(neighList[node2].neighbours.begin(), neighList[node2].neighbours.end(), node1), neighList[node2].neighbours.end());
}