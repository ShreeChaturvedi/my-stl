#include "test.hpp"

#include "unordered-set/unordered_set.hpp"

TEST_CASE("unordered_set: insert/contains/erase") {
  unordered_set<int> s;
  CHECK(s.empty());

  auto [it1, ok1] = s.insert(1);
  CHECK(ok1);
  CHECK_EQ(*it1, 1);

  auto [it2, ok2] = s.insert(1);
  CHECK(!ok2);
  CHECK_EQ(*it2, 1);

  CHECK(s.contains(1));
  CHECK(!s.contains(2));

  CHECK(s.erase(1));
  CHECK(!s.contains(1));
  CHECK(!s.erase(1));
}

