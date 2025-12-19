# Vector<T>

A contiguous, dynamically sized array with manual element lifetime management via `std::allocator<T>`.

## Highlights

- Contiguous storage and pointer-like iterators.
- Growth factor ~1.5x with special-casing for tiny capacities.
- `emplace_back`, `insert`, `erase`, `reserve`, `resize` supported.

## API Notes

- Constructors: default, capacity, initializer list, copy, move.
- Access: `operator[]` (unchecked), `at` (throws), `front`, `back`, `data`.
- Modifiers: `push_back`, `emplace_back`, `insert`, `erase`, `clear`, `reserve`, `resize`.

## Complexity

- `push_back`, `emplace_back`: amortized O(1)
- `insert`, `erase`: O(n)
- `reserve`, `resize`: O(n)
- `operator[]`: O(1), `at`: O(1) + bounds check

## Iterator / Reference Invalidation

- Reallocation invalidates all iterators, references, and pointers.
- `insert` invalidates iterators at or after the insertion point.
- `erase` invalidates iterators at or after the erased range.

## Differences vs `std::vector`

- No allocator template parameter (uses `std::allocator<T>` internally).
- `resize` only supports default construction and requires `T` to be default constructible.
- `front`, `back`, and `pop_back` throw on empty.
- No `shrink_to_fit` or capacity growth tuning beyond `reserve`.

## Example

```cpp
#include "vector/vector.hpp"

Vector<int> v;
v.push_back(1);
v.emplace_back(2);
v.insert(v.begin() + 1, 42);
v.erase(v.begin());
```
