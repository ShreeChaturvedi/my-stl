#include "bench.hpp"

#include "../vector/vector.hpp"

#include <random>
#include <vector>

BENCH_CASE("vector/push_back_no_reserve") {
  std::mt19937 rng(123);
  std::uniform_int_distribution<int> dist(0, 1'000'000);

  {
    Vector<int> xs;
    const auto ns = stl_bench::time_it([&] {
      for (std::size_t i = 0; i < n; ++i) xs.push_back(dist(rng));
    });
    stl_bench::do_not_optimize(xs.size());
    stl_bench::report("Vector<int>::push_back (no reserve)", n, ns);
  }

  {
    std::vector<int> xs;
    const auto ns = stl_bench::time_it([&] {
      for (std::size_t i = 0; i < n; ++i) xs.push_back(dist(rng));
    });
    stl_bench::do_not_optimize(xs.size());
    stl_bench::report("std::vector<int>::push_back (no reserve)", n, ns);
  }
}

BENCH_CASE("vector/push_back_reserve") {
  std::mt19937 rng(123);
  std::uniform_int_distribution<int> dist(0, 1'000'000);

  {
    Vector<int> xs;
    xs.reserve(n);
    const auto ns = stl_bench::time_it([&] {
      for (std::size_t i = 0; i < n; ++i) xs.push_back(dist(rng));
    });
    stl_bench::do_not_optimize(xs.size());
    stl_bench::report("Vector<int>::push_back (reserve)", n, ns);
  }

  {
    std::vector<int> xs;
    xs.reserve(n);
    const auto ns = stl_bench::time_it([&] {
      for (std::size_t i = 0; i < n; ++i) xs.push_back(dist(rng));
    });
    stl_bench::do_not_optimize(xs.size());
    stl_bench::report("std::vector<int>::push_back (reserve)", n, ns);
  }
}
