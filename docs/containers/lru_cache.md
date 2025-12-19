# LRUCache<K, V>

An LRU (least-recently-used) cache with O(1) expected access.

## Highlights

- `List<K>` stores recency order.
- `unordered_map<K, {V, iterator}>` provides O(1) lookup.

## API Notes

- `get(key)` returns `std::optional<V>` and updates recency.
- `insert(key, value)` inserts and evicts the least-recently-used entry if full.
- Capacity is fixed at construction time.

## Complexity

- Expected O(1) `get` and `insert`

## Differences vs typical cache libraries

- Minimal API and deterministic eviction policy.
- Uses my-stl containers internally.

## Example

```cpp
#include "lru-cache/lru_cache.hpp"

LRUCache<int, int> cache(2);
cache.insert(1, 10);
cache.insert(2, 20);
auto v = cache.get(1);
```
