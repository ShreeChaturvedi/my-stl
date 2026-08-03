# Span<T>

A non-owning, bounds-aware view over contiguous memory.

## Highlights

- Lightweight and trivially copyable.
- Works with raw pointers, arrays, and `Vector`.

## API Notes

- `subspan(offset, count)` returns a new view.
- `first(n)` / `last(n)` return a prefix/suffix view; throw if `n > size()`.
- `size_bytes()` is `size() * sizeof(T)`.
- `at`, `front`, `back` throw on invalid access.

## Complexity

- All operations are O(1).

## Differences vs `std::span`

- Minimal API surface (no extent parameter / fixed extent).
- Bounds errors on `first`/`last`/`subspan` throw `std::out_of_range` rather than relying on a precondition-only contract.
- Supports construction from `Vector` explicitly.

## Example

```cpp
#include "span/span.hpp"

int data[4] = {1, 2, 3, 4};
Span<int> view(data, 4);
auto tail = view.subspan(1);
```
