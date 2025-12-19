#include "bench.hpp"

#include "flat-set/flat_set.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <random>
#include <set>
#include <vector>

namespace {

std::vector<int> sorted_keys(std::size_t n) {
  std::vector<int> keys(n);
  std::iota(keys.begin(), keys.end(), 0);
  return keys;
}

std::vector<int> shuffled_keys(std::vector<int> keys) {
  std::mt19937 rng(12345);
  std::shuffle(keys.begin(), keys.end(), rng);
  return keys;
}

} // namespace

BENCH_CASE("flat_set/build+find_sorted") {
  const auto keys = sorted_keys(n);
  const auto lookups = shuffled_keys(keys);

  stl_bench::run_samples("flat_set/build+find (sorted, my-stl)", n, [&] {
    FlatSet<int> s;
    s.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
      s.insert(keys[i]);

    std::int64_t sum = 0;
    for (std::size_t i = 0; i < n; ++i) {
      auto it = s.find(lookups[i]);
      if (it != s.end())
        sum += *it;
    }
    stl_bench::do_not_optimize(sum);
  });
}

BENCH_CASE("flat_set/build+find (std::set)") {
  const auto keys = sorted_keys(n);
  const auto lookups = shuffled_keys(keys);

  stl_bench::run_samples("flat_set/build+find (sorted, std::set)", n, [&] {
    std::set<int> s;
    for (std::size_t i = 0; i < n; ++i)
      s.insert(keys[i]);

    std::int64_t sum = 0;
    for (std::size_t i = 0; i < n; ++i) {
      auto it = s.find(lookups[i]);
      if (it != s.end())
        sum += *it;
    }
    stl_bench::do_not_optimize(sum);
  });
}
