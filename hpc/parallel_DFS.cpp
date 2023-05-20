#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

const int MAXN = 10000; // maximum number of nodes

vector<int> adjList[MAXN];
bool visited[MAXN];

void parallelDFS(int source) {
    stack<int> st;
    st.push(source);
    visited[source] = true;

    while (!st.empty()) {
        int node;
        #pragma omp critical
        {
            node = st.top();
            st.pop();
        }

        // Process the node here
        cout << "Visiting node: " << node << endl;

        #pragma omp parallel for
        for (int i = 0; i < adjList[node].size(); i++) {
            int neighbor = adjList[node][i];
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                #pragma omp critical
                {
                    st.push(neighbor);
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

    // initialize visited array
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    int source = 0; // set the source node for DFS

    // Perform parallel DFS
    parallelDFS(source);

    return 0;
}

