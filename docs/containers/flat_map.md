# FlatMap<K, V>

A sorted vector-backed map optimized for cache locality.

## Highlights

- Stores sorted key-value pairs in a `Vector`.
- Fast iteration and binary-search lookups.
- Optimized for sorted insert patterns (fast append path).

## API Notes

- `insert` / `emplace` return `{iterator, bool}`.
- `find` performs a binary search.
- `operator[]` inserts default values when missing.

## Complexity

- `find`: O(log n)
- `insert`, `erase`: O(n) due to shifts

## Differences vs `std::map`

- Backed by a flat vector (not a tree), so insert/erase are O(n).
- Minimal API (no hint insert, no node handles).

## Example

```cpp
#include "flat-map/flat_map.hpp"

FlatMap<int, int> m;
m.emplace(1, 10);
m.emplace(2, 20);
```
