#include "test.hpp"

#include "vector/vector.hpp"

#include <compare>
#include <string>

TEST_CASE("Vector: push_back/size/index") {
  Vector<int> xs;
  CHECK(xs.empty());
  for (int i = 0; i < 100; ++i)
    xs.push_back(i);
  CHECK_EQ(xs.size(), 100u);
  CHECK_EQ(xs[0], 0);
  CHECK_EQ(xs[99], 99);
}

TEST_CASE("Vector: front/back/pop_back/resize") {
  Vector<int> xs;
  CHECK_THROWS(xs.front());
  CHECK_THROWS(xs.back());
  CHECK_THROWS(xs.pop_back());

  xs.push_back(1);
  xs.push_back(2);
  CHECK_EQ(xs.front(), 1);
  CHECK_EQ(xs.back(), 2);
  xs.pop_back();
  CHECK_EQ(xs.size(), 1u);
  CHECK_EQ(xs.back(), 1);

  xs.resize(5);
  CHECK_EQ(xs.size(), 5u);
  CHECK_EQ(xs.front(), 1);
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

TEST_CASE("Vector: insert/erase") {
  Vector<int> xs{1, 3, 4};
  auto it = xs.insert(xs.begin() + 1, 2);
  CHECK_EQ(*it, 2);
  CHECK_EQ(xs.size(), 4u);
  CHECK_EQ(xs[0], 1);
  CHECK_EQ(xs[1], 2);
  CHECK_EQ(xs[2], 3);
  CHECK_EQ(xs[3], 4);

  it = xs.erase(xs.begin() + 2);
  CHECK_EQ(*it, 4);
  CHECK_EQ(xs.size(), 3u);
  CHECK_EQ(xs[0], 1);
  CHECK_EQ(xs[1], 2);
  CHECK_EQ(xs[2], 4);

  xs.insert(xs.begin(), 0);
  xs.insert(xs.end(), 5);
  CHECK_EQ(xs.size(), 5u);
  CHECK_EQ(xs.front(), 0);
  CHECK_EQ(xs.back(), 5);

  xs.erase(xs.begin() + 1, xs.begin() + 4);
  CHECK_EQ(xs.size(), 2u);
  CHECK_EQ(xs[0], 0);
  CHECK_EQ(xs[1], 5);
}

namespace {
struct MoveOnlyNoAssign {
  int value;

  explicit MoveOnlyNoAssign(int v) : value(v) {}
  MoveOnlyNoAssign(const MoveOnlyNoAssign&) = delete;
  MoveOnlyNoAssign& operator=(const MoveOnlyNoAssign&) = delete;
  MoveOnlyNoAssign(MoveOnlyNoAssign&&) noexcept = default;
  MoveOnlyNoAssign& operator=(MoveOnlyNoAssign&&) = delete;
};
} // namespace

TEST_CASE("Vector: insert/erase without assignment") {
  Vector<MoveOnlyNoAssign> xs;
  xs.emplace_back(1);
  xs.emplace_back(3);

  auto it = xs.emplace(xs.begin() + 1, 2);
  CHECK_EQ(it->value, 2);
  CHECK_EQ(xs.size(), 3u);
  CHECK_EQ(xs[0].value, 1);
  CHECK_EQ(xs[1].value, 2);
  CHECK_EQ(xs[2].value, 3);

  xs.erase(xs.begin() + 1);
  CHECK_EQ(xs.size(), 2u);
  CHECK_EQ(xs[0].value, 1);
  CHECK_EQ(xs[1].value, 3);

  CHECK_THROWS(xs.erase(xs.end()));
}

TEST_CASE("Vector: at throws") {
  CHECK_THROWS((Vector<int>{1, 2, 3}.at(99)));
}

TEST_CASE("Vector: shrink_to_fit after clear/pop") {
  Vector<int> xs;
  for (int i = 0; i < 32; ++i)
    xs.push_back(i);
  const auto grown = xs.capacity();
  CHECK(grown >= xs.size());

  xs.pop_back();
  xs.pop_back();
  CHECK_EQ(xs.capacity(), grown);
  xs.shrink_to_fit();
  CHECK_EQ(xs.capacity(), xs.size());
  CHECK_EQ(xs.size(), 30u);
  CHECK_EQ(xs.front(), 0);
  CHECK_EQ(xs.back(), 29);

  xs.clear();
  CHECK(xs.empty());
  // capacity may still be non-zero until shrink_to_fit
  const auto after_clear_cap = xs.capacity();
  CHECK(after_clear_cap >= xs.size());
  xs.shrink_to_fit();
  CHECK_EQ(xs.capacity(), 0u);
  CHECK(xs.data() == nullptr);
}

TEST_CASE("Vector: equality and three-way compare") {
  Vector<int> a{1, 2, 3};
  Vector<int> b{1, 2, 3};
  Vector<int> c{1, 2, 4};
  Vector<int> d{1, 2};
  Vector<int> e = a;

  CHECK(a == b);
  CHECK_FALSE(a != b);
  CHECK(a == e);
  CHECK(a != c);
  CHECK(a != d);

  a[1] = 9;
  CHECK(a != b);
  CHECK_FALSE(a == b);

  Vector<int> left{1, 2, 3};
  Vector<int> right{1, 2, 4};
  // Avoid Catch decomposing ordering <=> 0 (MSVC rejects that form).
  CHECK(std::is_lt(left <=> right));
  CHECK(std::is_gt(right <=> left));
  CHECK(std::is_eq(left <=> Vector<int>{1, 2, 3}));
  CHECK(std::is_lt(left <=> Vector<int>{1, 2, 3, 0}));
  CHECK(std::is_gt(Vector<int>{1, 2, 3, 0} <=> left));
}
