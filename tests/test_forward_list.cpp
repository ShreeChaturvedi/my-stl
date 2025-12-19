#include "test.hpp"

#include "forward-list/forward_list.hpp"

#include <string>

TEST_CASE("ForwardList: push_front/erase_after") {
  ForwardList<int> xs;
  CHECK(xs.empty());

  xs.push_front(1);
  xs.push_front(2);
  xs.push_front(3);

  CHECK_EQ(xs.front(), 3);

  auto before = xs.before_begin();
  xs.erase_after(before); // erase 3
  CHECK_EQ(xs.front(), 2);
}

TEST_CASE("ForwardList: insert_after") {
  ForwardList<int> xs;
  auto before = xs.before_begin();
  xs.insert_after(before, 1);
  xs.insert_after(xs.begin(), 2);
  CHECK_EQ(xs.front(), 1);
  CHECK_EQ(*xs.begin(), 1);
  CHECK_EQ(*(++xs.begin()), 2);
}

TEST_CASE("ForwardList: copy/move") {
  ForwardList<std::string> xs;
  xs.push_front("a");
  xs.push_front("b");

  ForwardList<std::string> ys(xs);
  CHECK(!ys.empty());
  CHECK_EQ(ys.front(), "b");

  ForwardList<std::string> zs(std::move(xs));
  CHECK_EQ(zs.front(), "b");
}
