#include <benchmark/benchmark.h>

#include "graph.h"

static void BM_SPF_CalculatePath(benchmark::State& state) {
  Graph g;
  g.addVertex(); g.addVertex(); g.addVertex(); g.addVertex();
  g.addVertex(); g.addVertex();
  g.setEdge(0, 1, 7); g.setEdge(0, 2, 9); g.setEdge(0, 5, 14);
  g.setEdge(1, 0, 7); g.setEdge(1, 2, 10); g.setEdge(1, 3, 15);
  g.setEdge(2, 0, 9); g.setEdge(2, 1, 10); g.setEdge(2, 5, 2);
  g.setEdge(3, 1, 15); g.setEdge(3, 2, 11); g.setEdge(3, 4, 6);
  g.setEdge(4, 3, 6); g.setEdge(4, 5, 9);
  g.setEdge(5, 0, 14); g.setEdge(5, 2, 2); g.setEdge(5, 4, 9);

  for (auto _ : state) {
    g.path(state.range(0), state.range(1));
  }
}

static void BM_SPF_GetPath(benchmark::State& state) {
  Graph g;
  g.addVertex(); g.addVertex(); g.addVertex(); g.addVertex();
  g.addVertex(); g.addVertex();
  g.setEdge(0, 1, 7); g.setEdge(0, 2, 9); g.setEdge(0, 5, 14);
  g.setEdge(1, 0, 7); g.setEdge(1, 2, 10); g.setEdge(1, 3, 15);
  g.setEdge(2, 0, 9); g.setEdge(2, 1, 10); g.setEdge(2, 5, 2);
  g.setEdge(3, 1, 15); g.setEdge(3, 2, 11); g.setEdge(3, 4, 6);
  g.setEdge(4, 3, 6); g.setEdge(4, 5, 9);
  g.setEdge(5, 0, 14); g.setEdge(5, 2, 2); g.setEdge(5, 4, 9);
  g.path(state.range(0), state.range(1));

  for (auto _ : state) {
    g.path(state.range(0), state.range(1));
  }
}

BENCHMARK(BM_SPF_CalculatePath)
    ->Iterations(1)
    ->Args({0, 5})
    ->Args({1, 4})
    ->Args({2, 3})
    ->Args({3, 2})
    ->Args({4, 1})
    ->Args({5, 0})
    ->Complexity();

BENCHMARK(BM_SPF_GetPath)
    ->Iterations(1)
    ->Args({0, 5})
    ->Args({1, 4})
    ->Args({2, 3})
    ->Args({3, 2})
    ->Args({4, 1})
    ->Args({5, 0})
    ->Complexity();
