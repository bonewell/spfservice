cmake_minimum_required(VERSION 3.10)

project(spfservice)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(${PROJECT_NAME}
  "main.cpp"
  "graph.cpp"
  "processor.cpp"
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE
  CONAN_PKG::boost
)

enable_testing()

set(UNIT_TEST ${PROJECT_NAME}_unittest)
add_executable(${UNIT_TEST}
  "graph.cpp"
  "graph_test.cpp"
  "processor.cpp"
  "processor_test.cpp"
)
target_compile_features(${UNIT_TEST} PRIVATE cxx_std_17)
target_link_libraries(${UNIT_TEST} PRIVATE
  CONAN_PKG::gtest
  CONAN_PKG::boost
)
add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})

set(BENCHMARK ${PROJECT_NAME}_benchmark)
add_executable(${BENCHMARK}
  "graph.cpp"
  "graph_benchmark.cpp"
)
target_compile_features(${BENCHMARK} PRIVATE cxx_std_17)
target_link_libraries(${BENCHMARK} PRIVATE
  CONAN_PKG::benchmark
)
