#include "test.hpp"

#include "small-vector/small_vector.hpp"

#include <string>

TEST_CASE("SmallVector: inline growth and accessors") {
  SmallVector<int, 4> xs;
  CHECK(xs.empty());
  CHECK(xs.using_inline_storage());
  CHECK_EQ(xs.capacity(), 4u);

  for (int i = 0; i < 4; ++i)
    xs.push_back(i);
  CHECK(xs.using_inline_storage());
  CHECK_EQ(xs.size(), 4u);
  CHECK_EQ(xs.front(), 0);
  CHECK_EQ(xs.back(), 3);

  xs.push_back(4);
  CHECK_EQ(xs.size(), 5u);
  CHECK(!xs.using_inline_storage());
  CHECK(xs.capacity() >= 5u);
  CHECK_EQ(xs[4], 4);
}

TEST_CASE("SmallVector: insert/erase") {
  SmallVector<int, 4> xs{1, 3, 4};
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
  CHECK_EQ(xs[2], 4);

  xs.erase(xs.begin(), xs.end());
  CHECK(xs.empty());
}

TEST_CASE("SmallVector: copy/move") {
  SmallVector<std::string, 4> xs{"a", "b", "c"};
  SmallVector<std::string, 4> ys = xs;
  CHECK_EQ(ys.size(), 3u);
  CHECK_EQ(ys[1], "b");

  SmallVector<std::string, 4> zs = std::move(xs);
  CHECK_EQ(zs.size(), 3u);
  CHECK_EQ(zs[2], "c");
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

TEST_CASE("SmallVector: insert/erase without assignment") {
  SmallVector<MoveOnlyNoAssign, 4> xs;
  xs.emplace_back(1);
  xs.emplace_back(3);

  auto it = xs.emplace(xs.begin() + 1, 2);
  CHECK_EQ(it->value, 2);
  CHECK_EQ(xs.size(), 3u);

  xs.erase(xs.begin() + 1);
  CHECK_EQ(xs.size(), 2u);
  CHECK_EQ(xs[1].value, 3);
}
