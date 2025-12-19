#include "test.hpp"

#include "array-list/array_list.hpp"

#include <string>

TEST_CASE("ArrayList: add/size/index") {
  ArrayList<int> xs;
  CHECK(xs.empty());
  CHECK(xs.size() == 0);
  for (int i = 0; i < 100; ++i)
    xs.add(i);
  CHECK(xs.size() == 100);
  CHECK_EQ(xs[0], 0);
  CHECK_EQ(xs[42], 42);
  CHECK_EQ(xs[99], 99);
}

TEST_CASE("ArrayList: remove/insert") {
  ArrayList<int> xs;
  for (int i = 0; i < 10; ++i)
    xs.add(i);
  CHECK_EQ(xs.remove(4), 4);
  CHECK_EQ(xs.size(), 9u);
  CHECK_EQ(xs[4], 5);

  xs.insert(4, 123);
  CHECK_EQ(xs.size(), 10u);
  CHECK_EQ(xs[4], 123);
  CHECK_EQ(xs[5], 5);
}

TEST_CASE("ArrayList: copy/move") {
  ArrayList<std::string> xs;
  xs.add("a");
  xs.add("b");

  ArrayList<std::string> copy = xs;
  CHECK_EQ(copy.size(), 2u);
  CHECK_EQ(copy[0], "a");

  ArrayList<std::string> moved = std::move(xs);
  CHECK_EQ(moved.size(), 2u);
  CHECK_EQ(moved[1], "b");
}

TEST_CASE("ArrayList: insert at end near capacity") {
  ArrayList<int> xs(16);
  for (int i = 0; i < 15; ++i)
    xs.add(i);
  xs.insert(xs.size(), 999);
  CHECK_EQ(xs.size(), 16u);
  CHECK_EQ(xs[15], 999);
}
