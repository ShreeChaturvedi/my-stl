#pragma once

#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace stl_bench {

using clock = std::chrono::steady_clock;

struct Config final {
  std::size_t iters = 5;
  std::size_t warmup = 1;
};

inline Config& config() {
  static Config cfg;
  return cfg;
}

struct BenchCase final {
  std::string_view name;
  void (*fn)(std::size_t n);
};

inline std::vector<BenchCase>& registry() {
  static std::vector<BenchCase> cases;
  return cases;
}

struct Registrar final {
  explicit Registrar(std::string_view name, void (*fn)(std::size_t n)) {
    registry().push_back({name, fn});
  }
};

template <typename T> inline void do_not_optimize(T&& value) {
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

template <typename Fn> inline std::chrono::nanoseconds time_it(Fn&& fn) {
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
  std::cout << name << ": " << ns.count() << " ns total (" << ns_per_op << " ns/op, n=" << n
            << ")\n";
}

inline void report_samples(std::string_view name, std::size_t n,
                           std::vector<std::chrono::nanoseconds> samples) {
  if (samples.empty())
    return;
  std::sort(samples.begin(), samples.end());

  const auto min = samples.front();
  const auto median = samples[samples.size() / 2];
  const auto max = samples.back();

  const auto total = std::accumulate(samples.begin(), samples.end(), std::chrono::nanoseconds{0});
  const auto mean = total / static_cast<std::int64_t>(samples.size());

  const auto fmt = [&](std::chrono::nanoseconds ns) {
    const double ns_per_op =
        n == 0 ? 0.0 : static_cast<double>(ns.count()) / static_cast<double>(n);
    std::cout << ns.count() << " ns (" << ns_per_op << " ns/op)";
  };

  std::cout << name << " [iters=" << samples.size() << ", n=" << n << "]: ";
  std::cout << "min=";
  fmt(min);
  std::cout << ", median=";
  fmt(median);
  std::cout << ", mean=";
  fmt(mean);
  std::cout << ", max=";
  fmt(max);
  std::cout << "\n";
}

template <typename Fn> inline void run_samples(std::string_view name, std::size_t n, Fn&& fn) {
  for (std::size_t i = 0; i < config().warmup; ++i)
    std::forward<Fn>(fn)();

  std::vector<std::chrono::nanoseconds> samples;
  samples.reserve(config().iters);
  for (std::size_t i = 0; i < config().iters; ++i)
    samples.push_back(time_it(fn));

  report_samples(name, n, std::move(samples));
}

} // namespace stl_bench

#define STL_BENCH_CONCAT_INNER(a, b) a##b
#define STL_BENCH_CONCAT(a, b) STL_BENCH_CONCAT_INNER(a, b)

#define BENCH_CASE(name_literal)                                                                   \
  static void STL_BENCH_CONCAT(bench_fn_, __LINE__)(std::size_t n);                                \
  static ::stl_bench::Registrar STL_BENCH_CONCAT(bench_registrar_, __LINE__)(                      \
      name_literal, &STL_BENCH_CONCAT(bench_fn_, __LINE__));                                       \
  static void STL_BENCH_CONCAT(bench_fn_, __LINE__)(std::size_t n)
