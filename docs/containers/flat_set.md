# FlatSet<K>

A sorted vector-backed set optimized for cache locality.

## Highlights

- Stores sorted keys in a `Vector`.
- Fast iteration and binary-search lookups.
- Optimized for sorted insert patterns (fast append path).

## API Notes

- `insert` returns `{iterator, bool}`.
- `find` performs a binary search.

## Complexity

- `find`: O(log n)
- `insert`, `erase`: O(n)

## Differences vs `std::set`

- Backed by a flat vector (not a tree), so insert/erase are O(n).
- Minimal API (no node handles, no hint insert).

## Example

```cpp
#include "flat-set/flat_set.hpp"

FlatSet<int> s;
s.insert(1);
s.insert(2);
```
