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

TEST(SPF, MarkAsVisited) {
  Graph g;
  g.vertexes = {{0, 0}, {1}};
  g.unvisitedIds = {0, 1};
  auto& a = g[0];

  g.mark(a);

  EXPECT_THAT(g.unvisitedIds.find(a.id), Eq(g.unvisitedIds.end()));
}

TEST(SPF, TheEnd) {
  Graph g;
  g.vertexes = {{0, 0}, {1}};

  EXPECT_THAT(g.isFinished(), Eq(true));
}

TEST(SPF, TheEndWithNoEdge) {
  Graph g;
  g.vertexes = {{0, 0}, {1}};
  g.unvisitedIds = {1};

  EXPECT_THAT(g.isFinished(), Eq(true));
}

TEST(SPF, NoFinished) {
  Graph g;
  g.vertexes = {{0, 0}, {1, 4}};
  g.unvisitedIds = {1};

  EXPECT_THAT(g.isFinished(), Eq(false));
}

TEST(SPF, NextUnvisited) {
  Graph g;
  g.vertexes = {{0, 0}, {1, 5}, {2, 3}};
  g.unvisitedIds = {1, 2};

  EXPECT_THAT(g.next().id, Eq(2));
}

TEST(SPF, Calculate) {
  Graph g;
  g.vertexes = {{0}, {1}, {2}, {3}, {4}, {5}};
  g[0].neighbors = {{1, 7}, {2, 9}, {5, 14}};
  g[1].neighbors = {{0, 7}, {2, 10}, {3, 15}};
  g[2].neighbors = {{0, 9}, {1, 10}, {3, 11}, {5, 2}};
  g[3].neighbors = {{1, 15}, {2, 11}, {4, 6}};
  g[4].neighbors = {{3, 6}, {5, 9}};
  g[5].neighbors = {{0, 14}, {2, 2}, {4, 9}};
  g.unvisitedIds = {0, 1, 2, 3, 4, 5};

  EXPECT_THAT(g.calculate(0, 4), Eq(20));

}
