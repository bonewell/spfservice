#include "graph.h"

#include <algorithm>
#include <iterator>

void Vertex::setDistance(Vertex const& a) {
  if (info.visited) return;
  auto d = a.info.distance + a.neighbors.at(id);
  if (d < info.distance) {
    info.distance = d;
    info.previous = a.id;
  }
}

bool Vertex::isInfinity() const {
  return info.distance == std::numeric_limits<Distance>::infinity();
}

bool Vertex::isSource() const {
  return info.visited && !info.previous;
}

void Graph::updateNeighbors(Vertex const& a) {
  std::for_each(begin(a.neighbors), end(a.neighbors),
      [this, &a](auto const& p) { vertexes[p.first].setDistance(a); });
}

void Graph::markAsVisited(Vertex& a) {
  a.info.visited = true;
  unvisitedIds.erase(a.id);
}

bool Graph::isFinished() const {
  // TODO: O(n) -> O(1)
  return std::all_of(begin(unvisitedIds), end(unvisitedIds), [this] (auto id) {
    return vertexes.at(id).isInfinity();
  });
}

Vertex& Graph::next() {
  // TODO: O(n) -> O(1)
  return vertexes[*std::min_element(begin(unvisitedIds), end(unvisitedIds),
      [this] (auto id1, auto id2) {
    return vertexes.at(id1).info.distance < vertexes.at(id2).info.distance;
  })];
}

void Graph::calculate(Vertex& from) {
  init();
  from.info.distance = 0;
  do {
    auto& current = next();
    updateNeighbors(current);
    markAsVisited(current);
  } while (!isFinished());
}

void Graph::init() {
  std::transform(begin(vertexes), end(vertexes),
      std::inserter(unvisitedIds, unvisitedIds.end()),
      [] (auto const& p) -> Id { return p.second.id; });
  std::for_each(begin(vertexes), end(vertexes),
      [] (auto& p) { p.second.info = {}; });
}

std::list<Id> Graph::path(Vertex const& to) const {
  std::list<Id> p{to.id};
  auto prev = to.info.previous;
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
  auto& source = vertexes.at(from);
  if (!source.isSource()) {
    calculate(source);
  }
  auto const& target = vertexes.at(to);
  return path(target);
}
