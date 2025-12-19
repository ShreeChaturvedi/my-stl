# LinkedList<T>

A simple singly linked list with a sentinel head and cached tail pointer.

## Highlights

- `append` is O(1) thanks to a cached tail (`end_`).
- Index-based `insert`/`remove`/`operator[]` are supported.

## API Notes

- `append(value)` returns the index where the element was inserted.
- `insert(value, index)` inserts at index (O(n)).
- `remove(index)` removes at index (O(n)) and returns the removed value.

## Complexity

- `append`: O(1)
- `insert`, `remove`, `operator[]`: O(n)

## Differences vs `std::forward_list`

- Uses index-based operations instead of iterators.
- No allocator template parameter.
- Intended as an educational linked list baseline.

## Example

```cpp
#include "linked-list/linked_list.hpp"

LinkedList<int> list;
list.append(1);
list.insert(2, 1);
int x = list[0];
```
