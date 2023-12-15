#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <set>
#include <climits>

using namespace std;

class WeightedGraph {
public:
    int numOfNodes;
    vector<vector<double>> adjacencyMatrix;

    WeightedGraph(int nodes) : numOfNodes(nodes), adjacencyMatrix(nodes + 1, std::vector<double>(nodes + 1, 0)) {}

    void addEdge(int from, int to, double weight) {
        adjacencyMatrix[from][to] = weight;
        adjacencyMatrix[to][from] = weight;
    }
};

WeightedGraph readGraphFromFile(const string &filename);

vector<double> dijkstraAlgorithm(const WeightedGraph &graph, int startingNode);

int main(int argc, char **argv) {
    string filename = argv[1];

    WeightedGraph graph = readGraphFromFile(filename);

    int start = 1;
    vector<double> distance = dijkstraAlgorithm(graph, start);

    // Outputting the distances
    cout << "Shortest paths:" << endl;
    for (int i = 1; i <= graph.numOfNodes; ++i) {
        cout << start << " - " << i << ": " << distance[i] << std::endl;
    }

    return 0;
}

WeightedGraph readGraphFromFile(const string &filename) {
    ifstream inputFile(filename);

    if (!inputFile) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    int numOfNodes, numOfEdges;
    inputFile >> numOfNodes >> numOfEdges;

    WeightedGraph graph(numOfNodes);

    for (int i = 0; i < numOfEdges; ++i) {
        int from, to;
        double weight;
        inputFile >> from >> to >> weight;
        graph.addEdge(from, to, weight/10);
    }

    inputFile.close();
    return graph;
}

vector<double> dijkstraAlgorithm(const WeightedGraph &graph, int startingNode) {
    vector<double> distance(graph.numOfNodes + 1, INT_MAX);
    vector<bool> checked(graph.numOfNodes + 1, false);

    // Use a priority queue to get the minimum distance node efficiently
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push({0, startingNode});
    distance[startingNode] = 0;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (checked[u]) {
            continue;
        }

        checked[u] = true;

        for (int v = 1; v <= graph.numOfNodes; ++v) {
            if (!checked[v] && graph.adjacencyMatrix[u][v] > 0 &&
                distance[u] + graph.adjacencyMatrix[u][v] < distance[v]) {
                distance[v] = distance[u] + graph.adjacencyMatrix[u][v];
                pq.push({distance[v], v});
            }
        }
    }

    return distance;
}

