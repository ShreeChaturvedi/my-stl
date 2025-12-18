#pragma once

#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace stl_bench {

using clock = std::chrono::steady_clock;

struct BenchCase final {
  std::string_view name;
  void (*fn)(std::size_t n);
};

inline std::vector<BenchCase>& registry() {
  static std::vector<BenchCase> cases;
  return cases;
}

struct Registrar final {
  explicit Registrar(std::string_view name, void (*fn)(std::size_t n)) { registry().push_back({name, fn}); }
};

template <typename T>
inline void do_not_optimize(T&& value) {
#if defined(__clang__) || defined(__GNUC__)
  asm volatile("" : : "g"(value) : "memory");
#else
  (void)value;
#endif
}

inline void clobber_memory() {
#if defined(__clang__) || defined(__GNUC__)
  asm volatile("" : : : "memory");
#endif
}

template <typename Fn>
inline std::chrono::nanoseconds time_it(Fn&& fn) {
  do_not_optimize(0);
  clobber_memory();
  const auto start = clock::now();
  std::forward<Fn>(fn)();
  const auto end = clock::now();
  clobber_memory();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

inline void report(std::string_view name, std::size_t n, std::chrono::nanoseconds ns) {
  const double ns_per_op = n == 0 ? 0.0 : static_cast<double>(ns.count()) / static_cast<double>(n);
  std::cout << name << ": " << ns.count() << " ns total (" << ns_per_op << " ns/op, n=" << n << ")\n";
}

}  // namespace stl_bench

#define STL_BENCH_CONCAT_INNER(a, b) a##b
#define STL_BENCH_CONCAT(a, b) STL_BENCH_CONCAT_INNER(a, b)

#define BENCH_CASE(name_literal)                                                                     \
  static void STL_BENCH_CONCAT(bench_fn_, __LINE__)(std::size_t n);                                   \
  static ::stl_bench::Registrar STL_BENCH_CONCAT(bench_registrar_, __LINE__)(                         \
      name_literal, &STL_BENCH_CONCAT(bench_fn_, __LINE__));                                          \
  static void STL_BENCH_CONCAT(bench_fn_, __LINE__)(std::size_t n)
