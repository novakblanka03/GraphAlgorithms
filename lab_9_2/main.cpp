#include <iostream>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

typedef struct Point {
    vector<int> neighbours;
} Point;

enum Color {
    RED, GREEN
};

void bfs(vector<Point> neighList, int start, vector<int> &level);

vector<Point> neighListFromFile(int &numberOfPoints, int &numberOfEdges, istream &input);

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
    vector<int> level(neighList.size(), 0);
    bfs(neighList, 0, level);

    for (int i = 0; i < level.size(); ++i) {
        cout << i + 1 << " " << level[i] << endl;
    }
    return 0;
}

void bfs(vector<Point> neighList, int start, vector<int> &level) {
    vector<int> nrShortest(neighList.size(), 0);
    vector<int> visited(neighList.size(), 0);
    queue<int> bfsQueue;
    visited[start] = 1;
    level[start] = 0;
    nrShortest[start] = 1;
    bfsQueue.push(start);
    while (!bfsQueue.empty()) {
        int startPoint = bfsQueue.front();
        for (int i = 0; i < neighList[startPoint].neighbours.size(); ++i) {
            int neighbour = neighList[startPoint].neighbours[i];
            if (visited[neighbour] == 0) {
                visited[neighbour] = 1;
                level[neighbour] = level[startPoint] + 1;
                bfsQueue.push(neighbour);
            }
        }
        bfsQueue.pop();
    }
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

