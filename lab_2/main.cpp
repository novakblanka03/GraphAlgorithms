#include <iostream>
#include <fstream>
#include <vector>

///Prüfer-kód

using namespace std;

typedef struct Point {
    vector<int> neighbours;
} Point;

vector<Point> neighListFromFile(int &numberOfPoints, int &numberOfEdges, istream &input = cin);

void printNeighList(vector<Point> neighList, int &numberOfPoints, int &numberOfEdges, ostream &output = cout);

int main(int argc, char **argv) {
    ifstream graphIn(argv[2]);
    if (!graphIn) {
        cerr << "Error opening input file." << endl;
        return 1; // Return an error code
    }
    int numberOfPoints, numberOfEdges;
    vector<Point> neighList = neighListFromFile(numberOfPoints, numberOfEdges, graphIn);
    printNeighList(neighList, numberOfPoints, numberOfEdges);



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