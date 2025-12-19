#include "bench.hpp"

#include "unordered-map/unordered_map.hpp"

#include <random>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> make_keys(std::size_t n) {
  std::vector<std::string> keys;
  keys.reserve(n);
  for (std::size_t i = 0; i < n; ++i)
    keys.push_back("k" + std::to_string(i));
  return keys;
}

BENCH_CASE("unordered_map/emplace_reserve") {
  auto keys = make_keys(n);

  stl_bench::run_samples("unordered_map<string,int>::emplace (reserve)", n, [&] {
    unordered_map<std::string, int> m;
    m.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
      m.emplace(keys[i], static_cast<int>(i));
    stl_bench::do_not_optimize(m.size());
  });

  stl_bench::run_samples("std::unordered_map<string,int>::emplace (reserve)", n, [&] {
    std::unordered_map<std::string, int> m;
    m.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
      m.emplace(keys[i], static_cast<int>(i));
    stl_bench::do_not_optimize(m.size());
  });
}
