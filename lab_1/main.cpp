#include <iostream>
#include <vector>
#include <algorithm>
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
        cout << "Number of edges left: " << numberOfEdges << endl;
        cout << "---------------------------------" << endl << endl;
    }

    return 0;
}

//int main(int argc, char **argv) {
//    ifstream graphIn(argv[1]);
//    ifstream graphIn2(argv[1]);
//    if (!graphIn) {
//        cerr << "Error opening input file." << endl;
//        return 1; // Return an error code
//    }
//    int numberOfPoints, numberOfEdges;
//    vector<Point> neighList = neighListFromFile(numberOfPoints, numberOfEdges, graphIn);
//    printNeighList(neighList, numberOfPoints, numberOfEdges);
//
//    vector<Edge> edgeList = edgeListFromFile(numberOfPoints, numberOfEdges, graphIn2);
//    printEdgeList(edgeList);
//
//    while (numberOfEdges > 0) {
//        double max_betweenness;
//        for (int i = 0; i < neighList.size(); ++i) {
//            bfs(neighList, edgeList, i);
//            for (int j = 0; j < edgeList.size(); ++j) {
//                edgeList[j].final_betweenness += edgeList[j].betweenness / 2;
//            }
//        }
//
//        sort(edgeList.begin(), edgeList.end(), [](Edge a, Edge b) {
//            return a.final_betweenness > b.final_betweenness;
//        });
//
//        printEdgeList(edgeList);
//
//        max_betweenness = edgeList[0].final_betweenness;
//
//        while(edgeList[0].final_betweenness == max_betweenness and !edgeList.empty()) {
//            cout << "Cut edge: " << edgeList[0].start + 1 << " -- " << edgeList[0].end + 1 << endl;
//            removeEdgeFromEdgeList(edgeList, 0);
//            removeEdge(neighList, edgeList[0].start, edgeList[0].end);
//            --numberOfEdges;
//        }
//        cout << "---------------------------------" << endl;
//
//        for (auto &edge: edgeList) {
//            edge.betweenness = 0;
//            edge.final_betweenness = 0;
//        }
//        cout << "Highest betweenness centrality: " << max_betweenness << "\nNumber of edges left: " << numberOfEdges << endl << endl;
//    }
//
//    graphIn.close();
//    return 0;
//}
