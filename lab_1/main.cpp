#include <iostream>
#include <vector>
#include "girvanNewmannAlgorithm.h"

using namespace std;

int main(int argc, char **argv) {
    ifstream graphIn(argv[1]);
    if (!graphIn) {
        cerr << "Error opening input file." << endl;
        return 1; // Return an error code
    }
    int numberOfPoints, numberOfEdges;
    vector<Point> neighList = neighListFromFile(numberOfPoints, numberOfEdges, graphIn);
    printNeighList(neighList, numberOfPoints, numberOfEdges);

    auto **betweenness= new double *[neighList.size()];
    auto **final_betweenness = new double *[neighList.size()];
    for (int i = 0; i < neighList.size(); ++i) {
        betweenness[i] = new double[neighList.size()];
    }
    for (int i = 0; i < neighList.size(); ++i) {
        for (int j = 0; j < neighList.size(); ++j) {
            betweenness[i][j] = 1;
        }
    }

    for (int i = 0; i < neighList.size(); ++i) {
        final_betweenness[i] = new double[neighList.size()];
    }
    for (int i = 0; i < neighList.size(); ++i) {
        for (int j = 0; j < neighList.size(); ++j) {
            final_betweenness[i][j] = 0;
        }
    }

    while (numberOfEdges != 0) {
        double max_betweenness = 0;
        for (int i = 0; i < neighList.size(); ++i) {
            nrOfShortestPathsFromFirst(neighList, betweenness, i + 1);
            for (int j = 0; j < neighList.size(); ++j) {
                for (int k = 0; k < neighList.size(); ++k) {
                    final_betweenness[j][k] += betweenness[j][k] / 2;
                    betweenness[j][k] = 0;
                }
            }
        }
        int maxEdgeBegin, maxEdgeEnd;
        max_betweenness = determineMaxBetweenness(final_betweenness, (int) neighList.size(), maxEdgeBegin, maxEdgeEnd);
        removeEdge(neighList, maxEdgeBegin, maxEdgeEnd);
        for (int i = 0; i < neighList.size(); ++i) {
            for (int k = 0; k < neighList.size(); ++k) {
                final_betweenness[i][k] = 0;
            }
        }
        cout << "Highest betweenness centrality: " << max_betweenness << "\nNumber of edges left: " << --numberOfEdges << endl << endl;
    }

    //printNeighList(neighList, numberOfPoints, numberOfEdges);

    for (int i = 0; i < neighList.size(); ++i) {
        delete[] betweenness[i];
    }
    delete[] betweenness;

    for (int i = 0; i < neighList.size(); ++i) {
        delete[] final_betweenness[i];
    }
    delete[] final_betweenness;

    graphIn.close();
    return 0;
}

