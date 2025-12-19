#include "bench.hpp"

#include "vector/vector.hpp"

#include <cstddef>
#include <random>
#include <vector>

BENCH_CASE("vector/push_back_no_reserve") {
  std::mt19937 rng(123);
  std::uniform_int_distribution<int> dist(0, 1'000'000);
  std::vector<int> values(n);
  for (std::size_t i = 0; i < n; ++i)
    values[i] = dist(rng);

  stl_bench::run_samples("Vector<int>::push_back (no reserve)", n, [&] {
    Vector<int> xs;
    for (std::size_t i = 0; i < n; ++i)
      xs.push_back(values[i]);
    stl_bench::do_not_optimize(xs.size());
  });

  stl_bench::run_samples("std::vector<int>::push_back (no reserve)", n, [&] {
    std::vector<int> xs;
    for (std::size_t i = 0; i < n; ++i)
      xs.push_back(values[i]);
    stl_bench::do_not_optimize(xs.size());
  });
}

BENCH_CASE("vector/push_back_reserve") {
  std::mt19937 rng(123);
  std::uniform_int_distribution<int> dist(0, 1'000'000);
  std::vector<int> values(n);
  for (std::size_t i = 0; i < n; ++i)
    values[i] = dist(rng);

  stl_bench::run_samples("Vector<int>::push_back (reserve)", n, [&] {
    Vector<int> xs;
    xs.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
      xs.push_back(values[i]);
    stl_bench::do_not_optimize(xs.size());
  });

  stl_bench::run_samples("std::vector<int>::push_back (reserve)", n, [&] {
    std::vector<int> xs;
    xs.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
      xs.push_back(values[i]);
    stl_bench::do_not_optimize(xs.size());
  });
}
