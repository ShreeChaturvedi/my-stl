#include "bench.hpp"

#include "map/map.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <map>
#include <numeric>
#include <random>
#include <vector>

namespace {

std::vector<int> shuffled_keys(std::size_t n) {
  std::vector<int> keys(n);
  std::iota(keys.begin(), keys.end(), 0);
  std::mt19937 rng(12345);
  std::shuffle(keys.begin(), keys.end(), rng);
  return keys;
}

} // namespace

BENCH_CASE("map/build+find") {
  const auto keys = shuffled_keys(n);

  stl_bench::run_samples("map/build+find (my-stl)", n, [&] {
    map<int, int> m;
    for (std::size_t i = 0; i < n; ++i)
      m.insert({keys[i], static_cast<int>(i)});

    std::int64_t sum = 0;
    for (std::size_t i = 0; i < n; ++i) {
      auto it = m.find(keys[i]);
      if (it != m.end())
        sum += it->second;
    }
    stl_bench::do_not_optimize(sum);
  });

  stl_bench::run_samples("map/build+find (std::map)", n, [&] {
    std::map<int, int> m;
    for (std::size_t i = 0; i < n; ++i)
      m.emplace(keys[i], static_cast<int>(i));

    std::int64_t sum = 0;
    for (std::size_t i = 0; i < n; ++i) {
      auto it = m.find(keys[i]);
      if (it != m.end())
        sum += it->second;
    }
    stl_bench::do_not_optimize(sum);
  });
}
