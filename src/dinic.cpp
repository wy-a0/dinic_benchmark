#include "max_flow.h"


static bool bfs_dinic(int s, int t);
static long long dfs_dinic(int v, int t, long long f);

// Dinic's Algorithmus: wiederhole BFS (Level-Graph) + DFS (Blocking-Flow)
long long dinic(int s, int t) {
    long long flow = 0;
    const long long INF = std::numeric_limits<long long>::max();

    while (bfs_dinic(s, t)) {
        // current-arc zeiger für jede phase zurücksetzen
        std::fill(iter.begin(), iter.end(), 0);

        long long pushed;
        while ((pushed = dfs_dinic(s, t, INF)) > 0) {
            flow += pushed;
        }
    }
    return flow;
}

// BFS, baut den Level Graphen im Restgraphen
static bool bfs_dinic(int s, int t) {
    std::fill(level.begin(), level.end(), -1);
    std::queue<int> q;
    level[s] = 0;
    q.push(s);

    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (const Edge &e : adj[v]) {
            if (e.cap > 0 && level[e.to] == -1) {
                level[e.to] = level[v] + 1;
                q.push(e.to);
            }
        }
    }
    return level[t] != -1; // ist t erreichbar
}

// DFS, findet Blocking-Flow entlang des Level-Graphen mit current-arc
static long long dfs_dinic(int v, int t, long long f) {
    if (v == t || f == 0) return f;

    for (int &i = iter[v]; i < (int)adj[v].size(); ++i) {
        Edge &e = adj[v][i];

        // nur Kanten im Level Graphen, genau eine Stufe tiefer
        if (e.cap > 0 && level[e.to] == level[v] + 1) {
            long long pushed = dfs_dinic(e.to, t, std::min(f, e.cap));
            if (pushed > 0) {
                e.cap -= pushed;                       // Vorwärtskante
                adj[e.to][e.rev].cap += pushed;        // Rückkante
                return pushed;
            }
        }
    }
    return 0;
}
