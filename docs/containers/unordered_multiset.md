# unordered_multiset<K>

A hash multiset built on top of `unordered_multimap<K, unit>`.

## Highlights

- Allows duplicate keys.
- `count` returns the number of matches.

## API Notes

- `insert` returns an iterator to one inserted element.
- `erase(key)` removes all matches and returns the count removed.

## Complexity

- Expected O(1) for single-key operations, worst-case O(n)

## Differences vs `std::unordered_multiset`

- Minimal API (no bucket interface, no allocator template parameter).

## Example

```cpp
#include "unordered-multiset/unordered_multiset.hpp"

unordered_multiset<int> s;
s.insert(1);
s.insert(1);
auto count = s.count(1);
```
