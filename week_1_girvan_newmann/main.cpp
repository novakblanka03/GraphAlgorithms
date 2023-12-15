#include <iostream>
#include <vector>
#include "girvanNewmannAlgorithm.h"

using namespace std;

int main(int argc, char **argv) {
    cout << "--- GIRVAN-NEWMANN ALGORITHM  ---" << endl;
    ifstream graphIn(argv[1]);
    if (!graphIn) {
        cerr << "Error opening input file." << endl;
        return 1; // Return an error code
    }
    int numberOfPoints, numberOfEdges;
    vector<Point> neighList = neighListFromFile(numberOfPoints, numberOfEdges, graphIn);
    //printNeighList(neighList, numberOfPoints, numberOfEdges);

    vector<vector<double>> flux(numberOfPoints, vector<double>(numberOfPoints, 0));
    vector<vector<double>> betweenness(numberOfPoints, vector<double>(numberOfPoints, 0));

    while (numberOfEdges > 0) {
        for (int k = 0; k < numberOfPoints; ++k) {
            bfs(neighList, k, flux);
            for (int i = 0; i < numberOfPoints; ++i) {
                for (int j = 0; j < numberOfPoints; ++j) {
                    betweenness[i][j] += flux[i][j] / 2;
                    flux[i][j] = 0;
                }
            }
        }
        double max_betweenness = getMaxBetweenness(betweenness);
        for (int i = 0; i < numberOfPoints; ++i) {
            for (int j = 0; j < numberOfPoints; ++j) {
                if (j > i) {
                    if (betweenness[i][j] == max_betweenness) {
                        cout << "Cut edge: " << i + 1 << " -- " << j + 1 << endl;
                        removeEdge(neighList, i, j);
                        numberOfEdges--;
                        if (numberOfEdges == 0) {
                            break;
                        }
                    }
                }

            }
        }

        for (int i = 0; i < numberOfPoints; ++i) {
            for (int j = 0; j < numberOfPoints; ++j) {
                betweenness[i][j] = 0;
            }
        }
        cout << "Highest betweenness: " << max_betweenness << endl;
        cout << "Number of edges left: " << numberOfEdges << endl << endl;
        cout << "---------------------------------" << endl << endl;
    }

    return 0;
}