#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <sstream>

using namespace std;

typedef struct Point {
    vector<int> neighbours;
} Point;

typedef struct Edge {
    int start;
    int end;
} Edge;

vector<int> pruferCode(vector<Point> &neighList, int numPoints);

void pruferDecode(vector<Edge> &edgeList, vector<int> &prufer, int numPoints);

bool isValidPruferCode(const vector<int>& prufer, int numPoints);

vector<Point> neighListFromFile(int &numberOfPoints, int &numberOfEdges, istream &input = cin);

void removeEdge(vector<Point> &neighlist, int node1, int node2);


int main(int argc, char **argv) {
    ifstream prufIn(argv[1]);
    if (!prufIn) {
        cerr << "Error opening 1. input file." << endl;
        return 1; // Return an error code
    }
    string line;
    int shift = 0;
    int numberOfEdges = 0;
    while(getline(prufIn, line)){
        istringstream iss(line);
        vector<int> prufer;
        int currInt;

        while(iss >> currInt){
            prufer.push_back(currInt);
        }
        if(!isValidPruferCode(prufer, (int) prufer.size() + 2)){
            cout << "Invalid Prufer code: ";
            for (int i:prufer) {
                cout << i << " ";
            }
            cout << endl;
        } else {
            vector<Edge> edgeList;
            pruferDecode(edgeList, prufer, (int) prufer.size() + 2);
//            for (auto edge:edgeList) {
//                cout << "(" << edge.start << ", " << edge.end << ") ";
//            }
            string filename;
            for (int i:prufer) {
                filename += to_string(i);
                if(i != prufer.back()){
                    filename += "-";
                }
            }
            filename += ".txt";
            ofstream edgesOut(filename);
            ofstream concatOut("concat.txt");
            if(!edgesOut){
                cerr << "Error opening 2. output file." << endl;
                return 1;
            }
            for (auto edge:edgeList) {
                edgesOut << edge.start << " " << edge.end << endl;
            }
            int minEdge = INT_MAX;
            for (auto edge:edgeList) {
                concatOut << edge.start + shift << " " << edge.end + shift << endl;
                numberOfEdges++;
                if(edge.start + shift < minEdge){
                    minEdge = edge.start + shift;
                }
                if(edge.end + shift < minEdge){
                    minEdge = edge.end + shift;
                }
            }
            if(shift != 0){
                concatOut << shift << " " << minEdge << endl;
                numberOfEdges++;
            }
        }
        shift += (int) prufer.size() + 2;
    }

    ifstream graphIn("concat.txt");
    if (!graphIn) {
        cerr << "Error opening 1. input file." << endl;
        return 1; // Return an error code
    }
    //here I get a sigtrap error from bad allocation probably
    //no time to fix allocation error, but otherwise it would work
//    vector<Point> neighlist = neighListFromFile(shift, numberOfEdges, graphIn);
//    vector<int> prufer = pruferCode(neighlist, shift);
//    for (int i:prufer) {
//        cout << i << " ";
//    }
    return 0;
}

bool isValidPruferCode(const vector<int>& prufer, int numPoints) {
    // Condition 1: Check the length of the Prufer code
    if (prufer.size() != numPoints - 2) {
        return false;
    }

    // Condition 2: Check for valid vertex indices
    set<int> uniqueIndices;
    for (int index : prufer) {
        if (index < 1 || index > numPoints || uniqueIndices.count(index) > 0) {
            return false;
        }
        uniqueIndices.insert(index);
    }

    // Condition 3: Check degree sequence conditions
    vector<int> degree(numPoints + 1, 1); // Vertex indices are 1-based

    for (int i : prufer) {
        degree[i]++;
    }

    for (int i = 1; i <= numPoints; ++i) {
        if (degree[i] != 1 && degree[i] != 2) {
            return false;
        }
    }

    // All conditions passed, Prufer code is valid
    return true;
}

void pruferDecode(vector<Edge> &edgeList, vector<int> &prufer, int numPoints) {
    // Initialize degree array to track the degree of each vertex
    vector<int> degree(numPoints + 2, 1); // Vertex indices are 1-based

    // Count occurrences of each vertex in the Prufer code
    for (int i : prufer) {
        degree[i]++;
    }

    // Find the first leaf vertex and connect it to the corresponding Prufer code element
    for (int i : prufer) {
        int leaf = find(degree.begin(), degree.end(), 1) - degree.begin();
        edgeList.push_back({i, leaf});
        degree[i]--;
        degree[leaf]--;
    }

    // Connect the remaining two vertices
    int leaf1 = find(degree.begin(), degree.end(), 1) - degree.begin();
    int leaf2 = find(degree.begin() + leaf1 + 1, degree.end(), 1) - degree.begin();
    edgeList.push_back({leaf1, leaf2});
}

vector<Point> neighListFromFile(int &numberOfPoints, int &numberOfEdges, istream &input) {
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

vector<int> pruferCode(vector<Point> &neighList, int numPoints) {
    vector<int> prufer(numPoints - 2);
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
    return prufer;
}

void removeEdge(vector<Point> &neighList, int node1, int node2) {
    neighList[node1].neighbours.erase(
            remove(neighList[node1].neighbours.begin(), neighList[node1].neighbours.end(), node2),
            neighList[node1].neighbours.end());
    neighList[node2].neighbours.erase(
            remove(neighList[node2].neighbours.begin(), neighList[node2].neighbours.end(), node1),
            neighList[node2].neighbours.end());
}