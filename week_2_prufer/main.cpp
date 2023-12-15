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

void pruferCode(vector<Point> &neighList, vector<int> &prufer, int numPoints);

void pruferDecode(vector<Point> &neighList);

void removeEdge(vector<Point> &neighList, int node1, int node2);

int main(int argc, char **argv) {
    ifstream graphIn(argv[1]);
    if (!graphIn) {
        cerr << "Error opening input file." << endl;
        return 1; // Return an error code
    }
    int numberOfPoints, numberOfEdges;
    vector<Point> neighList = neighListFromFile(numberOfPoints, numberOfEdges, graphIn);
    printNeighList(neighList, numberOfPoints, numberOfEdges);

    vector<int> prufer;
    pruferCode(neighList, prufer, numberOfPoints);
    cout << "Prufer code: ";
    for (auto code:prufer) {
        cout << code + 1 << " ";
    }


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

void pruferCode(vector<Point> &neighList, vector<int> &prufer, int numPoints) {
    int i = 0;
    while(prufer.size() < numPoints - 2) {
        if(neighList[i].neighbours.size() == 1){
            int neighbour = neighList[i].neighbours[0];
            prufer.push_back(neighbour);
            removeEdge(neighList, i, neighbour);
            i = 0;
        } else{
            ++i;
        }
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

void pruferDecode(vector<Point> &neighList) {

}
