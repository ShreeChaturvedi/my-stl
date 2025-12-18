#include "test.hpp"

#include "../linked-list/list.h"

TEST_CASE("LinkedList: append/index/size") {
  LinkedList<int> xs;
  CHECK_EQ(xs.size(), 0u);
  for (int i = 0; i < 5; ++i) xs.append(i);
  CHECK_EQ(xs.size(), 5u);
  CHECK_EQ(xs[0], 0);
  CHECK_EQ(xs[4], 4);
}

TEST_CASE("LinkedList: insert/remove") {
  LinkedList<int> xs;
  xs.append(1);
  xs.append(3);

  xs.insert(2, 1);
  CHECK_EQ(xs.size(), 3u);
  CHECK_EQ(xs[1], 2);

  xs.remove(2);
  CHECK_EQ(xs.size(), 2u);
  CHECK_EQ(xs[1], 2);
}

