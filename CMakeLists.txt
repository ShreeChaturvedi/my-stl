cmake_minimum_required(VERSION 3.20)

project(my_stl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

option(STL_ENABLE_WERROR "Treat warnings as errors" ON)
option(STL_ENABLE_COVERAGE "Enable clang/gcc coverage instrumentation" OFF)

add_library(stl INTERFACE)
target_include_directories(stl INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_options(stl INTERFACE -Wall -Wextra -Wpedantic)
if (STL_ENABLE_WERROR)
  target_compile_options(stl INTERFACE -Werror)
endif()

if (STL_ENABLE_COVERAGE)
  target_compile_options(stl INTERFACE -O0 -g -fprofile-instr-generate -fcoverage-mapping)
  target_link_options(stl INTERFACE -fprofile-instr-generate -fcoverage-mapping)
endif()

add_executable(stl_tests
  tests/test_main.cpp
  tests/test_array_list.cpp
  tests/test_linked_list.cpp
  tests/test_forward_list.cpp
  tests/test_ring_buffer.cpp
  tests/test_heap.cpp
  tests/test_unique_ptr.cpp
  tests/test_vector.cpp
  tests/test_unordered_map.cpp
  tests/test_lru_cache.cpp
  tests/test_trie.cpp
  tests/test_stack.cpp
  tests/test_priority_queue.cpp
  tests/test_list.cpp
)
target_link_libraries(stl_tests PRIVATE stl)
add_test(NAME stl_tests COMMAND stl_tests)

add_executable(stl_bench
  bench/bench_main.cpp
  bench/bench_vector.cpp
  bench/bench_unordered_map.cpp
)
target_link_libraries(stl_bench PRIVATE stl)
target_compile_options(stl_bench PRIVATE -O3)
