#include "graph.h"

#include <algorithm>

void Vertex::update(Vertex const& a) {
  auto d = a.distance + a.neighbors.at(id);
  if (distance > d) {
    distance = d;
    previous = a.id;
  }
}

bool Vertex::infinite() const {
  return distance == std::numeric_limits<Distance>::max();
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

void Graph::mark(const Vertex &a) {
  unvisitedIds.erase(a.id);
}

bool Graph::isFinished() const {
  // TODO: O(n) -> O(1)
  return std::all_of(begin(unvisitedIds), end(unvisitedIds), [this] (auto id) {
    return vertexes[id].infinite();
  });
}

Vertex const& Graph::next() const {
  // TODO: O(n) -> O(1)
  return vertexes[*std::min_element(begin(unvisitedIds), end(unvisitedIds),
      [this] (auto id1, auto id2) {
    return vertexes[id1].distance < vertexes[id2].distance;
  })];
}

Distance Graph::calculate(Id from, Id to) {
  vertexes[from].distance = 0;
  do {
    auto const& current = next();
    update(current);
    mark(current);
  } while (!isFinished());
  return vertexes[to].distance;
}

std::list<Id> Graph::path(Id to) const {
  std::list<Id> p{to};
  auto prev = vertexes[to].previous;
  while (prev != -1) {
    p.push_front(prev);
    prev = vertexes[prev].previous;
  }
  return p;
}
