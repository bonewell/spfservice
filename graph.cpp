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
    auto& v = vertexes_[p.first];
    if (!v.info.visited) {
      unvisited_.erase(&v);
      v.setDistance(a);
      unvisited_.insert(&v);
    }
  });
}

void Graph::markAsVisited(Vertex& a) {
  a.info.visited = true;
  unvisited_.erase(&a);
}

bool Graph::isFinished() const {
  return unvisited_.empty() || (*cbegin(unvisited_))->isInfinity();
}

Vertex& Graph::next() {
  auto* v = *cbegin(unvisited_);
  return vertexes_[v->id];
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
  std::for_each(begin(vertexes_), end(vertexes_),
      [] (auto& p) { p.info = {}; });
  unvisited_.clear();
  std::transform(begin(vertexes_), end(vertexes_),
      std::inserter(unvisited_, unvisited_.end()),
      [] (auto const& p) -> Vertex const*const { return &p; });
}

void Graph::setSource(Vertex& v) {
  unvisited_.erase(&v);
  v.info.distance = 0;
  unvisited_.insert(&v);
}

Id Graph::addVertex() {
  auto id = vertexes_.size();
  vertexes_.push_back({id});
  return id;
}

void Graph::setEdge(Id from, Id to, Distance distance) {
  checkId(from);
  checkId(to);
  checkDistance(distance);
  vertexes_[from].neighbors[to] = distance;
}

std::list<Id> Graph::path(Vertex const& to) const {
  std::list<Id> p{to.id};
  auto prev = to.info.previous;
  while (prev) {
    p.push_front(*prev);
    prev = vertexes_.at(*prev).info.previous;
  }
  return p;
}

std::list<Id> Graph::path(Id from, Id to, bool force) {
  checkId(from);
  checkId(to);
  auto& source = vertexes_.at(from);
  if (force || !source.isSource()) {
    calculate(source);
  }
  auto const& target = vertexes_.at(to);
  return path(target);
}

void Graph::checkId(Id id) {
  if (id >= vertexes_.size()) {
    throw std::invalid_argument{"Wrong vertex ID"};
  }
}

void Graph::removeEdge(Id from, Id to) {
  checkId(from);
  checkId(to);
  vertexes_[from].neighbors.erase(to);
}

void Graph::checkDistance(Distance distance) {
  if (distance < 0) {
    throw std::invalid_argument{"Negative weight"};
  }
}
