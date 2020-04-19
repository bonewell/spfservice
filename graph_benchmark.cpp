#include <benchmark/benchmark.h>

#include "graph.h"

static void BM_SPF(benchmark::State& state) {
  Graph g;
  g.insert(0, {{1, 7}, {2, 9}, {5, 14}});
  g.insert(1, {{0, 7}, {2, 10}, {3, 15}});
  g.insert(2, {{0, 9}, {1, 10}, {3, 11}, {5, 2}});
  g.insert(3, {{1, 15}, {2, 11}, {4, 6}});
  g.insert(4, {{3, 6}, {5, 9}});
  g.insert(5, {{0, 14}, {2, 2}, {4, 9}});

  for (auto _ : state) {
    g.path(0, 4);
  }
}

BENCHMARK(BM_SPF);
