#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "graph.h"

using ::testing::Eq;

TEST(SPF, ShorterPathFound) {
  Vertex a{0, 6};
  Vertex b{1, 10};
  a.neighbors[b.id] = 2;

  b.update(a);

  EXPECT_THAT(b.distance, Eq(8));
}

TEST(SPF, NoShorterPath) {
  Vertex a{0, 6};
  Vertex b{1, 7};
  a.neighbors[b.id] = 2;

  b.update(a);

  EXPECT_THAT(b.distance, Eq(7));
}

TEST(SPF, UpdateNeighbors) {
  Graph g;
  g.vertexes = {{0, 6}, {1}, {2}};
  g.unvisitedIds = {1, 2};
  auto& a = g[0];
  auto& b = g[1];
  auto& c = g[2];
  a.neighbors[b.id] = 3;
  a.neighbors[c.id] = 5;

  g.update(a);

  EXPECT_THAT(b.distance, Eq(9));
  EXPECT_THAT(c.distance, Eq(11));
}

TEST(SPF, SkipVisitedNeighbor) {
  Graph g;
  g.vertexes = {{0, 2}, {1, 5}};
  auto& a = g[0];
  auto& b = g[1];
  a.neighbors[b.id] = 1;

  g.update(a);

  EXPECT_THAT(b.distance, Eq(5));
}
