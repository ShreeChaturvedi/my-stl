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
ctest --test-dir build --output-on-failure
```

Tests are written with Catch2.

## Benchmarks (Latest Run)

Run file: `docs/benchmarks/runs/bench-20251219-154045-m2.json`

- Apple M2, macOS 15.5
- Apple clang 17.0.0
- Release (`-O3`)

Median ns/op (ratio = my-stl / std):

| Case | my-stl (ns/op) | std (ns/op) | Ratio |
| --- | --- | --- | --- |
| Deque push_back+pop_front | `1.720` | `2.892` | `0.595x` |
| Vector push_back (no reserve) | `1.609` | `2.516` | `0.640x` |
| Vector push_back (reserve) | `0.463` | `0.465` | `0.996x` |
| unordered_map emplace (reserve) | `48.886` | `41.325` | `1.183x` |
| map build+find | `239.379` | `213.428` | `1.122x` |
| set build+find | `216.455` | `190.596` | `1.136x` |
| flat_map build+find | `50.222` | `133.032` | `0.378x` |
| flat_set build+find | `43.602` | `189.874` | `0.230x` |
| small_vector push_back | `0.856` | `0.804` | `1.064x` |
| stable_vector push_back | `16.355` | `14.612` | `1.119x` |

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
