#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "graph.h"

using ::testing::Eq;
using ::testing::ContainerEq;

class TestGraph : public Graph {
public:
  using Graph::operator[];
  using Graph::unvisited;
  using Graph::init;
  using Graph::updateNeighbors;
  using Graph::markAsVisited;
  using Graph::isFinished;
  using Graph::next;
  using Graph::calculate;
  using Graph::path;
};

TEST(SPF, ShorterPathFound) {
  Vertex a{0, {{1, 2}}, {6}};
  Vertex b{1, {}, {10}};

  b.setDistance(a);

  EXPECT_THAT(b.info.distance, Eq(8));
}

TEST(SPF, NoShorterPath) {
  Vertex a{0, {{1, 2}}, {6}};
  Vertex b{1, {}, {7}};

  b.setDistance(a);

  EXPECT_THAT(b.info.distance, Eq(7));
}

TEST(SPF, VertexIsVisited) {
  Vertex a{0, {{1, 2}}, {6}};
  Vertex b{1, {}, {10, true}};

  b.setDistance(a);

  EXPECT_THAT(b.info.distance, Eq(10));
}

TEST(SPF, UpdateNeighbors) {
  TestGraph g;
  g.insert(0, {{1, 3}, {2, 5}});
  g.insert(1);
  g.insert(2);
  g.init();
  g[0].info.distance = 6;

  g.updateNeighbors(g[0]);

  EXPECT_THAT(g[1].info.distance, Eq(9));
  EXPECT_THAT(g[2].info.distance, Eq(11));
}

TEST(SPF, SkipVisitedNeighbor) {
  TestGraph g;
  g.insert(0, {{1, 1}});
  g.insert(1);
  g.init();
  g[1].info.distance = 5;
  g[1].info.visited = true;

  g.updateNeighbors(g[0]);

  EXPECT_THAT(g[1].info.distance, Eq(5));
}

TEST(SPF, MarkAsVisited) {
  TestGraph g;
  g.insert(0);
  g.insert(1);

  g.markAsVisited(g[0]);

  EXPECT_THAT(g[0].info.visited, Eq(true));
  EXPECT_THAT(g.unvisited().find(g[0].id), Eq(g.unvisited().end()));
}

TEST(SPF, TheEndNoUnvisited) {
  TestGraph g;
  g.insert(0);
  g.insert(1);

  EXPECT_THAT(g.isFinished(), Eq(true));
}

TEST(SPF, TheEndWithNoEdge) {
  TestGraph g;
  g.insert(0);
  g.insert(1);
  g.init();
  g.markAsVisited(g[0]);

  EXPECT_THAT(g.isFinished(), Eq(true));
}

TEST(SPF, NoFinished) {
  TestGraph g;
  g.insert(0);
  g.insert(1);
  g.init();
  g.markAsVisited(g[0]);
  g[1].info.distance = 4;

  EXPECT_THAT(g.isFinished(), Eq(false));
}

TEST(SPF, NextUnvisited) {
  TestGraph g;
  g.insert(0);
  g.insert(1);
  g.insert(2);
  g.init();
  g.markAsVisited(g[0]);
  g[1].info.distance = 5;
  g[2].info.distance = 3;

  EXPECT_THAT(g.next().id, Eq(2));
}

TEST(SPF, Calculate) {
  TestGraph g;
  g.insert(0, {{1, 7}, {2, 9}, {5, 14}});
  g.insert(1, {{0, 7}, {2, 10}, {3, 15}});
  g.insert(2, {{0, 9}, {1, 10}, {3, 11}, {5, 2}});
  g.insert(3, {{1, 15}, {2, 11}, {4, 6}});
  g.insert(4, {{3, 6}, {5, 9}});
  g.insert(5, {{0, 14}, {2, 2}, {4, 9}});

  g.calculate(g[0]);

  EXPECT_THAT(g[4].info.distance, Eq(20));
}

TEST(SPF, GetPath) {
  TestGraph g;
  g.insert(0); g[0].info = {0, true};
  g.insert(1); g[1].info = {0, true, 0};
  g.insert(2); g[2].info = {0, true, 0};
  g.insert(3); g[3].info = {0, true, 2};
  g.insert(4); g[4].info = {0, true, 5};
  g.insert(5); g[5].info = {0, true, 2};

  EXPECT_THAT(g.path(g[4]), ContainerEq(std::list<Id>{0, 2, 5, 4}));
}

TEST(SPF, CalculateAndGetPath) {
  Graph g;
  g.insert(0, {{1, 7}, {2, 9}, {5, 14}});
  g.insert(1, {{0, 7}, {2, 10}, {3, 15}});
  g.insert(2, {{0, 9}, {1, 10}, {3, 11}, {5, 2}});
  g.insert(3, {{1, 15}, {2, 11}, {4, 6}});
  g.insert(4, {{3, 6}, {5, 9}});
  g.insert(5, {{0, 14}, {2, 2}, {4, 9}});

  EXPECT_THAT(g.path(0, 4), ContainerEq(std::list<Id>{0, 2, 5, 4}));
}

TEST(SPF, GetAlreadyCalculatedPath) {
  Graph g;
  g.insert(0, {{1, 7}, {2, 9}, {5, 14}});
  g.insert(1, {{0, 7}, {2, 10}, {3, 15}});
  g.insert(2, {{0, 9}, {1, 10}, {3, 11}, {5, 2}});
  g.insert(3, {{1, 15}, {2, 11}, {4, 6}});
  g.insert(4, {{3, 6}, {5, 9}});
  g.insert(5, {{0, 14}, {2, 2}, {4, 9}});
  g.path(0, 5);

  EXPECT_THAT(g.path(0, 4), ContainerEq(std::list<Id>{0, 2, 5, 4}));
}
