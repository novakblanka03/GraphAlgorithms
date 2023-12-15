#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

enum Color {
    WHITE, GRAY, BLACK
};

enum EdgeType {
    TREE, BACK, FORWARD, CROSS
};

typedef struct Point {
    vector<int> neighbours;
} Point;

void depthFirstSearch(vector<Point> &neighList, int numberOfPoints);

int dfsTraversal(vector<Point> &neighList, int u, vector<int> &color, vector<int> &level, vector<int> &dad, int l = 0);

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


    depthFirstSearch(neighList, numberOfPoints);
    return 0;
}

void depthFirstSearch(vector<Point> &neighList, int numberOfPoints) {
    vector<int> level(numberOfPoints, -1);
    vector<int> color(numberOfPoints, WHITE);
    vector<int> dad(numberOfPoints, 0);
    for (int i = 0; i < numberOfPoints; ++i) {
        if (color[i] == WHITE) {
            dfsTraversal(neighList, i, color, level, dad);
        }
    }
}

int dfsTraversal(vector<Point> &neighList, int u, vector<int> &color, vector<int> &level, vector<int> &dad, int l) {
    color[u] = GRAY;
    level[u] = l;
    int RfminVM = INT_MAX;
    int parted_sons = 0;
    int sons = 0;

    for (int i = 0; i < neighList[u].neighbours.size(); ++i) {
        int v = neighList[u].neighbours[i];
        if (color[v] == WHITE) {
            sons++;
            dad[v] = u;
            int UminVM = dfsTraversal(neighList, v, color, level, dad, l + 1);
            if (UminVM < l) {
                if(UminVM < RfminVM){
                    RfminVM = UminVM;
                }
            } else {
                parted_sons++;
            }
        } else if(color[v] == GRAY){
            if(level[v] < l -1 && level[v] < RfminVM){
                RfminVM = level[v];
            }
        }
    }
    color[u] = BLACK;
    if (dad[u] != 0) {
        if (parted_sons > 0) {
            cout << u + 1  << " - " << parted_sons << endl;
        }
    } else{
        if(sons > 1){
            cout << u + 1 << " -- " << sons << endl;
        }
    }
    return RfminVM;
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

