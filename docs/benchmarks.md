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

Run file: `docs/benchmarks/runs/bench-20251219-120903-m2.json`

- Timestamp (UTC): 2025-12-19T17:09:03Z
- Machine: Apple M2, macOS 15.5
- Compiler: Apple clang 17.0.0
- Build: Release (`-O3`)

Summary (median ns/op; ratio = my-stl / std):

| Case | my-stl | std | Ratio |
| --- | --- | --- | --- |
| Vector push_back (no reserve) | 0.930 | 0.683 | 1.360 |
| Vector push_back (reserve) | 0.443 | 0.442 | 1.001 |
| unordered_map emplace (reserve) | 49.592 | 41.297 | 1.201 |
| flat_map build+find | 10188.500 | 217.557 | 46.831 |
| flat_set build+find | 5005.510 | 195.182 | 25.645 |
| small_vector push_back | 1.037 | 0.643 | 1.613 |
| stable_vector push_back | 16.395 | 14.678 | 1.117 |

See `docs/benchmarks/bench_summary.csv` for the full table.
The chart below shows the ratio (my-stl / std). Lower is better.

![Benchmark ratio chart](benchmarks/charts/ratio.svg)

## Notes

- Microbenchmarks measure specific operations and can be sensitive to allocator, CPU scaling, and compiler flags.
- Use the scripts above to regenerate results on your hardware for accurate comparisons.
- `flat_map` and `flat_set` are compared to `std::map`/`std::set`, which are not direct equivalents; use the results as a directional reference only.
