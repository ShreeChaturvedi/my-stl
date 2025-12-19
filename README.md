# my-stl

[![ci](https://github.com/ShreeChaturvedi/my-stl/actions/workflows/ci.yml/badge.svg)](https://github.com/ShreeChaturvedi/my-stl/actions/workflows/ci.yml)
[![license](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![c++](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://isocpp.org/)

A modern C++23 "build-your-own STL" project: header-only containers, RAII utilities, tests, and benchmarks.

## Highlights

- Header-only containers with STL-like APIs and deterministic behavior.
- Self-hosting where it makes sense (containers built on other my-stl containers).
- Unit tests for every container plus a small benchmark harness.
- Clear separation of container modules and supporting utilities.

## Container Matrix

Y = implemented and tested, Bench = has benchmark coverage.

| Module | Primary type(s) | Notes | Test | Bench |
| --- | --- | --- | --- | --- |
| `array-list/` | `ArrayList<T>` | simple contiguous array list | Y | N |
| `vector/` | `Vector<T>` | allocator-aware dynamic array | Y | Y |
| `deque/` | `Deque<T>` | ring-buffer deque | Y | N |
| `forward-list/` | `ForwardList<T>` | singly linked list | Y | N |
| `linked-list/` | `LinkedList<T>` | singly linked list with tail | Y | N |
| `list/` | `List<T>` | doubly linked list with sentinel | Y | N |
| `stack/` | `Stack<T>` | `Vector` adaptor | Y | N |
| `queue/` | `Queue<T>` | `List` adaptor | Y | N |
| `heap/` | `Heap<T, Compare, branches>` | d-ary heap | Y | N |
| `priority-queue/` | `PriorityQueue<T, Compare, branches>` | heap adaptor | Y | N |
| `ring-buffer/` | `RingBuffer<T, N>` | fixed-size circular buffer | Y | N |
| `map/` | `map<K, V>`, `multimap<K, V>` | RB-tree-backed | Y | N |
| `set/` | `set<K>`, `multiset<K>` | RB-tree-backed | Y | N |
| `unordered-map/` | `unordered_map<K, V>` | separate chaining | Y | Y |
| `unordered-set/` | `unordered_set<K>` | map-backed | Y | N |
| `unordered-multimap/` | `unordered_multimap<K, V>` | separate chaining | Y | N |
| `unordered-multiset/` | `unordered_multiset<K>` | multimap-backed | Y | N |
| `flat-map/` | `FlatMap<K, V>` | sorted vector-backed | Y | Y |
| `flat-set/` | `FlatSet<K>` | sorted vector-backed | Y | Y |
| `small-vector/` | `SmallVector<T, N>` | inline storage | Y | Y |
| `stable-vector/` | `StableVector<T>` | pointer-stable storage | Y | Y |
| `span/` | `Span<T>` | view over contiguous data | Y | N |
| `string/` | `basic_string<CharT>` | SSO string | Y | N |
| `unique-ptr/` | `unique_ptr<T>` | single-owner RAII | Y | N |
| `trie/` | `Trie` | byte radix tree | Y | N |
| `lru-cache/` | `LRUCache<K, V>` | list + map | Y | N |
| `rb-tree/` | `RbTree<...>` | internal building block | Y (indirect) | N |
| `utility/` | `unit` | internal building block | Y (indirect) | N |

## Design Notes

- Header-only: everything is `*.hpp` + `*.tpp`, included via CMake include paths.
- Self-hosting where it fits:
  - `Heap` uses `Vector`
  - `unordered_map` uses `Vector` + `ForwardList`
  - `LRUCache` uses `List` + `unordered_map`
  - `Stack` uses `Vector`, `Queue` uses `List`, `PriorityQueue` uses `Heap`
- APIs are STL-like with deliberate simplifications documented in `docs/containers/`.

## Build and Test

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
./build/stl_tests
```

## Benchmarks

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/stl_bench --list
./build/stl_bench --n 200000 --iters 5 --warmup 1 --filter vector
```

Benchmark methodology, charts, and recorded results are in `docs/benchmarks.md`.
Use the scripts in `scripts/bench/` to run and regenerate reports on your machine.

## Documentation

- `docs/containers/README.md` -- per-container notes, complexity, differences vs `std::`
- `docs/benchmarks.md` -- benchmark methodology and results

## Repository Layout

- `tests/` -- unit tests
- `bench/` -- microbenchmarks
- `docs/` -- documentation and reports
- `*/` -- each top-level container is a self-contained module

## Contributing

See `CONTRIBUTING.md`.

## License

MIT -- see `LICENSE`.
