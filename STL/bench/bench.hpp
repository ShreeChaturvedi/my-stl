#pragma once

#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string_view>

namespace stl_bench {

using clock = std::chrono::steady_clock;

template <typename Fn>
inline std::chrono::nanoseconds time_it(Fn&& fn) {
  const auto start = clock::now();
  std::forward<Fn>(fn)();
  const auto end = clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

inline void report(std::string_view name, std::size_t n, std::chrono::nanoseconds ns) {
  const double ns_per_op = n == 0 ? 0.0 : static_cast<double>(ns.count()) / static_cast<double>(n);
  std::cout << name << ": " << ns.count() << " ns total (" << ns_per_op << " ns/op, n=" << n << ")\n";
}

}  // namespace stl_bench

