# Span<T>

A non-owning view over contiguous memory.

## Highlights

- Lightweight and trivially copyable.
- Works with raw pointers, arrays, and `Vector`.

## API Notes

- `subspan(offset, count)` returns a new view.
- `at`, `front`, `back` throw on invalid access.

## Complexity

- All operations are O(1).

## Differences vs `std::span`

- Minimal API (no extent parameter, no `first`/`last` helpers).
- Supports construction from `Vector` explicitly.

## Example

```cpp
#include "span/span.hpp"

int data[4] = {1, 2, 3, 4};
Span<int> view(data, 4);
auto tail = view.subspan(1);
```
