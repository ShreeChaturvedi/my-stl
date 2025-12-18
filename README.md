# mini-stl

A modern C++ (C++23) “build-your-own-STL” project: hand-rolled containers, RAII utilities, tests, and benchmarks.

## Containers

- `array-list` — `ArrayList<T>`
- `forward-list` — `ForwardList<T>`
- `linked-list` — `LinkedList<T>`
- `list` — `List<T>`
- `queue` — `Queue<T>`
- `ring-buffer` — `RingBuffer<T, capacity>`
- `heap` — `Heap<T, compare, branches>`
- `priority-queue` — `PriorityQueue<T, Compare, branches>`
- `stack` — `Stack<T>`
- `vector` — `Vector<T>`
- `unordered-map` — `unordered_map<K, V>`
- `unique-ptr` — `unique_ptr<T>`
- `trie` — `Trie`
- `lru-cache` — `LRUCache<K, V>`

## Design Notes

- Header-only: everything is `*.hpp` + `*.tpp` and included via CMake include paths.
- Self-hosting (where it makes sense): containers use other mini-stl containers for their internal storage.
  - `Heap` uses `Vector`
  - `unordered_map` uses `Vector` + `ForwardList`
  - `LRUCache` uses `List` + `unordered_map`
  - `Stack` uses `Vector`, `Queue` uses `List`, `PriorityQueue` uses `Heap`

## Roadmap

STL containers not implemented yet:

- Sequence: `deque`, `span`, `basic_string`
- Associative: `map`, `set`, `multimap`, `multiset`
- Unordered: `unordered_set`, `unordered_multimap`, `unordered_multiset`

Common “Boost-style” containers worth adding:

- `small_vector` (inline storage), `flat_map`/`flat_set` (sorted vector-backed), `stable_vector`
- `circular_buffer` (already covered by `RingBuffer<T, N>`, modulo API differences)

## Container Details

### `Vector<T>` (`vector/vector.hpp`)

- Storage: contiguous buffer managed with `std::allocator<T>`; elements are constructed/destructed explicitly.
- Growth: ~1.5×, with a special-case for small capacities.
- Complexity: `push_back` amortized O(1), `operator[]` O(1), `at` O(1) + bounds check, `reserve` O(n).
- Notes: `operator[]` is unchecked; `at/front/back/pop_back` throw on invalid use.

### `ArrayList<T>` (`array-list/array_list.hpp`)

- Storage: contiguous `new[]` buffer (simpler than `Vector`), with manual growth and copying.
- Complexity: `add` amortized O(1), `insert/remove` O(n), indexing O(1).
- Notes: primarily an educational baseline; prefer `Vector` for move-aware element management.

### `ForwardList<T>` (`forward-list/forward_list.hpp`)

- Storage: singly-linked list with a sentinel `before_begin` node.
- Complexity: `push_front` O(1), `erase_after` O(1), traversal O(n).

### `LinkedList<T>` (`linked-list/linked_list.hpp`)

- Storage: singly-linked list with a sentinel head and cached tail pointer.
- Complexity: `append` O(1), `operator[]/insert/remove` by index O(n).

### `List<T>` (`list/list.hpp`)

- Storage: doubly-linked list with a sentinel node.
- Complexity: `push_front/push_back/pop_front/pop_back` O(1), `move_to_front` O(1), traversal O(n).
- Notes: provides stable iterators suitable for caches (used by `LRUCache`).

### `RingBuffer<T, N>` (`ring-buffer/ring_buffer.hpp`)

- Storage: fixed-size `std::array<T, N>` with circular head/tail indices.
- Complexity: `push/pop/peek` O(1).
- Notes: throws on overflow/underflow; `N > 0` is enforced at compile time.

### `Heap<T, compare, branches>` (`heap/heap.hpp`)

- Storage: d-ary heap (default binary) stored in `Vector<T>` (1-indexed math, 0-indexed storage).
- Complexity: `push` O(log₍branches₎ n), `pop` O(log₍branches₎ n), `top` O(1).
- Notes: `compare` controls the extremum at `top()`; `MinHeap<T>` and `MaxHeap<T>` aliases are provided.

### `PriorityQueue<T, Compare, branches>` (`priority-queue/priority_queue.hpp`)

- Wrapper over `Heap`; default is max-priority (`Compare = std::greater<T>`).
- Complexity: matches `Heap` (`push/pop` O(log n), `top` O(1)).

### `Stack<T>` (`stack/stack.hpp`)

- Wrapper over `Vector<T>` using `push_back/pop_back`.
- Complexity: `push/pop/top` amortized O(1).

### `Queue<T>` (`queue/queue.hpp`)

- Wrapper over `List<T>` using `push_back/pop_front`.
- Complexity: `push/pop/front/back` O(1).

### `unordered_map<K, V>` (`unordered-map/unordered_map.hpp`)

- Storage: separate chaining with `Vector<ForwardList<pair<K, V>>>` buckets; automatic rehash on load factor.
- Complexity (expected): `find/at/operator[]/insert/erase` O(1), worst-case O(n).
- Notes: `reserve` and `max_load_factor` control rehash behavior.

### `Trie` (`trie/trie.hpp`)

- Storage: radix tree over bytes (256-way branching) with `unique_ptr` children.
- Complexity: `insert/contains/erase` O(L) where L is the key length.
- Notes: optimized for simplicity; memory-heavy compared to compressed tries.

### `unique_ptr<T>` (`unique-ptr/unique_ptr.hpp`)

- RAII single-owner pointer with move semantics (`release/reset/get/operator*`).
- Notes: uses `std::default_delete<T>`; no array specialization.

### `LRUCache<K, V>` (`lru-cache/lru_cache.hpp`)

- Storage: `List<K>` for recency order + `unordered_map<K, {V, list_iterator}>` for O(1) lookup.
- Complexity: `get/insert` expected O(1); eviction is O(1).

## Build + Test

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
./build/stl_tests
```

## Benchmarks

```bash
./build/stl_bench --list
./build/stl_bench --n 200000 --iters 5 --warmup 1 --filter vector
```

Benchmarks are microbenchmarks: each case runs the same operation over the same input size for the custom container and
the closest `std::` container, then prints total time and time/op. Use `--n` to control the operation count and
`--filter` to run a subset.

## Coverage (Clang)

```bash
cmake -S . -B build-cov -DCMAKE_BUILD_TYPE=Debug -DSTL_ENABLE_COVERAGE=ON
cmake --build build-cov -j
LLVM_PROFILE_FILE=build-cov/coverage.profraw ./build-cov/stl_tests
llvm-profdata merge -sparse build-cov/coverage.profraw -o build-cov/coverage.profdata
llvm-cov report ./build-cov/stl_tests -instr-profile=build-cov/coverage.profdata
```
