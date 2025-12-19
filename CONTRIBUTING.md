# Contributing to my-stl

Thanks for the interest in contributing. This repo is a learning-focused, header-only C++23 "build-your-own STL" project.
Contributions that improve correctness, clarity, tests, and performance are welcome.

## Quick Start

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## Project Structure

- Containers live in top-level folders (e.g., `vector/`, `unordered-map/`) with `*.hpp` + `*.tpp`.
- Tests live in `tests/` and are built into `stl_tests` (Catch2).
- Benchmarks live in `bench/` and are built into `stl_bench`.
- Documentation lives in `docs/`.

## Style & Expectations

- C++23, `-Wall -Wextra -Wpedantic` clean.
- Keep APIs STL-like where practical and document differences.
- Prefer `#pragma once` and header-only implementations.
- Match existing style in the module you touch.
- Add tests for new behavior or bug fixes.

## Benchmarking

If you change performance-sensitive code:

- Add or update a `bench/bench_*.cpp` case for that container.
- Update `docs/benchmarks.md` with the new results and charts.
- Include the command you ran and environment details.

## Submitting Changes

- Keep commits small and scoped (ideally one container at a time).
- Use short, imperative commit subjects (e.g., `docs: update vector details`).
- Do not commit build artifacts.

If you have questions, open an issue or start a discussion.
