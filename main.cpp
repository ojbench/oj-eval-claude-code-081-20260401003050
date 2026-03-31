#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int MAXN = 3005;

int n, m;
int degree[MAXN];

struct Graph {
    vector<vector<pair<int, int>>> adj;  // adj[u] = list of (v, edge_id)
    vector<int> cap;  // capacity of each edge
    int edge_count;

    Graph(int n) : adj(n + 1), cap(0), edge_count(0) {}

    void add_edge(int u, int v) {
        adj[u].push_back({v, edge_count});
        adj[v].push_back({u, edge_count});
        cap.push_back(1);
        edge_count++;
    }

    void reset() {
        fill(cap.begin(), cap.end(), 1);
    }
};

int compute_max_flow(Graph& g, int s, int t, int max_possible) {
    vector<int> parent(n + 1);
    vector<int> parent_edge(n + 1);
    vector<bool> visited(n + 1);
    int flow = 0;

    while (flow < max_possible) {
        fill(visited.begin(), visited.end(), false);
        fill(parent.begin(), parent.end(), -1);

        queue<int> q;
        q.push(s);
        visited[s] = true;

        bool found = false;
        while (!q.empty() && !found) {
            int u = q.front();
            q.pop();

            if (u == t) {
                found = true;
                break;
            }

            for (auto [v, eid] : g.adj[u]) {
                if (!visited[v] && g.cap[eid] > 0) {
                    visited[v] = true;
                    parent[v] = u;
                    parent_edge[v] = eid;
                    q.push(v);
                }
            }
        }

        if (!found) break;

        // Update flow
        int curr = t;
        while (parent[curr] != -1) {
            int eid = parent_edge[curr];
            g.cap[eid]--;
            curr = parent[curr];
        }
        flow++;
    }

    return flow;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;

    Graph g(n);

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        g.add_edge(a, b);
        degree[a]++;
        degree[b]++;
    }

    long long total = 0;
    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            if (degree[a] == 0 || degree[b] == 0) continue;

            g.reset();
            int max_possible = min(degree[a], degree[b]);
            int flow = compute_max_flow(g, a, b, max_possible);
            total += flow;
        }
    }

    cout << total << endl;

    return 0;
}
