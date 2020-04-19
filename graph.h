#ifndef GRAPH_H_
#define GRAPH_H_

#include <limits>
#include <list>
#include <optional>
#include <set>
#include <unordered_map>
#include <vector>

using Id = long long unsigned int;
using Distance = double;
using Neighbors = std::unordered_map<Id, Distance>;

struct SpfInfo {
  Distance distance{std::numeric_limits<Distance>::infinity()};
  bool visited{false};
  std::optional<Id> previous;
};

struct Vertex {
  Id id;
  Neighbors neighbors;
  SpfInfo info;

  /**
   * Sets new distance to this vertex through a vertex.
   * @param a - vertex which is used to calculate new distance.
   */
  void setDistance(Vertex const& a);

  /**
   * Checks whether distance to this vertex is infinite.
   * @return true if the distance is infinite.
   */
  bool isInfinity() const;

  /**
   * Checks whether the vertex is source of the path.
   * @return true if the vertex is source;
   */
  bool isSource() const;
};

class Graph {
public:

  /**
   * Inserts vertex into graph.
   * @param id - ID of the vertex.
   * @param neighbors - distances to neighbors.
   */
  void insert(Id id, Neighbors neighbors = {});

  /**
   * Gets path from a source to a target vertex.
   * @param from - the source vertex.
   * @param to - the target vertex.
   * @return list of the vertex by order.
   */
  std::list<Id> path(Id from, Id to);

protected:
  /**
   * Initializes graph to calculate.
   */
  void init();

  /**
   * Sets the vertex like source.
   * @param v - the source vertex.
   */
  void setSource(Vertex& v);

  /**
   * Updates distance to neighbors of the current vertex through it.
   * @param a - current vertex.
   */
  void updateNeighbors(Vertex const& a);

  /**
   * Marks vertex as visited.
   * @param a - the vertex.
   */
  void markAsVisited(Vertex& a);

  /**
   * Checks whether calculation finished.
   * @return true if calculation finished.
   */
  bool isFinished() const;

  /**
   * Gets the next unvisited vertex with the minimum distance.
   * @return the vertex.
   */
  Vertex& next();

  /**
   * Calculate distance from source vertex to every one.
   * @param from - the source vertex.
   */
  void calculate(Vertex& from);

  /**
   * Gets path to the target vertex.
   * @param to - the target vertex.
   * @return list of the vertex by order.
   */
  std::list<Id> path(Vertex const& to) const;

  /**
   * Gets vertex by id
   * @param id - ID of the vertex
   * @return vertex
   */
  Vertex& operator[](Id id) { return vertexes[id]; }

  bool hasUnvisited(Id id)
  { return unvisited.find(&vertexes[id]) != unvisited.end(); }

private:
  using VCPtr = Vertex const*const;
  struct LessDistance { bool operator()(VCPtr lhs, VCPtr rhs) const; };
  std::vector<Vertex> vertexes;
  std::set<VCPtr, LessDistance> unvisited{LessDistance{}};
};

#endif /* GRAPH_H_ */
