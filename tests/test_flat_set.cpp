#include "test.hpp"

#include "flat-set/flat_set.hpp"

TEST_CASE("FlatSet: insert/find/iteration order") {
  FlatSet<int> s;
  CHECK(s.empty());

  auto [it1, inserted1] = s.insert(2);
  CHECK(inserted1);
  CHECK_EQ(*it1, 2);

  auto [it2, inserted2] = s.insert(1);
  CHECK(inserted2);
  CHECK_EQ(*it2, 1);

  auto [it3, inserted3] = s.insert(3);
  CHECK(inserted3);
  CHECK_EQ(s.size(), 3u);

  auto [it4, inserted4] = s.insert(2);
  CHECK(!inserted4);
  CHECK_EQ(*it4, 2);

  CHECK_EQ(*s.begin(), 1);
  CHECK_EQ(*(s.begin() + 1), 2);
  CHECK_EQ(*(s.begin() + 2), 3);

  CHECK(s.contains(3));
  CHECK(!s.contains(99));
}

TEST_CASE("FlatSet: erase") {
  FlatSet<int> s;
  s.insert(1);
  s.insert(2);
  s.insert(3);

  CHECK_EQ(s.erase(99), 0u);
  CHECK_EQ(s.erase(2), 1u);
  CHECK_EQ(s.size(), 2u);
  CHECK_EQ(*s.begin(), 1);
  CHECK_EQ(*(s.begin() + 1), 3);
}
