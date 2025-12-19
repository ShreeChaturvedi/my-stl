#include "bench.hpp"

#include "small-vector/small_vector.hpp"

#include <cstddef>
#include <random>
#include <vector>

BENCH_CASE("small_vector/push_back") {
  std::mt19937 rng(123);
  std::uniform_int_distribution<int> dist(0, 1'000'000);
  std::vector<int> values(n);
  for (std::size_t i = 0; i < n; ++i)
    values[i] = dist(rng);

  stl_bench::run_samples("SmallVector<int,16>::push_back", n, [&] {
    SmallVector<int, 16> xs;
    for (std::size_t i = 0; i < n; ++i)
      xs.push_back(values[i]);
    stl_bench::do_not_optimize(xs.size());
  });

  stl_bench::run_samples("std::vector<int>::push_back", n, [&] {
    std::vector<int> xs;
    for (std::size_t i = 0; i < n; ++i)
      xs.push_back(values[i]);
    stl_bench::do_not_optimize(xs.size());
  });
}
