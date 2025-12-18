# mini-stl

A modern C++ (C++23) “build-your-own-STL” project: hand-rolled containers, RAII utilities, tests, and benchmarks.

## Containers

- `STL/array-list` — `ArrayList<T>`
- `STL/linked-list` — `LinkedList<T>`
- `STL/ring-buffer` — `RingBuffer<T, capacity>`
- `STL/heap` — `Heap<T, compare, branches>`
- `STL/vector` — `Vector<T>`
- `STL/unordered-map` — `unordered_map<K, V>`
- `STL/unique-ptr` — `unique_ptr<T>`
- `STL/trie` — `Trie`
- `STL/lru-cache` — `LRUCache<K, V>`

## Build + Test

```bash
cmake -S STL -B STL/build -DCMAKE_BUILD_TYPE=Debug
cmake --build STL/build -j
./STL/build/stl_tests
```

## Benchmarks

```bash
./STL/build/stl_bench --list
./STL/build/stl_bench --n 200000 --filter vector
```

## Coverage (Clang)

```bash
cmake -S STL -B STL/build-cov -DCMAKE_BUILD_TYPE=Debug -DSTL_ENABLE_COVERAGE=ON
cmake --build STL/build-cov -j
LLVM_PROFILE_FILE=STL/build-cov/coverage.profraw ./STL/build-cov/stl_tests
llvm-profdata merge -sparse STL/build-cov/coverage.profraw -o STL/build-cov/coverage.profdata
llvm-cov report ./STL/build-cov/stl_tests -instr-profile=STL/build-cov/coverage.profdata
```
