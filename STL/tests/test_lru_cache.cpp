#include "test.hpp"

#include "../lru-cache/lru_cache.hpp"

TEST_CASE("LRUCache: insert/get/eviction") {
  LRUCache<int, int> c(2);
  CHECK(c.insert(1, 10));
  CHECK(c.insert(2, 20));
  CHECK_EQ(c.size(), 2u);

  auto v = c.get(1);
  CHECK(v.has_value());
  CHECK_EQ(v->get(), 10);

  CHECK(c.insert(3, 30));  // evicts key 2
  CHECK(!c.get(2).has_value());
  CHECK(c.get(1).has_value());
  CHECK(c.get(3).has_value());
}
