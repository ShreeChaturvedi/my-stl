# unordered_multimap<K, V>

A hash multimap with separate chaining and grouped duplicates per bucket.

## Highlights

- Allows multiple values per key.
- `count` and `equal_range` are supported.

## API Notes

- `insert` returns an iterator to the inserted element.
- `erase(key)` removes all matches and returns the count removed.

## Complexity

- Expected O(1) for single-key operations, worst-case O(n)

## Differences vs `std::unordered_multimap`

- Minimal API (no bucket interface or allocator template parameter).
- Iteration order is bucket-local and not specified.

## Example

```cpp
#include "unordered-multimap/unordered_multimap.hpp"

unordered_multimap<int, int> m;
m.insert({1, 10});
m.insert({1, 20});
auto [first, last] = m.equal_range(1);
```
