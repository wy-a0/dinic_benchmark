#include "max_flow.h"
#include <chrono>
#include <random>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>

// ----------
static std::ofstream csv("results.csv", std::ios::trunc);

// ----------
using bench_clock = std::chrono::steady_clock;
using std::chrono::microseconds;
using std::chrono::duration_cast;

// ----------
static std::mt19937 rng([]{
    std::random_device rd;
    auto t = static_cast<uint64_t>(bench_clock::now().time_since_epoch().count());
    std::seed_seq seq{
        rd(), rd(), rd(), rd(),
        static_cast<unsigned>(t & 0xffffffffu),
        static_cast<unsigned>(t >> 32)
    };
    return std::mt19937(seq);
}());

// Random Graph Generator 
static void generate_random_graph(int V, int E, long long max_cap) {
    init_graph(V);

    std::uniform_int_distribution<int> distrib_v(0, V - 1);
    std::uniform_int_distribution<long long> distrib_c(1, max_cap);

    int added = 0;
    while (added < E) {
        int u = distrib_v(rng);
        int v = distrib_v(rng);
        if (u == v) continue; // self loop ignorieren
        add_edge(u, v, distrib_c(rng));
        ++added;
    }
}

// ---------
static double run_benchmark(long long (*algorithm)(int, int),
                            int s, int t, int num_runs) {
    double total_time_us = 0.0;

    for (int run = 0; run < num_runs; ++run) {
        const auto original_adj = adj; // außerhalb Messung

        auto start = bench_clock::now();
        algorithm(s, t);
        auto stop  = bench_clock::now();

        total_time_us += static_cast<double>(
            duration_cast<microseconds>(stop - start).count()
        );

        // Restore graph + helper arrays
        adj = original_adj;
        level.assign(static_cast<int>(adj.size()), -1);
        iter.assign(static_cast<int>(adj.size()), 0);
    }

    return (total_time_us / num_runs) / 1000.0; // ms
}

static void print_header() {
    std::cout << std::setw(10) << "V"
              << std::setw(10) << "E"
              << std::setw(15) << "Typ"
              << std::setw(12) << "Inst"
              << std::setw(15) << "EK (ms)"
              << std::setw(15) << "Dinic (ms)" << '\n';
    std::cout << std::string(77, '-') << '\n';

    if (csv) {
        csv << "V,E,Typ,Instance,EK_ms,Dinic_ms\n";
        csv.flush();
        std::cout << " CSV: results.csv wird geschrieben.\n";
    } else {
        std::cout << "  CSV konnte nicht geöffnet werden.\n";
    }
}

static void run_test_case(int V, int E, long long max_cap,
                          const std::string& type,
                          int num_instances, int num_runs) {
    const int s = 0, t = V - 1;

    for (int inst = 1; inst <= num_instances; ++inst) {
        generate_random_graph(V, E, max_cap);

        double time_ek    = run_benchmark(edmonds_karp, s, t, num_runs);
        double time_dinic = run_benchmark(dinic,       s, t, num_runs);

        // Konsole pro Instanz, wenn zu viel: auskommentieren
        std::cout << std::setw(10) << V
                  << std::setw(10) << E
                  << std::setw(15) << type
                  << std::setw(12) << inst
                  << std::setw(15) << std::fixed << std::setprecision(3) << time_ek
                  << std::setw(15) << std::fixed << std::setprecision(3) << time_dinic
                  << '\n';

        // CSV 
        if (csv) {
            csv << V << "," << E << "," << '"' << type << '"' << ","
                << inst << ","
                << std::fixed << std::setprecision(6) << time_ek << ","
                << std::fixed << std::setprecision(6) << time_dinic << "\n";
        }
    }

    if (csv) csv.flush();
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    print_header();

    const int num_runs      = 5;   // Wiederholungen pro Instanz
    const int num_instances = 20;  // Zufallsgraphen pro (V,E,Typ)

    for (int k = 1; k <= 20; ++k) {
        int V = 100 * k;
        int maxE = V * (V - 1);

        // Dünn: ~2V
        int E_thin = 2 * V;
        if (E_thin > maxE) E_thin = maxE;
        run_test_case(V, E_thin, 1000, "Dünn", num_instances, num_runs);

        // Dicht: ~50V
        int E_dense = 50 * V;
        if (E_dense > maxE) E_dense = maxE;
        run_test_case(V, E_dense, 1000, "Dicht", num_instances, num_runs);

        // Unit-Cap: ~5V, cap=1
        int E_unit = 5 * V;
        if (E_unit > maxE) E_unit = maxE;
        run_test_case(V, E_unit, 1, "Unit-Cap", num_instances, num_runs);
    }

    return 0;
}
