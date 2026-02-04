#include "max_flow.h"
#include <queue>
#include <utility>   // std::pair
#include <algorithm> // std::min
#include <limits>

// Globale Strukturen (Definitionen)
std::vector<std::vector<Edge>> adj;
std::vector<int> level;
std::vector<int> iter;

void init_graph(int n) {
    adj.assign(n, std::vector<Edge>());
    level.assign(n, -1);
    iter.assign(n, 0);
}

void add_edge(int u, int v, long long cap) {
    // Vorwärtskante: u -> v
    Edge forward{v, static_cast<int>(adj[v].size()), cap};
    // Rückkante: v -> u ,Kapazität 0
    Edge backward{u, static_cast<int>(adj[u].size()), 0};

    adj[u].push_back(forward);
    adj[v].push_back(backward);
}

// Edmonds–Karp Algorithmus, BFS-basierte Ford–Fulkerson-Variante
long long edmonds_karp(int s, int t) {
    long long flow = 0;
    const long long INF = std::numeric_limits<long long>::max();

    while (true) {
        // 1) BFS, suche augmentierenden Pfad und merke Vorgänger + Kantenindex
        std::vector<int> parent(adj.size(), -1);
        std::vector<int> edge_index(adj.size(), -1);
        std::queue<std::pair<int, long long>> q;

        q.push({s, INF});
        parent[s] = s;

        long long path_flow = 0;

        while (!q.empty()) {
            int u = q.front().first;
            long long current_flow = q.front().second;
            q.pop();

            if (u == t) {
                path_flow = current_flow;
                break;
            }

            for (std::size_t i = 0; i < adj[u].size(); ++i) {
                Edge &e = adj[u][i];
                if (parent[e.to] == -1 && e.cap > 0) {
                    parent[e.to] = u;
                    edge_index[e.to] = static_cast<int>(i);
                    long long new_flow = std::min(current_flow, e.cap);
                    q.push({e.to, new_flow});
                }
            }
        }

        if (path_flow == 0) {
            // kein augmentierender Pfad mehr
            break;
        }

        // 2) Pfad augmentieren: Restkapazitäten anpassen, vorwärts / rückwärts
        flow += path_flow;
        int v = t;
        while (v != s) {
            int u = parent[v];
            int i = edge_index[v];

            // Vorwärtskante reduzieren
            adj[u][i].cap -= path_flow;

            // Rückkante erhöhen
            int rev_i = adj[u][i].rev;
            adj[v][rev_i].cap += path_flow;

            v = u;
        }
    }

    return flow;
}

