# Heap<T, Compare, branches>

A d-ary heap backed by `Vector<T>` (default: binary heap).

## Highlights

- Configurable arity via `branches` (>= 2).
- `Compare` controls min/max behavior (`std::less` by default).
- Supports heap construction from ranges and initializer lists.

## API Notes

- `top()` returns the highest-priority element.
- `pop()` removes and returns the highest-priority element.
- `sort(Vector<T>&)` performs an in-place heap sort.

## Complexity

- `push`, `pop`: O(log_base(branches) n)
- `top`: O(1)

## Differences vs `std::priority_queue`

- Exposes iterators over the heap storage.
- Provides `sort` helper and height/formatting utilities.
- Uses `Vector<T>` as the backing store (header-only).

## Example

```cpp
#include "heap/heap.hpp"

Heap<int> h;
h.push(3);
h.push(1);
int top = h.top();
int popped = h.pop();
```
