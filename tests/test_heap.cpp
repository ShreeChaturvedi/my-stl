#include "test.hpp"

#include "heap/heap.hpp"
#include "vector/vector.hpp"

#include <algorithm>
#include <random>
#include <vector>

TEST_CASE("Heap: push/top/pop maintains order") {
  Heap<int> h;
  h.push(3);
  h.push(1);
  h.push(2);
  CHECK_EQ(h.top(), 1);
  CHECK_EQ(h.pop(), 1);
  CHECK_EQ(h.pop(), 2);
  CHECK_EQ(h.pop(), 3);
  CHECK(h.empty());
  CHECK_THROWS(h.pop());
}

TEST_CASE("Heap: heap sort matches std::sort") {
  Vector<int> xs;
  std::vector<int> ys;
  std::mt19937 rng(123);
  std::uniform_int_distribution<int> dist(-1000, 1000);
  for (int i = 0; i < 2000; ++i) {
    const int v = dist(rng);
    xs.push_back(v);
    ys.push_back(v);
  }

  std::sort(ys.begin(), ys.end());

  Heap<int>::sort(xs);
  CHECK_EQ(xs.size(), ys.size());
  for (std::size_t i = 0; i < ys.size(); ++i)
    CHECK_EQ(xs[i], ys[i]);
}
