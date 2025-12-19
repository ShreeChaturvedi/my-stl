#include "test.hpp"

#include "flat-map/flat_map.hpp"

#include <string>

TEST_CASE("FlatMap: insert/find/iteration order") {
  FlatMap<int, std::string> m;
  CHECK(m.empty());

  auto [it1, inserted1] = m.insert({2, "b"});
  CHECK(inserted1);
  CHECK_EQ(it1->first, 2);
  CHECK_EQ(it1->second, "b");

  auto [it2, inserted2] = m.insert({1, "a"});
  CHECK(inserted2);
  CHECK_EQ(it2->first, 1);
  CHECK_EQ(m.size(), 2u);

  auto [it3, inserted3] = m.insert({3, "c"});
  CHECK(inserted3);
  CHECK_EQ(m.size(), 3u);

  auto [it4, inserted4] = m.insert({2, "bb"});
  CHECK(!inserted4);
  CHECK_EQ(it4->second, "b");

  CHECK_EQ(m.begin()->first, 1);
  CHECK_EQ((m.begin() + 1)->first, 2);
  CHECK_EQ((m.begin() + 2)->first, 3);

  auto it = m.find(2);
  CHECK(it != m.end());
  CHECK_EQ(it->second, "b");
  CHECK(m.contains(1));
  CHECK(!m.contains(99));
}

TEST_CASE("FlatMap: operator[]/at/erase") {
  FlatMap<int, std::string> m;
  CHECK_THROWS(m.at(1));

  m[3] = "c";
  CHECK_EQ(m.at(3), "c");

  (void)m[2];
  CHECK_EQ(m.size(), 2u);
  CHECK_EQ(m.at(2), "");

  CHECK_EQ(m.erase(99), 0u);
  CHECK_EQ(m.erase(2), 1u);
  CHECK_EQ(m.size(), 1u);

  CHECK_THROWS(m.at(2));
}
