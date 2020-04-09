#ifndef GRAPH_H_
#define GRAPH_H_

#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Id = long long int;
using Distance = double;
using Neighbors = std::unordered_map<Id, Distance>;

struct Vertex {
  Id id;
  Distance distance{std::numeric_limits<Distance>::max()};
  Neighbors neighbors;

  /**
   * Updates distance to this vertex through a vertex
   * @param a - vertex which is used to calculate new distance
   */
  void update(Vertex const& a);
};

struct Graph {
  std::vector<Vertex> vertexes;
  std::unordered_set<Id> unvisitedIds;

  Vertex& operator[](Id id) { return vertexes[id]; }
  Vertex const& operator[](Id id) const { return vertexes[id]; }

  /**
   * Updates distance to neighbors of the current vertex through it
   * @param a - current vertex
   */
  void update(Vertex const& a);

  /**
   * Checks whether the vertex is visited or not
   * @return true is the vertex is unvisited
   */
  bool isUnvisited(Id id) const;
};

#endif /* GRAPH_H_ */
