# StableVector<T>

A vector-like container that keeps element addresses stable by storing `unique_ptr<T>` slots.

## Highlights

- Pointer/reference stability across growth (elements are heap-allocated).
- Random-access iterators.

## API Notes

- `emplace_back` allocates a new `T` and stores it in a slot.
- `erase` removes slots and shifts the pointer array.

## Complexity

- `push_back`: amortized O(1)
- `insert`, `erase`: O(n)

## Differences vs `std::vector`

- Elements live on the heap; pointer stability comes at extra indirection.
- No allocator template parameter.

## Example

```cpp
#include "stable-vector/stable_vector.hpp"

StableVector<int> v;
v.emplace_back(1);
v.emplace_back(2);
```
