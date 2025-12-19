# ArrayList<T>

A simple, educational dynamic array backed by `new[]` and manual growth.

## Highlights

- Minimal API (`add`, `insert`, `remove`) for learning purposes.
- Default capacity of 16.
- Not allocator-aware; intended as a baseline before `Vector`.

## API Notes

- `add(const T&)` appends.
- `insert(index, value)` inserts and shifts.
- `remove(index)` removes and returns the element by value.
- `operator[]` provides unchecked access.
- `reserve(capacity)` grows the backing array.

## Complexity

- `add`: amortized O(1)
- `insert`, `remove`: O(n)
- `operator[]`: O(1)

## Differences vs `std::vector`

- No iterators or range-based insert/erase.
- No bounds-checked accessor (`at`).
- Uses raw `new[]` / `delete[]` and copies elements on growth.
- Primarily a teaching container; prefer `Vector<T>` for real use.

## Example

```cpp
#include "array-list/array_list.hpp"

ArrayList<int> xs;
xs.add(1);
xs.add(2);
xs.insert(1, 42);
int removed = xs.remove(0);
```
