#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 3005;
const int INF = 1e9;

struct Edge {
    int to, cap, rev;
};

vector<Edge> graph[MAXN];
int level[MAXN];
int iter[MAXN];
int n, m;
int degree[MAXN];
int component[MAXN];

// Union-Find for connected components
int parent_uf[MAXN];
int find_uf(int x) {
    if (parent_uf[x] != x) parent_uf[x] = find_uf(parent_uf[x]);
    return parent_uf[x];
}
void union_uf(int x, int y) {
    x = find_uf(x);
    y = find_uf(y);
    if (x != y) parent_uf[x] = y;
}

void add_edge(int from, int to, int cap) {
    graph[from].push_back({to, cap, (int)graph[to].size()});
    graph[to].push_back({from, 0, (int)graph[from].size() - 1});
}

bool bfs(int s, int t) {
    static int queue[MAXN];
    int head = 0, tail = 0;

    memset(level, -1, (n + 1) * sizeof(int));
    level[s] = 0;
    queue[tail++] = s;

    while (head < tail) {
        int v = queue[head++];
        if (v == t) return true;  // Early termination

        for (auto& e : graph[v]) {
            if (level[e.to] < 0 && e.cap > 0) {
                level[e.to] = level[v] + 1;
                queue[tail++] = e.to;
            }
        }
    }

    return level[t] >= 0;
}

int dfs(int v, int t, int f) {
    if (v == t) return f;

    for (int& i = iter[v]; i < (int)graph[v].size(); i++) {
        Edge& e = graph[v][i];
        if (level[v] < level[e.to] && e.cap > 0) {
            int d = dfs(e.to, t, min(f, e.cap));
            if (d > 0) {
                e.cap -= d;
                graph[e.to][e.rev].cap += d;
                return d;
            }
        }
    }

    return 0;
}

int max_flow(int s, int t, int max_possible) {
    int flow = 0;
    while (flow < max_possible && bfs(s, t)) {
        memset(iter, 0, (n + 1) * sizeof(int));  // Only reset what we need
        int f;
        while (flow < max_possible && (f = dfs(s, t, INF)) > 0) {
            flow += f;
        }
    }
    return flow;
}

// Save and restore graph state
vector<Edge> saved_graph[MAXN];

void save_graph() {
    for (int i = 1; i <= n; i++) {
        saved_graph[i] = graph[i];
    }
}

void restore_graph() {
    for (int i = 1; i <= n; i++) {
        graph[i] = saved_graph[i];
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;

    // Initialize union-find
    for (int i = 1; i <= n; i++) parent_uf[i] = i;

    vector<pair<int, int>> edges;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        edges.push_back({a, b});
        union_uf(a, b);
        degree[a]++;
        degree[b]++;
    }

    // Build graph
    for (auto [a, b] : edges) {
        add_edge(a, b, 1);
        add_edge(b, a, 1);
    }

    save_graph();

    long long total = 0;
    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            // Skip if either node is isolated
            if (degree[a] == 0 || degree[b] == 0) continue;

            // Skip if nodes are in different components
            if (find_uf(a) != find_uf(b)) continue;

            restore_graph();
            // Max flow is limited by min of degrees
            int max_possible = min(degree[a], degree[b]);
            int flow = max_flow(a, b, max_possible);
            total += flow;
        }
    }

    cout << total << endl;

    return 0;
}
