#include "test.hpp"

#include "unordered-multimap/unordered_multimap.hpp"
#include "unordered-multiset/unordered_multiset.hpp"

TEST_CASE("unordered_multimap: duplicates and equal_range") {
  unordered_multimap<int, int> m;
  m.insert({1, 10});
  m.insert({1, 20});
  m.insert({2, 30});
  CHECK_EQ(m.count(1), 2u);

  auto [first, last] = m.equal_range(1);
  int sum = 0;
  for (auto it = first; it != last; ++it) sum += it->second;
  CHECK_EQ(sum, 30);

  CHECK_EQ(m.erase(1), 2u);
  CHECK_EQ(m.count(1), 0u);
}

TEST_CASE("unordered_multiset: duplicates") {
  unordered_multiset<int> s;
  s.insert(1);
  s.insert(1);
  s.insert(2);
  CHECK_EQ(s.count(1), 2u);
  CHECK_EQ(s.erase(1), 2u);
  CHECK_EQ(s.count(1), 0u);
}

