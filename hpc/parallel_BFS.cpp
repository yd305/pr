#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

const int MAXN = 10000; // maximum number of nodes

vector<int> adjList[MAXN];
int distance[MAXN];

void parallelBFS(int source) {
    queue<int> q;
    q.push(source);
    ::distance[source] = 0;

    while (!q.empty()) {
        int queueSize = q.size();

        #pragma omp parallel for
        for (int i = 0; i < queueSize; i++) {
            int node;
            #pragma omp critical
            {
                node = q.front();
                q.pop();
            }

            // Process neighbors of the current node
            for (int j = 0; j < adjList[node].size(); j++) {
                int neighbor = adjList[node][j];
                if (::distance[neighbor] == -1) {
                    ::distance[neighbor] = ::distance[node] + 1;
                    #pragma omp critical
                    {
                        q.push(neighbor);
                    }
                }
            }
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    // read in the graph
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    // initialize distances to -1 (unvisited)
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        ::distance[i] = -1;
    }

    int source = 0; // set the source node for BFS

    // Perform parallel BFS
    parallelBFS(source);

    // print out the distances
    for (int i = 0; i < n; i++) {
        cout << "Distance from " << source << " to " << i << " is " << ::distance[i] << endl;
    }

    return 0;
}

