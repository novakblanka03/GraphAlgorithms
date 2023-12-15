#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <cfloat>

using namespace std;

void dijkstra(vector<vector<pair<int, double>>> &graph, int start, vector<vector<double>> &distMatrix);

void dijkstra2(vector<vector<pair<int, double>>> &graph, int start);


int main() {
    ifstream input("input.txt");
    if(!input){
        cout << "Error opening input file." << endl;
        return 1;
    }
    int numberOfPoints, numberOfEdges;
    input >> numberOfPoints >> numberOfEdges;

    vector<vector<pair<int, double>>> graph(numberOfPoints);

    vector<vector<double>> distMatrix(numberOfPoints, vector<double>(numberOfPoints, DBL_MAX));

    for (int i = 0; i < numberOfEdges; ++i) {
        int start, end;
        double weight;
        input >> start >> end >> weight;
        graph[--start].emplace_back(--end, weight);
        graph[end].emplace_back(start, weight);
    }

    for (int i = 0; i < numberOfPoints; ++i) {
        dijkstra(graph, i, distMatrix);
    }

    int center = -1;
    double minWeight = DBL_MAX, sum = 0;
    for (int i = 0; i < numberOfPoints; ++i) {
        sum = 0;
        for (int j = 0; j < numberOfPoints; ++j) {
            sum += distMatrix[i][j];
        }
        //cout << sum << " " << endl;
        if (sum < minWeight) {
            minWeight = sum;
            center = i;
        }
    }

    cout << "The center is " << center + 1 << " with a total weight of " << minWeight;
    dijkstra2(graph, center);
    return 0;
}

void dijkstra(vector<vector<pair<int, double>>> &graph, int start, vector<vector<double>> &distMatrix) {
    int numberOfPoints = (int) graph.size();
    vector<double> dist(numberOfPoints, DBL_MAX);
    vector<int> parent(numberOfPoints, -1);
    vector<bool> visited(numberOfPoints, false);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    dist[start] = 0;
    pq.emplace(0, start);
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (visited[u]) {
            continue;
        }
        visited[u] = true;
        for (const auto &neigh: graph[u]) {
            int v = neigh.first;
            double weight = neigh.second;
            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    // Store the results in the matrix
    for (int i = 0; i < numberOfPoints; ++i) {
        distMatrix[start][i] = dist[i];
    }
}

void dijkstra2(vector<vector<pair<int, double>>> &graph, int start) {
    ofstream out("network.txt");
    int numberOfPoints = (int) graph.size();
    vector<double> dist(numberOfPoints, DBL_MAX);
    vector<int> parent(numberOfPoints, -1);
    vector<bool> visited(numberOfPoints, false);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    dist[start] = 0;
    pq.emplace(0, start);
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (visited[u]) {
            continue;
        }
        visited[u] = true;
        for (const auto &neigh: graph[u]) {
            int v = neigh.first;
            double weight = neigh.second;
            if (!visited[v] && dist[u] + weight < dist[v]) {
                out << u + 1 << " " << v + 1 << " " << weight << endl;
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}