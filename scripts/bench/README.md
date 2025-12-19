# Benchmark Scripts

These scripts run the benchmark binary and generate a simple report + SVG chart.

## Run benchmarks and capture a structured run

```bash
python3 scripts/bench/run.py --n 200000 --iters 5 --warmup 1
```

The script prints the JSON file it created in `docs/benchmarks/runs/`.

## Generate reports from a run file

```bash
python3 scripts/bench/report.py --run docs/benchmarks/runs/bench-YYYYMMDD-HHMMSS.json
```

Outputs:

- `docs/benchmarks/bench_summary.json`
- `docs/benchmarks/bench_summary.csv`
- `docs/benchmarks/charts/ratio-light.svg`
- `docs/benchmarks/charts/ratio-dark.svg`

## Notes

- Re-run benchmarks on your machine for apples-to-apples comparisons.
- For reproducible results, keep the same compiler, build type, and CPU settings.
