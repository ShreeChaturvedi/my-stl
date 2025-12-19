#include "bench.hpp"

#include "flat-map/flat_map.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <map>
#include <numeric>
#include <random>
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

}  // namespace

BENCH_CASE("flat_map/build+find_sorted") {
  const auto keys = sorted_keys(n);
  const auto lookups = shuffled_keys(keys);

  stl_bench::run_samples("flat_map/build+find (sorted, my-stl)", n, [&] {
    FlatMap<int, int> m;
    m.reserve(n);
    for (std::size_t i = 0; i < n; ++i) m.insert({keys[i], static_cast<int>(i)});

    std::int64_t sum = 0;
    for (std::size_t i = 0; i < n; ++i) {
      auto it = m.find(lookups[i]);
      if (it != m.end()) sum += it->second;
    }
    stl_bench::do_not_optimize(sum);
  });
}

BENCH_CASE("flat_map/build+find (std::map)") {
  const auto keys = sorted_keys(n);
  const auto lookups = shuffled_keys(keys);

  stl_bench::run_samples("flat_map/build+find (sorted, std::map)", n, [&] {
    std::map<int, int> m;
    for (std::size_t i = 0; i < n; ++i) m.emplace(keys[i], static_cast<int>(i));

    std::int64_t sum = 0;
    for (std::size_t i = 0; i < n; ++i) {
      auto it = m.find(lookups[i]);
      if (it != m.end()) sum += it->second;
    }
    stl_bench::do_not_optimize(sum);
  });
}
