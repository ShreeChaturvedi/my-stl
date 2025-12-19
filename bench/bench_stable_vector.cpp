#include "bench.hpp"

#include "stable-vector/stable_vector.hpp"

#include <cstddef>
#include <memory>
#include <random>
#include <vector>

BENCH_CASE("stable_vector/push_back") {
  std::mt19937 rng(123);
  std::uniform_int_distribution<int> dist(0, 1'000'000);
  std::vector<int> values(n);
  for (std::size_t i = 0; i < n; ++i)
    values[i] = dist(rng);

  stl_bench::run_samples("StableVector<int>::push_back", n, [&] {
    StableVector<int> xs;
    xs.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
      xs.push_back(values[i]);
    stl_bench::do_not_optimize(xs.size());
  });

  stl_bench::run_samples("std::vector<unique_ptr<int>>::push_back", n, [&] {
    std::vector<std::unique_ptr<int>> xs;
    xs.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
      xs.push_back(std::make_unique<int>(values[i]));
    stl_bench::do_not_optimize(xs.size());
  });
}
