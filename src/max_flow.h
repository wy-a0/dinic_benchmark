#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

/**
 * @brief Restgraph-Kante
 *
 * to   : Zielknoten
 * rev  : Index der Rückkante in adj[to]
 * cap  : aktuelle Restkapazität der Vorwärtskante
 */
struct Edge {
    int to;
    int rev;
    long long cap;
};


extern std::vector<std::vector<Edge>> adj;  // Adjazenzliste des Restgraphen
extern std::vector<int> level;              // Level-Graph BFS für Dinic
extern std::vector<int> iter;               // current-arc Zeiger DFS für Dinic

/**
 * @brief Graph-Strukturen auf n Knoten initialisieren.
 *        Setzt adj, level, iter zurück.
 */
void init_graph(int n);

/**
 * @brief Fügt eine gerichtete Kante u->v mit Kapazität cap hinzu
 *        und erzeugt automatisch die Rückkante v->u mit Kapazität 0.
 *
 * @param u   Startknoten
 * @param v   Zielknoten
 * @param cap Kapazität der Vorwärtskante
 */
void add_edge(int u, int v, long long cap);

/**
 * @brief Edmonds–Karp Algorithmus (BFS-basierte Ford–Fulkerson-Variante)
 * @param s Quelle
 * @param t Senke
 * @return berechneter Max-Flow-Wert
 */
long long edmonds_karp(int s, int t);

/**
 * @brief Dinic-Algorithmus (BFS Level-Graph + DFS Blocking-Flow)
 * @param s Quelle
 * @param t Senke
 * @return berechneter Max-Flow-Wert
 */
long long dinic(int s, int t);

#endif // MAX_FLOW_H
