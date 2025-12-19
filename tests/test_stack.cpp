#include "test.hpp"

#include "stack/stack.hpp"

#include <string>

TEST_CASE("Stack: push/top/pop") {
  Stack<int> s;
  CHECK(s.empty());

  s.push(1);
  s.push(2);
  CHECK_EQ(s.size(), 2u);
  CHECK_EQ(s.top(), 2);

  s.pop();
  CHECK_EQ(s.top(), 1);
}

TEST_CASE("Stack: emplace") {
  Stack<std::string> s;
  s.emplace(3, 'x');
  CHECK_EQ(s.top(), "xxx");
}
