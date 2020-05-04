#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "processor.h"

using ::testing::Eq;

TEST(Processor, UnknownAction) {
  EXPECT_THAT(Processor{}.serve(R"({"action":"RemoveGraph"})"),
      Eq(R"({"error":"Unknown action"})"));
}

TEST(Processor, NoAction) {
  EXPECT_THAT(Processor{}.serve(R"({})"), Eq(R"({"error":"Unknown action"})"));
}

TEST(Processor, AddVertex) {
  EXPECT_THAT(Processor{}.serve(R"({"action":"AddVertex"})"), Eq(R"({"id":"0"})"));
}

TEST(Processor, AddEdge) {
  Processor p;
  p.serve(R"({"action":"AddVertex"})");
  p.serve(R"({"action":"AddVertex"})");

  EXPECT_THAT(p.serve(R"({"action":"AddEdge","from":0,"to":1,"weight":3.4})"),
      Eq(R"({})"));
}

TEST(Processor, AddEdgeWithWrongID) {
  Processor p;
  p.serve(R"({"action":"AddVertex"})");

  EXPECT_THAT(p.serve(R"({"action":"AddEdge","from":0,"to":1,"weight":3.4})"),
      Eq(R"({"error":"Wrong vertex ID"})"));
}

TEST(Processor, AddEdgeWithoutWeight) {
  EXPECT_THAT(Processor{}.serve(R"({"action":"AddEdge","from":0,"to":1})"),
      Eq(R"({"error":"Not enough data"})"));
}

TEST(Processor, AddEdgeWithoutTo) {
  EXPECT_THAT(Processor{}.serve(R"({"action":"AddEdge","from":0,"weight":3.4})"),
      Eq(R"({"error":"Not enough data"})"));
}

TEST(Processor, AddEdgeWithoutFrom) {
  EXPECT_THAT(Processor{}.serve(R"({"action":"AddEdge","to":1,"weight":3.4})"),
      Eq(R"({"error":"Not enough data"})"));
}

TEST(Processor, GetPath) {
  Processor p;
  p.serve(R"({"action":"AddVertex"})");
  p.serve(R"({"action":"AddVertex"})");
  p.serve(R"({"action":"AddEdge","from":0,"to":1,"weight":3.4})");

  EXPECT_THAT(p.serve(R"({"action":"GetPath","from":0,"to":1})"),
      Eq(R"({"ids":["0","1"]})"));
}

TEST(Processor, GetPathWithoutFrom) {
  EXPECT_THAT(Processor{}.serve(R"({"action":"GetPath","to":1})"),
      Eq(R"({"error":"Not enough data"})"));
}

TEST(Processor, GetPathWithoutTo) {
  EXPECT_THAT(Processor{}.serve(R"({"action":"GetPath","from":0})"),
      Eq(R"({"error":"Not enough data"})"));
}
