#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef struct EDGE {
    int begin;
    int end;
    int weight;
} EDGE;

vector<EDGE> edgeListFromFile(int &numberPoints, int &numberEdges, istream &input);

void printEdgeList(vector<EDGE> edges, int numberOfEdges, ostream &output = cout);

int main(int argc, char **argv) {
    int numberPoints, numberEdges;
    ifstream input(argv[1]);
    if (!input) {
        cerr << "Error opening input file." << endl;
        return 1; // Return an error code
    }
    input >> numberPoints >> numberEdges;

    vector<EDGE> weightedEdgesList = edgeListFromFile(numberPoints, numberEdges, input);
    printEdgeList(weightedEdgesList, numberEdges, cout);
    input.close();

    std::sort(weightedEdgesList.begin(), weightedEdgesList.end(), [](const EDGE &a, const EDGE &b) {
        return a.weight < b.weight;
    });

    cout << "After sorting:\n";
    printEdgeList(weightedEdgesList, numberEdges, cout);

    int tree[numberPoints];
    for (int i = 0; i < numberPoints ; ++i) {
        tree[i] = i;
    }
    int minimalWeight = 0;
    for (int i = 0; i < numberEdges ; ++i) {
        int temp = tree[weightedEdgesList[i].begin];
        if(tree[weightedEdgesList[i].begin] != tree[weightedEdgesList[i].end]){
            minimalWeight += weightedEdgesList[i].weight;
            cout << weightedEdgesList[i].begin + 1 << " " << weightedEdgesList[i].end + 1 << endl;
            for (int j = 0; j < numberPoints; ++j) {
                if(tree[j] == temp){
                    tree[j] = tree[weightedEdgesList[i].end];
                }
            }
        }
    }
    cout << "Minimal sum weight accumulated: " << minimalWeight;
    return 0;
}

vector<EDGE> edgeListFromFile(int &numberPoints, int &numberEdges, istream &input) {
    vector<EDGE> toReturn;
    for (int i = 0; i < numberEdges; ++i) {
        EDGE edge;
        input >> edge.begin >> edge.end;
        input >> edge.weight;
        edge.begin--;
        edge.end--;
        toReturn.push_back(edge);
    }
    return toReturn;
}

void printEdgeList(vector<EDGE> edges, int numberOfEdges, ostream &output) {
    for (int i = 0; i < numberOfEdges; i++) {
        output << edges[i].begin + 1 << " - " << edges[i].end  + 1 << " (" << edges[i].weight << ")" << endl;
    }
    output << endl;
}