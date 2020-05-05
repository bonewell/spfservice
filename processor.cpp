#include "processor.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <sstream>
#include <utility>

namespace ptree = boost::property_tree;
namespace json = boost::property_tree::json_parser;

namespace {
/**
 * Abstract action.
 */
class Action {
public:
  /**
   * Creates specific command.
   * @param input - input data about command.
   * @return pointer to specific command.
   */
  static std::unique_ptr<Action> create(ptree::ptree input);
  Action(ptree::ptree input): input_{std::move(input)} {}
  virtual ~Action() = default;

  /**
   * Executes command.
   * @param graph - the graph to run the command on it.
   * @return result of the command.
   */
  ptree::ptree execute(Graph& graph);

protected:
  /**
   * Runs command.
   * @param graph - the graph to run the command on it.
   * @return result of the command.
   */
  virtual ptree::ptree run(Graph& graph) = 0;

  ptree::ptree input_;
};

class AddVertex: public Action {
public:
  using Action::Action;
  ptree::ptree run(Graph& graph) {
    ptree::ptree output;
    output.put("id", graph.addVertex());
    return output;
  }
};

class AddEdge: public Action {
public:
  using Action::Action;
  ptree::ptree run(Graph& graph) {
    auto from = input_.get_optional<Id>("from");
    auto to = input_.get_optional<Id>("to");
    auto weight = input_.get_optional<Distance>("weight");
    if (!from || !to || !weight) {
      throw std::invalid_argument{"Not enough data"};
    }
    graph.setEdge(*from, *to, *weight);
    return {};
  }
};

class RemoveEdge: public Action {
public:
  using Action::Action;
  ptree::ptree run(Graph& graph) {
    auto from = input_.get_optional<Id>("from");
    auto to = input_.get_optional<Id>("to");
    if (!from || !to) {
      throw std::invalid_argument{"Not enough data"};
    }
    graph.removeEdge(*from, *to);
    return {};
  }
};

class GetPath: public Action {
public:
  using Action::Action;
  ptree::ptree run(Graph& graph) {
    auto from = input_.get_optional<Id>("from");
    auto to = input_.get_optional<Id>("to");
    if (!from || !to) {
      throw std::invalid_argument{"Not enough data"};
    }
    ptree::ptree output;
    ptree::ptree array;
    auto ids = graph.path(*from, *to);
    for (auto id: ids) {
      ptree::ptree item;
      item.put("", id);
      array.push_back(std::make_pair("", item));
    }
    output.put_child("ids", array);
    return output;
  }
};

class Unknown: public Action {
public:
  using Action::Action;
  ptree::ptree run(Graph&) {
    throw std::invalid_argument{"Unknown action"};
  }
};

std::unique_ptr<Action> Action::create(ptree::ptree input) {
  auto action = input.get<std::string>("action", "unknown");

  if (action == "AddVertex") {
    return std::make_unique<AddVertex>(std::move(input));
  } else if (action == "AddEdge") {
    return std::make_unique<AddEdge>(std::move(input));
  } else if (action == "RemoveEdge") {
    return std::make_unique<RemoveEdge>(std::move(input));
  } else if (action == "GetPath") {
    return std::make_unique<GetPath>(std::move(input));
  }
  return std::make_unique<Unknown>(std::move(input));
}

ptree::ptree Action::execute(Graph& graph) try {
  return run(graph);
} catch (std::exception const& e) {
  ptree::ptree error;
  error.put("error", e.what());
  return error;
} catch (...) {
  ptree::ptree error;
  error.put("error", "Unknown error");
  return error;
}
}  // namespace

std::string Processor::serve(std::string request) {
  std::istringstream iss{request};
  ptree::ptree input;
  json::read_json(iss, input);

  std::ostringstream oss;
  auto output = Action::create(std::move(input))->execute(graph_);
  json::write_json_helper(oss, output, 0, false);
  return oss.str();
}
