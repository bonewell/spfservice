#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "graph.h"

using ::testing::Eq;
using ::testing::ContainerEq;

class TestGraph : public Graph {
public:
  using Graph::operator[];
  using Graph::hasUnvisited;
  using Graph::init;
  using Graph::setSource;
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

TEST(SPF, AddVertex) {
  Graph g;

  EXPECT_THAT(g.addVertex(), Eq(0));
}

TEST(SPF, AddSecondVertex) {
  Graph g;
  g.addVertex();

  EXPECT_THAT(g.addVertex(), Eq(1));
}

TEST(SPF, SetNewEdge) {
  TestGraph g;
  g.addVertex(); g.addVertex();

  g.setEdge(0, 1, 2.3);

  EXPECT_THAT(g[0].neighbors[1], Eq(2.3));
}

TEST(SPF, UpdateEdge) {
  TestGraph g;
  g.addVertex(); g.addVertex();
  g.setEdge(0, 1, 2.3);

  g.setEdge(0, 1, 3.4);
  EXPECT_THAT(g[0].neighbors[1], Eq(3.4));
}

TEST(SPF, SetEdgeWithWrongFrom) {
  Graph g;

  EXPECT_THROW(g.setEdge(0, 1, 1.1), std::invalid_argument);
}

TEST(SPF, SetEdgeWithWrongTo) {
  Graph g;
  g.addVertex();

  EXPECT_THROW(g.setEdge(0, 1, 1.1), std::invalid_argument);
}

TEST(SPF, UpdateNeighbors) {
  TestGraph g;
  g.addVertex(); g.addVertex(); g.addVertex();
  g.setEdge(0, 1, 3); g.setEdge(0, 2, 5);
  g.init();
  g.setSource(g[0]);

  g.updateNeighbors(g[0]);

  EXPECT_THAT(g[1].info.distance, Eq(3));
  EXPECT_THAT(g.hasUnvisited(1), Eq(true));
  EXPECT_THAT(g[2].info.distance, Eq(5));
  EXPECT_THAT(g.hasUnvisited(2), Eq(true));
}

TEST(SPF, SkipVisitedNeighbor) {
  TestGraph g;
  g.addVertex(); g.addVertex();
  g.setEdge(0, 1, 1);
  g.init();
  g[1].info.distance = 5;
  g[1].info.visited = true;

  g.updateNeighbors(g[0]);

  EXPECT_THAT(g[1].info.distance, Eq(5));
}

TEST(SPF, MarkAsVisited) {
  TestGraph g;
  g.addVertex(); g.addVertex();

  g.markAsVisited(g[0]);

  EXPECT_THAT(g[0].info.visited, Eq(true));
  EXPECT_THAT(g.hasUnvisited(0), Eq(false));
}

TEST(SPF, TheEndNoUnvisited) {
  TestGraph g;
  g.addVertex(); g.addVertex();

  EXPECT_THAT(g.isFinished(), Eq(true));
}

TEST(SPF, TheEndWithNoEdge) {
  TestGraph g;
  g.addVertex(); g.addVertex();
  g.init();
  g.markAsVisited(g[0]);

  EXPECT_THAT(g.isFinished(), Eq(true));
}

TEST(SPF, NoFinished) {
  TestGraph g;
  g.addVertex(); g.addVertex();
  g.init();
  g.markAsVisited(g[0]);
  g[1].info.distance = 4;

  EXPECT_THAT(g.isFinished(), Eq(false));
}

TEST(SPF, FirstUnvisited) {
  TestGraph g;
  g.addVertex(); g.addVertex(); g.addVertex();
  g.init();
  g.setSource(g[0]);

  EXPECT_THAT(g.next().id, Eq(0));
}

TEST(SPF, NextUnvisited) {
  TestGraph g;
  g.addVertex(); g.addVertex(); g.addVertex();
  g.init();
  g.markAsVisited(g[0]);

  EXPECT_THAT(g.next().id, Eq(1));
}

TEST(SPF, SetSource) {
  TestGraph g;
  g.addVertex(); g.addVertex();
  g.init();

  g.setSource(g[0]);

  EXPECT_THAT(g[0].info.distance, Eq(0));
  EXPECT_THAT(g.hasUnvisited(0), Eq(true));
}

TEST(SPF, Calculate) {
  TestGraph g;
  g.addVertex(); g.addVertex(); g.addVertex(); g.addVertex();
  g.addVertex(); g.addVertex();
  g.setEdge(0, 1, 7); g.setEdge(0, 2, 9); g.setEdge(0, 5, 14);
  g.setEdge(1, 0, 7); g.setEdge(1, 2, 10); g.setEdge(1, 3, 15);
  g.setEdge(2, 0, 9); g.setEdge(2, 1, 10); g.setEdge(2, 5, 2);
  g.setEdge(3, 1, 15); g.setEdge(3, 2, 11); g.setEdge(3, 4, 6);
  g.setEdge(4, 3, 6); g.setEdge(4, 5, 9);
  g.setEdge(5, 0, 14); g.setEdge(5, 2, 2); g.setEdge(5, 4, 9);

  g.calculate(g[0]);

  EXPECT_THAT(g[4].info.distance, Eq(20));
}

TEST(SPF, GetPath) {
  TestGraph g;
  g.addVertex(); g.addVertex(); g.addVertex(); g.addVertex();
  g.addVertex(); g.addVertex();
  g[0].info = {0, true};
  g[1].info = {0, true, 0};
  g[2].info = {0, true, 0};
  g[3].info = {0, true, 2};
  g[4].info = {0, true, 5};
  g[5].info = {0, true, 2};

  EXPECT_THAT(g.path(g[4]), ContainerEq(std::list<Id>{0, 2, 5, 4}));
}

TEST(SPF, GetPathWithWrongFrom) {
  Graph g;

  EXPECT_THROW(g.path(0, 1), std::invalid_argument);
}

TEST(SPF, GetPathWithWrongTo) {
  Graph g;
  g.addVertex();

  EXPECT_THROW(g.path(0, 1), std::invalid_argument);
}

TEST(SPF, CalculateAndGetPath) {
  Graph g;
  g.addVertex(); g.addVertex(); g.addVertex(); g.addVertex();
  g.addVertex(); g.addVertex();
  g.setEdge(0, 1, 7); g.setEdge(0, 2, 9); g.setEdge(0, 5, 14);
  g.setEdge(1, 0, 7); g.setEdge(1, 2, 10); g.setEdge(1, 3, 15);
  g.setEdge(2, 0, 9); g.setEdge(2, 1, 10); g.setEdge(2, 5, 2);
  g.setEdge(3, 1, 15); g.setEdge(3, 2, 11); g.setEdge(3, 4, 6);
  g.setEdge(4, 3, 6); g.setEdge(4, 5, 9);
  g.setEdge(5, 0, 14); g.setEdge(5, 2, 2); g.setEdge(5, 4, 9);

  EXPECT_THAT(g.path(0, 4), ContainerEq(std::list<Id>{0, 2, 5, 4}));
}

TEST(SPF, GetAlreadyCalculatedPath) {
  Graph g;
  g.addVertex(); g.addVertex(); g.addVertex(); g.addVertex();
  g.addVertex(); g.addVertex();
  g.setEdge(0, 1, 7); g.setEdge(0, 2, 9); g.setEdge(0, 5, 14);
  g.setEdge(1, 0, 7); g.setEdge(1, 2, 10); g.setEdge(1, 3, 15);
  g.setEdge(2, 0, 9); g.setEdge(2, 1, 10); g.setEdge(2, 5, 2);
  g.setEdge(3, 1, 15); g.setEdge(3, 2, 11); g.setEdge(3, 4, 6);
  g.setEdge(4, 3, 6); g.setEdge(4, 5, 9);
  g.setEdge(5, 0, 14); g.setEdge(5, 2, 2); g.setEdge(5, 4, 9);
  g.path(0, 5);

  EXPECT_THAT(g.path(0, 4), ContainerEq(std::list<Id>{0, 2, 5, 4}));
}
