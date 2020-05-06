#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <string>

#include "graph.h"

/**
 * Processor serves incoming requests.
 */
class Processor {
public:
  /**
   * Serves incoming request.
   * @param request - incoming request.
   * @return response.
   */
  std::string serve(std::string request);

private:
  Graph graph_;
};

#endif /* PROCESSOR_H_ */
