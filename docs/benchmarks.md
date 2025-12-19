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

Run file: `docs/benchmarks/runs/bench-20251219-154045-m2.json`

- Timestamp (UTC): 2025-12-19T20:40:45Z
- Machine: Apple M2, macOS 15.5
- Compiler: Apple clang 17.0.0
- Build: Release (`-O3`)

Summary (median ns/op; ratio = my-stl / std):

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

See `docs/benchmarks/bench_summary.csv` for the full table.
The chart below shows the ratio (my-stl / std). Lower is better.

![Benchmark ratio chart](benchmarks/charts/ratio-light.svg#gh-light-mode-only)
![Benchmark ratio chart](benchmarks/charts/ratio-dark.svg#gh-dark-mode-only)

## Notes

- Microbenchmarks measure specific operations and can be sensitive to allocator, CPU scaling, and compiler flags.
- Use the scripts above to regenerate results on your hardware for accurate comparisons.
- `flat_map` and `flat_set` are built from sorted keys and queried with shuffled lookups to reflect intended usage.
