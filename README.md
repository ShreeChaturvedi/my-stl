# my-stl

[![ci](https://github.com/ShreeChaturvedi/my-stl/actions/workflows/ci.yml/badge.svg)](https://github.com/ShreeChaturvedi/my-stl/actions/workflows/ci.yml)
[![license](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![c++](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://isocpp.org/)

A modern C++23 "build-your-own STL" project: header-only containers, RAII utilities, tests, and published benchmarks.

## Highlights

- Header-only containers with STL-like APIs and deterministic behavior.
- Self-hosting where it makes sense (containers built on other my-stl containers).
- Full unit test suite and reproducible benchmarks with published results.

## Containers

Sequence:
`ArrayList`, `Vector`, `Deque`, `ForwardList`, `LinkedList`, `List`, `RingBuffer`, `SmallVector`, `StableVector`,
`Span`, `basic_string`.

Associative:
`map`/`multimap`, `set`/`multiset`, `FlatMap`, `FlatSet`.

Unordered:
`unordered_map`, `unordered_set`, `unordered_multimap`, `unordered_multiset`.

Adaptors:
`Stack`, `Queue`, `PriorityQueue`, `Heap`.

Utilities:
`LRUCache`, `Trie`, `unique_ptr` (plus internal `RbTree`).

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

## Benchmarks (Latest Run)

Run file: `docs/benchmarks/runs/bench-20251219-141843-m2.json`

- Apple M2, macOS 15.5
- Apple clang 17.0.0
- Release (`-O3`)

Median ns/op (ratio = my-stl / std):

| Case | my-stl (ns/op) | std (ns/op) | Ratio |
| --- | --- | --- | --- |
| Deque push_back+pop_front | `2.304` | `6.176` | `0.373x` |
| Vector push_back (no reserve) | `0.974` | `0.823` | `1.183x` |
| Vector push_back (reserve) | `0.447` | `0.451` | `0.991x` |
| unordered_map emplace (reserve) | `59.899` | `46.126` | `1.299x` |
| map build+find | `392.299` | `239.301` | `1.639x` |
| set build+find | `298.538` | `229.745` | `1.299x` |
| flat_map build+find | `52.994` | `173.944` | `0.305x` |
| flat_set build+find | `50.237` | `177.828` | `0.283x` |
| small_vector push_back | `1.311` | `0.674` | `1.945x` |
| stable_vector push_back | `17.143` | `15.022` | `1.141x` |

Full results: `docs/benchmarks/bench_summary.csv`

![Benchmark ratio chart](docs/benchmarks/charts/ratio-light.svg#gh-light-mode-only)
![Benchmark ratio chart](docs/benchmarks/charts/ratio-dark.svg#gh-dark-mode-only)

flat_map/flat_set are built from sorted keys and queried with shuffled lookups.

## Run Benchmarks

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/stl_bench --list
./build/stl_bench --n 200000 --iters 5 --warmup 1 --filter vector
```

Detailed methodology and scripts: `docs/benchmarks.md` and `scripts/bench/`.

## Documentation

- `docs/containers/README.md` -- per-container notes, complexity, differences vs `std::`
- `docs/benchmarks.md` -- benchmark methodology and results

## Contributing

See `CONTRIBUTING.md`.

## License

MIT -- see `LICENSE`.
