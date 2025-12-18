#include "test.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

int main() {
  std::size_t failures = 0;

  for (const auto& tc : stl_test::registry()) {
    try {
      tc.fn();
    } catch (const stl_test::Failure&) {
      ++failures;
      std::cerr << "FAILED: " << tc.name << "\n";
    } catch (const std::exception& e) {
      ++failures;
      std::cerr << "FAILED (std::exception): " << tc.name << ": " << e.what() << "\n";
    } catch (...) {
      ++failures;
      std::cerr << "FAILED (unknown): " << tc.name << "\n";
    }
  }

  if (failures == 0) {
    std::cout << "OK (" << stl_test::registry().size() << " tests)\n";
    return EXIT_SUCCESS;
  }

  std::cerr << failures << " failing test(s)\n";
  return EXIT_FAILURE;
}

