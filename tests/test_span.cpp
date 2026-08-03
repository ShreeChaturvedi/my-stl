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

TEST_CASE("Span: first/last/size_bytes") {
  int xs[] = {1, 2, 3, 4, 5};
  Span<int> s(xs);

  CHECK_EQ(s.size_bytes(), 5u * sizeof(int));

  auto head = s.first(3);
  CHECK_EQ(head.size(), 3u);
  CHECK_EQ(head[0], 1);
  CHECK_EQ(head[2], 3);
  CHECK_EQ(head.size_bytes(), 3u * sizeof(int));

  auto tail = s.last(2);
  CHECK_EQ(tail.size(), 2u);
  CHECK_EQ(tail[0], 4);
  CHECK_EQ(tail[1], 5);

  auto empty_head = s.first(0);
  CHECK(empty_head.empty());
  CHECK_EQ(empty_head.size_bytes(), 0u);
  CHECK(empty_head.data() == s.data());

  auto empty_tail = s.last(0);
  CHECK(empty_tail.empty());
  CHECK_EQ(empty_tail.size_bytes(), 0u);

  auto full = s.first(s.size());
  CHECK_EQ(full.size(), s.size());
  CHECK_EQ(full[4], 5);
  CHECK_EQ(s.last(s.size()).front(), 1);

  CHECK_THROWS(s.first(6));
  CHECK_THROWS(s.last(6));
  CHECK_THROWS(s.first(100));
  CHECK_THROWS(s.last(100));
}

TEST_CASE("Span: first/last on empty") {
  Span<int> empty;
  CHECK(empty.empty());
  CHECK_EQ(empty.size_bytes(), 0u);
  CHECK_EQ(empty.first(0).size(), 0u);
  CHECK_EQ(empty.last(0).size(), 0u);
  CHECK_THROWS(empty.first(1));
  CHECK_THROWS(empty.last(1));
}
