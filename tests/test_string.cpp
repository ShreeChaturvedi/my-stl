#include "test.hpp"

#include "string/string.hpp"

TEST_CASE("string: construction, append, c_str") {
  string s("hi");
  CHECK_EQ(s.size(), 2u);
  CHECK_EQ(s.view(), "hi");
  CHECK_EQ(std::string_view(s.c_str()), "hi");

  s += '!';
  CHECK_EQ(s.view(), "hi!");

  s.append(" there");
  CHECK_EQ(s.view(), "hi! there");
}

TEST_CASE("string: copy/move") {
  string a("abc");
  string b = a;
  CHECK_EQ(b.view(), "abc");

  string c = std::move(a);
  CHECK_EQ(c.view(), "abc");
}
