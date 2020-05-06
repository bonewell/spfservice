#include "graph.h"

#include <algorithm>
#include <iterator>

bool Graph::LessDistance::operator()(Vertex* lhs, Vertex* rhs) const {
  return (lhs->info.distance < rhs->info.distance)
      || (lhs->info.distance == rhs->info.distance && lhs->id < rhs->id);
}

void Vertex::setDistance(Vertex const& a) {
  if (info.visited) return;
  auto d = a.info.distance + a.neighbors.at(this);
  if (d < info.distance) {
    info.distance = d;
    info.previous = const_cast<Vertex*>(&a);
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
    auto v = p.first;
    if (!v->info.visited) {
      unvisited_.erase(v);
      v->setDistance(a);
      unvisited_.insert(v);
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

Vertex& Graph::next() const {
  return const_cast<Vertex&>(**begin(unvisited_));
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
      [] (auto& p) { p.second.info = {}; });
  unvisited_.clear();
  std::transform(begin(vertexes_), end(vertexes_),
      std::inserter(unvisited_, unvisited_.end()),
      [] (auto& p) -> Vertex* { return &p.second; });
}

void Graph::setSource(Vertex& v) {
  unvisited_.erase(&v);
  v.info.distance = 0;
  unvisited_.insert(&v);
}

Id Graph::addVertex() {
  vertexes_.emplace(id_, Vertex{id_});
  return id_++;
}

void Graph::setEdge(Id from, Id to, Distance distance) {
  checkDistance(distance);
  at(from)->neighbors[&*at(to)] = distance;
}

std::list<Id> Graph::path(Vertex const& to) const {
  std::list<Id> p{to.id};
  auto prev = to.info.previous;
  while (prev) {
    p.push_front(prev->id);
    prev = prev->info.previous;
  }
  return p;
}

std::list<Id> Graph::path(Id from, Id to, bool force) {
  auto& source = *at(from);
  auto const& target = *at(to);
  if (force || !source.isSource()) {
    calculate(source);
  }
  return path(target);
}

Vertex* Graph::at(Id id) {
  auto it = vertexes_.find(id);
  if (it == end(vertexes_)) {
    throw std::invalid_argument{"Wrong vertex ID"};
  }
  return &it->second;
}

void Graph::removeEdge(Id from, Id to) {
  at(from)->neighbors.erase(&*at(to));
}

void Graph::checkDistance(Distance distance) {
  if (distance < 0) {
    throw std::invalid_argument{"Negative weight"};
  }
}
