#include "graph.h"

void Vertex::update(Vertex const& a) {
  auto d = a.distance + a.neighbors.at(id);
  if (distance > d) {
    distance = d;
  }
}

void Graph::update(Vertex const& a) {
  for (auto [id, _] : a.neighbors) {
    if (isUnvisited(id)) {
      vertexes[id].update(a);
    }
  }
}

bool Graph::isUnvisited(Id id) const {
  return unvisitedIds.find(id) != unvisitedIds.end();
}
