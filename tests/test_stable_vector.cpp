#include "test.hpp"

#include "stable-vector/stable_vector.hpp"

#include <cstddef>
#include <string>

TEST_CASE("StableVector: push_back/insert keeps element addresses stable") {
  StableVector<int> xs;
  xs.reserve(2);
  xs.push_back(10);
  xs.push_back(20);

  int* p10 = &xs[0];
  int* p20 = &xs[1];

  xs.insert(xs.begin(), 5);
  CHECK_EQ(xs.size(), 3u);
  CHECK_EQ(xs[0], 5);
  CHECK_EQ(xs[1], 10);
  CHECK_EQ(xs[2], 20);
  CHECK_EQ(&xs[1], p10);
  CHECK_EQ(&xs[2], p20);

  xs.push_back(30);
  CHECK_EQ(xs.size(), 4u);
  CHECK_EQ(xs[3], 30);
  CHECK_EQ(&xs[1], p10);
  CHECK_EQ(&xs[2], p20);
}

TEST_CASE("StableVector: erase") {
  StableVector<int> xs{1, 2, 3, 4};
  xs.erase(xs.begin() + 1, xs.begin() + 3);
  CHECK_EQ(xs.size(), 2u);
  CHECK_EQ(xs[0], 1);
  CHECK_EQ(xs[1], 4);
}

TEST_CASE("StableVector: copy is deep") {
  StableVector<std::string> xs{"a", "b"};
  StableVector<std::string> ys = xs;
  CHECK_EQ(ys.size(), 2u);
  CHECK_EQ(ys[0], "a");
  CHECK_EQ(ys[1], "b");
  CHECK(&ys[0] != &xs[0]);
  CHECK(&ys[1] != &xs[1]);
}

