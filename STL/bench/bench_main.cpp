#include "bench.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string_view>

namespace {

std::size_t parse_n(std::string_view s) {
  std::size_t n = 0;
  for (char c : s) {
    if (c < '0' || c > '9') return 0;
    n = n * 10 + static_cast<std::size_t>(c - '0');
  }
  return n;
}

}  // namespace

int main(int argc, char** argv) {
  std::size_t n = 100'000;
  std::string_view filter;
  bool list_only = false;

  for (int i = 1; i < argc; ++i) {
    const std::string_view arg = argv[i];
    if (arg == "--list") {
      list_only = true;
    } else if (arg.starts_with("--filter=")) {
      filter = arg.substr(std::string_view("--filter=").size());
    } else if (arg == "--filter" && i + 1 < argc) {
      filter = argv[++i];
    } else if (arg.starts_with("--n=")) {
      const auto value = arg.substr(std::string_view("--n=").size());
      const auto parsed = parse_n(value);
      if (parsed != 0) n = parsed;
    } else if (arg == "--n" && i + 1 < argc) {
      const auto parsed = parse_n(argv[++i]);
      if (parsed != 0) n = parsed;
    } else {
      std::cerr << "Usage: " << argv[0] << " [--n N] [--filter substr] [--list]\n";
      return EXIT_FAILURE;
    }
  }

  auto& cases = stl_bench::registry();
  std::sort(cases.begin(), cases.end(), [](const auto& a, const auto& b) { return a.name < b.name; });

  if (list_only) {
    for (const auto& c : cases) std::cout << c.name << "\n";
    return EXIT_SUCCESS;
  }

  std::size_t ran = 0;
  for (const auto& c : cases) {
    if (!filter.empty() && c.name.find(filter) == std::string_view::npos) continue;
    c.fn(n);
    ++ran;
  }

  if (ran == 0) {
    std::cerr << "No benchmarks matched.\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
