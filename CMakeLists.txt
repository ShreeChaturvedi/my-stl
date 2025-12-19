cmake_minimum_required(VERSION 3.20)

project(my_stl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

option(STL_ENABLE_WERROR "Treat warnings as errors" ON)
option(STL_ENABLE_COVERAGE "Enable clang/gcc coverage instrumentation" OFF)

include(FetchContent)

add_library(stl INTERFACE)
target_include_directories(stl INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

if (MSVC)
  target_compile_options(stl INTERFACE /W4 /permissive-)
else()
  target_compile_options(stl INTERFACE -Wall -Wextra -Wpedantic)
endif()
if (STL_ENABLE_WERROR)
  if (MSVC)
    target_compile_options(stl INTERFACE /WX)
  else()
    target_compile_options(stl INTERFACE -Werror)
  endif()
endif()

if (STL_ENABLE_COVERAGE)
  if (NOT MSVC)
    target_compile_options(stl INTERFACE -O0 -g -fprofile-instr-generate -fcoverage-mapping)
    target_link_options(stl INTERFACE -fprofile-instr-generate -fcoverage-mapping)
  endif()
endif()

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.9.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(stl_tests
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
  tests/test_queue.cpp
  tests/test_span.cpp
  tests/test_map_set.cpp
  tests/test_unordered_set.cpp
  tests/test_unordered_multi.cpp
  tests/test_deque.cpp
  tests/test_string.cpp
  tests/test_flat_map.cpp
  tests/test_flat_set.cpp
  tests/test_small_vector.cpp
  tests/test_stable_vector.cpp
)
target_link_libraries(stl_tests PRIVATE stl Catch2::Catch2WithMain)
include(Catch)
catch_discover_tests(stl_tests)

find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
  set(DOXYGEN_PROJECT_NAME "my-stl")
  set(DOXYGEN_PROJECT_BRIEF "C++23 build-your-own STL")
  set(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen")
  set(DOXYGEN_INPUT_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/array-list"
    "${CMAKE_CURRENT_SOURCE_DIR}/linked-list"
    "${CMAKE_CURRENT_SOURCE_DIR}/ring-buffer"
    "${CMAKE_CURRENT_SOURCE_DIR}/vector"
    "${CMAKE_CURRENT_SOURCE_DIR}/unordered-map"
    "${CMAKE_CURRENT_SOURCE_DIR}/heap"
    "${CMAKE_CURRENT_SOURCE_DIR}/trie"
    "${CMAKE_CURRENT_SOURCE_DIR}/unique-ptr"
    "${CMAKE_CURRENT_SOURCE_DIR}/lru-cache"
    "${CMAKE_CURRENT_SOURCE_DIR}/forward-list"
    "${CMAKE_CURRENT_SOURCE_DIR}/stack"
    "${CMAKE_CURRENT_SOURCE_DIR}/priority-queue"
    "${CMAKE_CURRENT_SOURCE_DIR}/list"
    "${CMAKE_CURRENT_SOURCE_DIR}/queue"
    "${CMAKE_CURRENT_SOURCE_DIR}/span"
    "${CMAKE_CURRENT_SOURCE_DIR}/utility"
    "${CMAKE_CURRENT_SOURCE_DIR}/rb-tree"
    "${CMAKE_CURRENT_SOURCE_DIR}/map"
    "${CMAKE_CURRENT_SOURCE_DIR}/set"
    "${CMAKE_CURRENT_SOURCE_DIR}/unordered-set"
    "${CMAKE_CURRENT_SOURCE_DIR}/unordered-multimap"
    "${CMAKE_CURRENT_SOURCE_DIR}/unordered-multiset"
    "${CMAKE_CURRENT_SOURCE_DIR}/deque"
    "${CMAKE_CURRENT_SOURCE_DIR}/string"
    "${CMAKE_CURRENT_SOURCE_DIR}/flat-map"
    "${CMAKE_CURRENT_SOURCE_DIR}/flat-set"
    "${CMAKE_CURRENT_SOURCE_DIR}/small-vector"
    "${CMAKE_CURRENT_SOURCE_DIR}/stable-vector"
  )
  list(JOIN DOXYGEN_INPUT_DIRS " " DOXYGEN_INPUT_DIRS)
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
                 "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
  add_custom_target(doxygen
    COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
endif()

add_executable(stl_bench
  bench/bench_main.cpp
  bench/bench_deque.cpp
  bench/bench_map.cpp
  bench/bench_set.cpp
  bench/bench_vector.cpp
  bench/bench_unordered_map.cpp
  bench/bench_flat_map.cpp
  bench/bench_flat_set.cpp
  bench/bench_small_vector.cpp
  bench/bench_stable_vector.cpp
)
target_link_libraries(stl_bench PRIVATE stl)
target_compile_options(stl_bench PRIVATE -O3)
