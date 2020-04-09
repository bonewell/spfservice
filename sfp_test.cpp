#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <numeric>
#include <unordered_map>

using ::testing::Eq;

using Id = long long int;
using Distance = double;
using Neighbors = std::unordered_map<Id, Distance>;

struct Vertex {
  Id id;
  Distance distance{std::numeric_limits<Distance>::max()};
  Neighbors neighbors;
};

void update_distance(Vertex const& a, Vertex& b) {
  auto d = a.distance + a.neighbors.at(b.id);
  if (b.distance > d) {
    b.distance = d;
  }
}

TEST(SPF, ShorterPathFound) {
  Vertex a{0, 6};
  Vertex b{1, 10};
  a.neighbors[b.id] = 2;

  update_distance(a, b);

  EXPECT_THAT(b.distance, Eq(8));
}

TEST(SPF, NoShorterPath) {
  Vertex a{0, 6};
  Vertex b{1, 7};
  a.neighbors[b.id] = 2;

  update_distance(a, b);

  EXPECT_THAT(b.distance, Eq(7));
}
