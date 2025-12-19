#include "bench.hpp"

#include "deque/deque.hpp"

#include <cstddef>
#include <deque>
#include <random>
#include <vector>

BENCH_CASE("deque/push_pop") {
  std::mt19937 rng(123);
  std::uniform_int_distribution<int> dist(0, 1'000'000);
  std::vector<int> values(n);
  for (std::size_t i = 0; i < n; ++i)
    values[i] = dist(rng);

  stl_bench::run_samples("Deque<int>::push_back+pop_front", n, [&] {
    Deque<int> d;
    for (std::size_t i = 0; i < n; ++i)
      d.push_back(values[i]);
    for (std::size_t i = 0; i < n; ++i)
      d.pop_front();
    stl_bench::do_not_optimize(d.size());
  });

  stl_bench::run_samples("std::deque<int>::push_back+pop_front", n, [&] {
    std::deque<int> d;
    for (std::size_t i = 0; i < n; ++i)
      d.push_back(values[i]);
    for (std::size_t i = 0; i < n; ++i)
      d.pop_front();
    stl_bench::do_not_optimize(d.size());
  });
}
