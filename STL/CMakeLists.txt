cmake_minimum_required(VERSION 3.20)

project(my_stl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
)
target_link_libraries(stl_tests PRIVATE stl)

add_executable(stl_bench
  bench/bench_main.cpp
)
target_link_libraries(stl_bench PRIVATE stl)
target_compile_options(stl_bench PRIVATE -O3)
