#include "graph.h"

#include <algorithm>
#include <iterator>

bool Graph::LessDistance::operator()(VCPtr lhs, VCPtr rhs) const {
  return (lhs->info.distance < rhs->info.distance)
      || (lhs->info.distance == rhs->info.distance && lhs->id < rhs->id);
}

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
      [this, &a](auto const& p) {
    auto& v = vertexes[p.first];
    if (!v.info.visited) {
      unvisited.erase(&v);
      v.setDistance(a);
      unvisited.insert(&v);
    }
  });
}

void Graph::markAsVisited(Vertex& a) {
  a.info.visited = true;
  unvisited.erase(&a);
}

bool Graph::isFinished() const {
  return unvisited.empty() || (*cbegin(unvisited))->isInfinity();
}

Vertex& Graph::next() {
  auto* v = *cbegin(unvisited);
  return vertexes[v->id];
}

void Graph::calculate(Vertex& from) {
  init();
  setSource(from);
  do {
    auto& current = next();
    updateNeighbors(current);
    markAsVisited(current);
  } while (!isFinished());
}

void Graph::init() {
  std::transform(begin(vertexes), end(vertexes),
      std::inserter(unvisited, unvisited.end()),
      [] (auto const& p) -> Vertex const*const { return &p; });
  std::for_each(begin(vertexes), end(vertexes),
      [] (auto& p) { p.info = {}; });
}

void Graph::setSource(Vertex& v) {
  unvisited.erase(&v);
  v.info.distance = 0;
  unvisited.insert(&v);
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
  vertexes.push_back({id, std::move(neighbors)});
}

std::list<Id> Graph::path(Id from, Id to) {
  auto& source = vertexes.at(from);
  if (!source.isSource()) {
    calculate(source);
  }
  auto const& target = vertexes.at(to);
  return path(target);
}
