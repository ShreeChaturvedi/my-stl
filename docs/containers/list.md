# List<T>

A doubly linked list with a sentinel node and stable iterators.

## Highlights

- O(1) `push_front`, `push_back`, `pop_front`, `pop_back`.
- Bidirectional iterators.
- `move_to_front` supports LRU-style data structures.

## API Notes

- `erase(iterator)` removes a single element in O(1).
- `move_to_front(iterator)` relinks an existing node to the front.

## Complexity

- `push_front`, `push_back`, `pop_front`, `pop_back`, `erase`: O(1)
- Traversal: O(n)

## Differences vs `std::list`

- No allocator template parameter.
- Minimal API (no splice/merge/sort).

## Example

```cpp
#include "list/list.hpp"

List<int> items;
auto it = items.push_back(10);
items.push_front(5);
items.move_to_front(it);
```
