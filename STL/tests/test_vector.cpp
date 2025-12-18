#include "test.hpp"

#include "../vector/vector.hpp"

#include <string>

TEST_CASE("Vector: push_back/size/index") {
  Vector<int> xs;
  CHECK(xs.empty());
  for (int i = 0; i < 100; ++i) xs.push_back(i);
  CHECK_EQ(xs.size(), 100u);
  CHECK_EQ(xs[0], 0);
  CHECK_EQ(xs[99], 99);
}

TEST_CASE("Vector: copy/move") {
  Vector<std::string> xs{"a", "b", "c"};
  Vector<std::string> ys = xs;
  CHECK_EQ(ys.size(), 3u);
  CHECK_EQ(ys[1], "b");

  Vector<std::string> zs = std::move(xs);
  CHECK_EQ(zs.size(), 3u);
  CHECK_EQ(zs[2], "c");
}

TEST_CASE("Vector: at throws") { CHECK_THROWS((Vector<int>{1, 2, 3}.at(99))); }

