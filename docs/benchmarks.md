# Benchmarks

This report summarizes microbenchmarks that compare my-stl containers to their closest `std::` equivalents.
Each benchmark runs the same operation with the same input size and prints timing samples.

## Methodology

- Build type: `Release` (`-O3` for `stl_bench`)
- Warmup iterations: 1
- Sample iterations: 5
- Input size (`n`): 200000
- Reported metric: median nanoseconds per operation

## How to Reproduce

```bash
python3 scripts/bench/run.py --n 200000 --iters 5 --warmup 1
python3 scripts/bench/report.py --run docs/benchmarks/runs/bench-YYYYMMDD-HHMMSS.json
```

## Latest Run

Run file: `docs/benchmarks/runs/bench-20251219-141843-m2.json`

- Timestamp (UTC): 2025-12-19T19:18:43Z
- Machine: Apple M2, macOS 15.5
- Compiler: Apple clang 17.0.0
- Build: Release (`-O3`)

Summary (median ns/op; ratio = my-stl / std):

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

See `docs/benchmarks/bench_summary.csv` for the full table.
The chart below shows the ratio (my-stl / std). Lower is better.

![Benchmark ratio chart](benchmarks/charts/ratio-light.svg#gh-light-mode-only)
![Benchmark ratio chart](benchmarks/charts/ratio-dark.svg#gh-dark-mode-only)

## Notes

- Microbenchmarks measure specific operations and can be sensitive to allocator, CPU scaling, and compiler flags.
- Use the scripts above to regenerate results on your hardware for accurate comparisons.
- `flat_map` and `flat_set` are built from sorted keys and queried with shuffled lookups to reflect intended usage.
