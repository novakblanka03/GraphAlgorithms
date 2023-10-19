#include <iostream>
#include <vector>
#include <algorithm>
#include "girvanNewmannAlgorithm.h"

void printEdgeList(vector<Edge> vector1, int points, int edges);

using namespace std;

int main(int argc, char **argv) {
    ifstream graphIn(argv[1]);
    ifstream graphIn2(argv[1]);
    if (!graphIn) {
        cerr << "Error opening input file." << endl;
        return 1; // Return an error code
    }
    int numberOfPoints, numberOfEdges;
    vector<Point> neighList = neighListFromFile(numberOfPoints, numberOfEdges, graphIn);
    printNeighList(neighList, numberOfPoints, numberOfEdges);

    vector<Edge> edgeList = edgeListFromFile(numberOfPoints, numberOfEdges, graphIn2);
    printEdgeList(edgeList);

    while (numberOfEdges > 0) {
        double max_betweenness;
        for (int i = 0; i < neighList.size(); ++i) {
            nrOfShortestPathsFromFirst(neighList, edgeList, i);
            for (int j = 0; j < edgeList.size(); ++j) {
                edgeList[j].final_betweenness += edgeList[j].betweenness / 2;
            }
        }
        printEdgeList(edgeList);
        //max_betweenness = determineMaxBetweenness(edgeList);

        sort(edgeList.begin(), edgeList.end(), [](Edge a, Edge b) {
            return a.final_betweenness > b.final_betweenness;
        });

        max_betweenness = edgeList[0].final_betweenness;

        while(edgeList[0].final_betweenness == max_betweenness and !edgeList.empty()) {
            cout << "Cut edge: " << edgeList[0].start + 1 << " -- " << edgeList[0].end + 1 << endl;
            removeEdgeFromEdgeList(edgeList, 0);
            removeEdge(neighList, edgeList[0].start, edgeList[0].end);
            --numberOfEdges;
        }
//        for (int i = 0; i < edgeList.size(); ++i) {
//            if (edgeList[i].final_betweenness == max_betweenness) {
//                cout << "Cut edge: " << edgeList[i].start + 1 << " -- " << edgeList[i].end + 1 << endl;
//                removeEdgeFromEdgeList(edgeList, i);
//                removeEdge(neighList, edgeList[i].start, edgeList[i].end);
//                --numberOfEdges;
//            }
//        }
        for (auto &edge: edgeList) {
            edge.betweenness = 1;
            edge.final_betweenness = 0;
        }
        cout << "Highest betweenness centrality: " << max_betweenness << "\nNumber of edges left: " << numberOfEdges << endl << endl;
    }

    //printNeighList(neighList, numberOfPoints, numberOfEdges);


    graphIn.close();
    return 0;
}



