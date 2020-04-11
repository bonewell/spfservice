#include "graph.h"

#include <algorithm>
#include <iterator>

void Vertex::update(Vertex const& a) {
  if (info.visited) return;
  auto d = a.info.distance + a.neighbors.at(id);
  if (d < info.distance) {
    info.distance = d;
    info.previous = a.id;
  }
}

bool Vertex::infinite() const {
  return info.distance == std::numeric_limits<Distance>::infinity();
}

bool Vertex::isSource() const {
  return info.visited && !info.previous;
}

void Graph::update(Vertex const& a) {
  std::for_each(begin(a.neighbors), end(a.neighbors),
      [this, &a](auto const& p) { vertexes[p.first].update(a); });
}

void Graph::mark(Vertex& a) {
  a.info.visited = true;
  unvisitedIds.erase(a.id);
}

bool Graph::isFinished() const {
  // TODO: O(n) -> O(1)
  return std::all_of(begin(unvisitedIds), end(unvisitedIds), [this] (auto id) {
    return vertexes.at(id).infinite();
  });
}

Vertex& Graph::next() {
  // TODO: O(n) -> O(1)
  return vertexes[*std::min_element(begin(unvisitedIds), end(unvisitedIds),
      [this] (auto id1, auto id2) {
    return vertexes.at(id1).info.distance < vertexes.at(id2).info.distance;
  })];
}

Distance Graph::calculate(Id to) {
  do {
    auto& current = next();
    update(current);
    mark(current);
  } while (!isFinished());
  return vertexes[to].info.distance;
}

void Graph::source(Id id) {
  std::transform(begin(vertexes), end(vertexes),
      std::inserter(unvisitedIds, unvisitedIds.end()),
      [] (auto const& p) -> Id { return p.second.id; });
  std::for_each(begin(vertexes), end(vertexes),
      [] (auto& p) { p.second.info = {}; });
  vertexes[id].info.distance = 0;
}

std::list<Id> Graph::path(Id to) const {
  std::list<Id> p{to};
  auto prev = vertexes.at(to).info.previous;
  while (prev) {
    p.push_front(*prev);
    prev = vertexes.at(*prev).info.previous;
  }
  return p;
}

void Graph::insert(Id id, Neighbors neighbors) {
  vertexes.emplace(id, Vertex{id, std::move(neighbors)});
}

std::list<Id> Graph::path(Id from, Id to) {
  if (!vertexes.at(from).isSource()) {
    source(from);
    calculate(to);
  }
  return path(to);
}
