#include "test.hpp"

#include "list/list.hpp"

TEST_CASE("List: push_front/push_back/iteration") {
  List<int> xs;
  CHECK(xs.empty());

  xs.push_front(2);
  xs.push_front(1);
  xs.push_back(3);

  CHECK_EQ(xs.size(), 3u);
  CHECK_EQ(xs.front(), 1);
  CHECK_EQ(xs.back(), 3);

  int sum = 0;
  for (int v : xs)
    sum += v;
  CHECK_EQ(sum, 6);
}

TEST_CASE("List: move_to_front") {
  List<int> xs;
  xs.push_back(1);
  auto it = xs.push_back(2);
  xs.push_back(3);

  xs.move_to_front(it);
  CHECK_EQ(xs.front(), 2);
  CHECK_EQ(xs.back(), 3);
}
