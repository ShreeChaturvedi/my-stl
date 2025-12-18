#include "test.hpp"

#include "deque/deque.hpp"

TEST_CASE("Deque: push/pop front/back and indexing") {
  Deque<int> d;
  CHECK(d.empty());
  CHECK_THROWS(d.pop_front());

  d.push_back(2);
  d.push_front(1);
  d.push_back(3);

  CHECK_EQ(d.size(), 3u);
  CHECK_EQ(d.front(), 1);
  CHECK_EQ(d.back(), 3);
  CHECK_EQ(d[1], 2);

  d.pop_front();
  CHECK_EQ(d.front(), 2);
  d.pop_back();
  CHECK_EQ(d.back(), 2);
  CHECK_EQ(d.size(), 1u);
}

TEST_CASE("Deque: wraparound growth") {
  Deque<int> d;
  for (int i = 0; i < 50; ++i) d.push_back(i);
  for (int i = 0; i < 25; ++i) d.pop_front();
  for (int i = 0; i < 50; ++i) d.push_front(1000 + i);
  CHECK_EQ(d.size(), 75u);
  CHECK_EQ(d.back(), 49);
}

