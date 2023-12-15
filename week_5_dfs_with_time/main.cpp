#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

enum Color {
    WHITE, GRAY, BLACK
};

enum EdgeType{
    TREE, BACK, FORWARD, CROSS
};

typedef struct Point {
    vector<int> neighbours;
} Point;

void depthFirstSearch(vector<Point> &neighList, int numberOfPoints, int &time, vector<int> &reach, vector<int> &leave, vector<vector<int>> &edgeType);

void dfsTraversal(vector<Point> &neighList, int u, vector<int> &color, int &time, vector<int> &reach, vector<int> &leave, vector<vector<int>> &edgeType);

vector<Point> neighListFromFile(int &numberOfPoints, int &numberOfEdges, istream &input = cin);

void printNeighList(vector<Point> neighList, int &numberOfPoints, int &numberOfEdges, ostream &output = cout);

int main(int argc, char **argv) {
    ifstream graphIn(argv[1]);
    if (!graphIn) {
        cerr << "Error opening input file." << endl;
        return 1; // Return an error code
    }
    int numberOfPoints, numberOfEdges;
    vector<Point> neighList = neighListFromFile(numberOfPoints, numberOfEdges, graphIn);
    printNeighList(neighList, numberOfPoints, numberOfEdges);

    vector<int> reach(numberOfPoints);
    vector<int> leave(numberOfPoints);
    vector<vector<int>> edgeType(numberOfPoints, vector<int>(numberOfPoints, -1));
    int time = 0;

    depthFirstSearch(neighList, numberOfPoints, time, reach, leave, edgeType);
    cout << endl;
    cout << endl;
    for (int i = 0; i < numberOfPoints; ++i) {
        for (int j = 0; j < numberOfPoints; ++j) {
            cout << edgeType[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}

void depthFirstSearch(vector<Point> &neighList, int numberOfPoints, int &time, vector<int> &reach, vector<int> &leave, vector<vector<int>> &edgeType) {
    vector<int> color(numberOfPoints);
    for (int i = 0; i < numberOfPoints; ++i) {
        color[i] = WHITE;
    }
    for (int i = 0; i < numberOfPoints; ++i) {
        if (color[i] == WHITE) {
            dfsTraversal(neighList, i, color, time, reach, leave, edgeType);
        }
    }
}

void dfsTraversal(vector<Point> &neighList, int u, vector<int> &color, int &time, vector<int> &reach,  vector<int> &leave, vector<vector<int>> &edgeType) {
    cout << u + 1 << " ";
    color[u] = GRAY;
    time++;
    reach[u] = time;
    bool ok = false;
    for (int i = 0; i < neighList[u].neighbours.size(); ++i) {
        int v = neighList[u].neighbours[i];
        if (color[v] == WHITE) {
            edgeType[u][v] = TREE;
            edgeType[v][u] = TREE;
            ok = true;
        } else if(color[v] == GRAY){
            edgeType[u][v] = BACK;
            edgeType[v][u] = BACK;
        } else{
            if(reach[v] < reach[u]){
                edgeType[u][v] = CROSS;
                edgeType[v][u] = CROSS;
            } else{
                edgeType[u][v] = FORWARD;
                edgeType[v][u] = FORWARD;
            }
        }
        if(ok){
            dfsTraversal(neighList, v, color, time, reach, leave, edgeType);
        }
    }
    color[u] = BLACK;
    time++;
    leave[u] = time;
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