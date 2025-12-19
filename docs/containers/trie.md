# Trie

A byte-oriented radix tree for string keys.

## Highlights

- 256-way branching per node (byte-level keys).
- `insert`, `contains`, and `erase` in O(L) where L is key length.

## API Notes

- Keys are `std::string_view`.
- `erase` removes keys and prunes nodes when possible.

## Complexity

- `insert`, `contains`, `erase`: O(L)

## Differences vs compressed tries

- Not a compressed trie; trades memory for simplicity.

## Example

```cpp
#include "trie/trie.hpp"

Trie t;
t.insert("cat");
bool ok = t.contains("cat");
```
