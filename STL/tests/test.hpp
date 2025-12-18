#pragma once

#include <cstddef>
#include <exception>
#include <functional>
#include <iostream>
#include <source_location>
#include <string_view>
#include <utility>
#include <vector>

namespace stl_test {

struct Failure final : std::exception {
  const char* what() const noexcept override { return "test failure"; }
};

struct TestCase final {
  std::string_view name;
  void (*fn)();
};

inline std::vector<TestCase>& registry() {
  static std::vector<TestCase> cases;
  return cases;
}

struct Registrar final {
  explicit Registrar(std::string_view name, void (*fn)()) { registry().push_back({name, fn}); }
};

inline void report_failure(std::string_view expr,
                           const std::source_location& where = std::source_location::current()) {
  std::cerr << where.file_name() << ":" << where.line() << ": CHECK failed: " << expr << "\n";
  throw Failure{};
}

template <typename A, typename B>
inline void report_failure_eq(const A& a,
                              const B& b,
                              std::string_view a_expr,
                              std::string_view b_expr,
                              const std::source_location& where = std::source_location::current()) {
  std::cerr << where.file_name() << ":" << where.line() << ": CHECK_EQ failed: " << a_expr << " vs "
            << b_expr << "\n";
  std::cerr << "  lhs: " << a << "\n";
  std::cerr << "  rhs: " << b << "\n";
  throw Failure{};
}

}  // namespace stl_test

#define STL_TEST_CONCAT_INNER(a, b) a##b
#define STL_TEST_CONCAT(a, b) STL_TEST_CONCAT_INNER(a, b)

#define TEST_CASE(name_literal)                                                                     \
  static void STL_TEST_CONCAT(test_fn_, __LINE__)();                                                 \
  static ::stl_test::Registrar STL_TEST_CONCAT(test_registrar_, __LINE__)(                           \
      name_literal, &STL_TEST_CONCAT(test_fn_, __LINE__));                                           \
  static void STL_TEST_CONCAT(test_fn_, __LINE__)()

#define CHECK(expr)                                                                                  \
  do {                                                                                               \
    if (!(expr)) ::stl_test::report_failure(#expr);                                                   \
  } while (0)

#define REQUIRE(expr) CHECK(expr)

#define CHECK_EQ(a, b)                                                                               \
  do {                                                                                               \
    const auto& stl_test_a = (a);                                                                    \
    const auto& stl_test_b = (b);                                                                    \
    if (!(stl_test_a == stl_test_b)) {                                                               \
      ::stl_test::report_failure_eq(stl_test_a, stl_test_b, #a, #b);                                  \
    }                                                                                                \
  } while (0)

#define CHECK_THROWS(expr)                                                                           \
  do {                                                                                               \
    bool stl_test_thrown = false;                                                                    \
    try {                                                                                            \
      (void)(expr);                                                                                  \
    } catch (...) {                                                                                  \
      stl_test_thrown = true;                                                                        \
    }                                                                                                \
    if (!stl_test_thrown) ::stl_test::report_failure("Expected exception: " #expr);                  \
  } while (0)

#define CHECK_NOTHROW(expr)                                                                          \
  do {                                                                                               \
    try {                                                                                            \
      (void)(expr);                                                                                  \
    } catch (...) {                                                                                  \
      ::stl_test::report_failure("Unexpected exception: " #expr);                                    \
    }                                                                                                \
  } while (0)

