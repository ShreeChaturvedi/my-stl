#include "test.hpp"

#include "map/map.hpp"
#include "set/set.hpp"

#include <string>

TEST_CASE("map: insert/find/operator[]/erase") {
  map<std::string, int> m;
  CHECK(m.empty());

  auto [it, inserted] = m.insert({"a", 1});
  CHECK(inserted);
  CHECK_EQ(it->second, 1);

  CHECK_EQ(m["b"], 0);
  m["b"] = 2;
  CHECK_EQ(m.at("b"), 2);

  m.erase("a");
  CHECK(!m.contains("a"));
  CHECK(m.contains("b"));
}

TEST_CASE("multimap: duplicates and erase_all") {
  multimap<int, int> m;
  m.insert({1, 10});
  m.insert({1, 20});
  m.insert({2, 30});

  auto [first, last] = m.equal_range(1);
  int sum = 0;
  for (auto it = first; it != last; ++it)
    sum += it->second;
  CHECK_EQ(sum, 30);

  CHECK_EQ(m.erase_all(1), 2u);
  CHECK(m.find(1) == m.end());
}

TEST_CASE("set: uniqueness") {
  set<int> s;
  auto [it1, ok1] = s.insert(1);
  auto [it2, ok2] = s.insert(1);
  CHECK(ok1);
  CHECK(!ok2);
  CHECK_EQ(*it1, 1);
  CHECK_EQ(*it2, 1);
  CHECK_EQ(s.size(), 1u);
}

TEST_CASE("multiset: duplicates") {
  multiset<int> s;
  s.insert(1);
  s.insert(1);
  s.insert(2);

  auto [first, last] = s.equal_range(1);
  std::size_t count = 0;
  for (auto it = first; it != last; ++it)
    ++count;
  CHECK_EQ(count, 2u);

  CHECK_EQ(s.erase_all(1), 2u);
}
