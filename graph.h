#ifndef GRAPH_H_
#define GRAPH_H_

#include <limits>
#include <list>
#include <set>
#include <unordered_map>

using Id = long long unsigned int;
using Distance = double;

struct Vertex;

struct SpfInfo {
  Distance distance{std::numeric_limits<Distance>::infinity()};
  bool visited{false};
  Vertex const* previous{nullptr};
};

struct Vertex {
  Id id;
  std::unordered_map<Vertex*, Distance> neighbors;
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
   * Adds new vertex in graph.
   * @return id of the vertex.
   */
  Id addVertex();

  /**
   * Removes vertex and its edges.
   * @param id - the vertex.
   */
  void removeVertex(Id id);

  /**
   * Adds a new edge or update distance of the already existed one.
   * @param from - begin the edge.
   * @param to - end of the edge.
   * @param distance - weight of the edge.
   */
  void setEdge(Id from, Id to, Distance distance);

  /**
   * Remove edge.
   * @param from - begin the edge.
   * @param to - end of the edge.
   */
  void removeEdge(Id from, Id to);

  /**
   * Gets path from a source to a target vertex.
   * @param from - the source vertex.
   * @param to - the target vertex.
   * @param force - calculate distances also if true.
   * @return list of the vertex by order.
   */
  std::list<Id> path(Id from, Id to, bool force = false);

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
  Vertex& operator[](Id id) { return *at(id); }

  bool hasUnvisited(Id id)
  { return unvisited_.find(at(id)) != unvisited_.end(); }

private:
  struct LessDistance { bool operator()(Vertex const* lhs, Vertex const* rhs) const; };
  Vertex* at(Id id);
  void checkDistance(Distance distance) const;
  Id id_{0};
  std::unordered_map<Id, Vertex> vertexes_;
  std::set<Vertex*, LessDistance> unvisited_{LessDistance{}};
};

#endif /* GRAPH_H_ */
