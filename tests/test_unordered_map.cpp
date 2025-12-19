#include "test.hpp"

#include "unordered-map/unordered_map.hpp"

#include <string>

TEST_CASE("unordered_map: insert/at/operator[]") {
  unordered_map<std::string, int> m;
  m.insert({"a", 1});
  m.emplace("b", 2);
  CHECK_EQ(m.size(), 2u);
  CHECK_EQ(m.at("a"), 1);
  CHECK_EQ(m["b"], 2);

  m["c"] = 3;
  CHECK_EQ(m.size(), 3u);
  CHECK_EQ(m.at("c"), 3);
}

TEST_CASE("unordered_map: erase") {
  unordered_map<int, int> m;
  for (int i = 0; i < 100; ++i)
    m.emplace(i, i * 10);
  CHECK_EQ(m.size(), 100u);
  m.erase(42);
  CHECK_EQ(m.size(), 99u);
  CHECK_THROWS(m.at(42));
}
