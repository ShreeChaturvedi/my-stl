#include "test.hpp"

#include "span/span.hpp"

TEST_CASE("Span: basic view and subspan") {
  int xs[] = {1, 2, 3, 4};
  Span<int> s(xs);
  CHECK_EQ(s.size(), 4u);
  CHECK_EQ(s.front(), 1);
  CHECK_EQ(s.back(), 4);
  CHECK_EQ(s[2], 3);

  auto mid = s.subspan(1, 2);
  CHECK_EQ(mid.size(), 2u);
  CHECK_EQ(mid[0], 2);
  CHECK_EQ(mid[1], 3);
}

TEST_CASE("Span: from Vector") {
  Vector<int> v;
  v.push_back(10);
  v.push_back(20);
  Span<const int> s(v);
  CHECK_EQ(s.size(), 2u);
  CHECK_EQ(s[1], 20);
}

