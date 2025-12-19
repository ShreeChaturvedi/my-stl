#!/usr/bin/env python3
from __future__ import annotations

import argparse
import datetime as dt
import json
import os
import platform
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

BENCH_LINE_RE = re.compile(
    r"^(?P<name>.+) \[iters=(?P<iters>\d+), n=(?P<n>\d+)\]: "
    r"min=(?P<min_ns>\d+) ns \((?P<min_ns_per_op>[0-9eE.+-]+) ns/op\), "
    r"median=(?P<median_ns>\d+) ns \((?P<median_ns_per_op>[0-9eE.+-]+) ns/op\), "
    r"mean=(?P<mean_ns>\d+) ns \((?P<mean_ns_per_op>[0-9eE.+-]+) ns/op\), "
    r"max=(?P<max_ns>\d+) ns \((?P<max_ns_per_op>[0-9eE.+-]+) ns/op\)$"
)


def run(cmd: list[str], cwd: Path | None = None) -> str:
    proc = subprocess.run(cmd, cwd=cwd, check=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if proc.stderr:
        sys.stderr.write(proc.stderr)
    return proc.stdout.strip()


def try_run(cmd: list[str]) -> str | None:
    try:
        return run(cmd)
    except Exception:
        return None


def detect_cpu() -> str:
    sysctl = try_run(["sysctl", "-n", "machdep.cpu.brand_string"])
    if sysctl:
        return sysctl
    lscpu = try_run(["lscpu"])
    if lscpu:
        for line in lscpu.splitlines():
            if line.lower().startswith("model name"):
                return line.split(":", 1)[1].strip()
    return platform.processor() or "unknown"


def detect_compiler() -> str:
    env_cxx = os.environ.get("CXX")
    if env_cxx:
        out = try_run([env_cxx, "--version"])
        if out:
            return out.splitlines()[0]
    out = try_run(["c++", "--version"])
    if out:
        return out.splitlines()[0]
    return "unknown"


def parse_cmake_cache(build_dir: Path) -> dict[str, str]:
    cache_path = build_dir / "CMakeCache.txt"
    if not cache_path.exists():
        return {}
    values = {}
    for line in cache_path.read_text().splitlines():
        if line.startswith("//") or not line:
            continue
        if "=" not in line or ":" not in line:
            continue
        key, rest = line.split(":", 1)
        _, value = rest.split("=", 1)
        values[key] = value
    return values


def collect_meta(build_dir: Path, args: argparse.Namespace) -> dict[str, object]:
    meta: dict[str, object] = {}
    meta["timestamp"] = dt.datetime.now(dt.timezone.utc).isoformat()
    meta["platform"] = platform.platform()
    meta["cpu"] = detect_cpu()
    meta["compiler"] = detect_compiler()
    meta["python"] = sys.version.split()[0]
    meta["bench_args"] = {"n": args.n, "iters": args.iters, "warmup": args.warmup, "filter": args.filter}

    git_sha = try_run(["git", "rev-parse", "HEAD"])
    if git_sha:
        meta["git_sha"] = git_sha
        dirty = try_run(["git", "status", "--porcelain"])
        meta["git_dirty"] = bool(dirty)

    cache = parse_cmake_cache(build_dir)
    if cache:
        meta["cmake_build_type"] = cache.get("CMAKE_BUILD_TYPE", "")
        meta["cxx_compiler"] = cache.get("CMAKE_CXX_COMPILER", "")
        meta["cxx_flags_release"] = cache.get("CMAKE_CXX_FLAGS_RELEASE", "")
    return meta


def parse_output(output: str) -> list[dict[str, object]]:
    results: list[dict[str, object]] = []
    for line in output.splitlines():
        m = BENCH_LINE_RE.match(line.strip())
        if not m:
            continue
        item = m.groupdict()
        results.append(
            {
                "name": item["name"],
                "iters": int(item["iters"]),
                "n": int(item["n"]),
                "min_ns": int(item["min_ns"]),
                "median_ns": int(item["median_ns"]),
                "mean_ns": int(item["mean_ns"]),
                "max_ns": int(item["max_ns"]),
                "min_ns_per_op": float(item["min_ns_per_op"]),
                "median_ns_per_op": float(item["median_ns_per_op"]),
                "mean_ns_per_op": float(item["mean_ns_per_op"]),
                "max_ns_per_op": float(item["max_ns_per_op"]),
            }
        )
    return results


def main() -> int:
    parser = argparse.ArgumentParser(description="Run stl_bench and capture a structured report.")
    parser.add_argument("--build-dir", default="build", help="CMake build directory.")
    parser.add_argument("--no-build", action="store_true", help="Skip CMake configure/build.")
    parser.add_argument("--n", type=int, default=200_000)
    parser.add_argument("--iters", type=int, default=5)
    parser.add_argument("--warmup", type=int, default=1)
    parser.add_argument("--filter", default="", help="Filter substring for benchmark cases.")
    parser.add_argument("--out-dir", default="docs/benchmarks/runs", help="Output directory for JSON/log.")
    parser.add_argument("--label", default="", help="Optional label appended to the output filename.")
    args = parser.parse_args()

    build_dir = ROOT / args.build_dir
    if not args.no_build:
        run(["cmake", "-S", str(ROOT), "-B", str(build_dir), "-DCMAKE_BUILD_TYPE=Release"])
        run(["cmake", "--build", str(build_dir), "-j"])

    bench = build_dir / "stl_bench"
    if not bench.exists():
        raise SystemExit(f"Missing benchmark binary: {bench}")

    cmd = [str(bench), f"--n={args.n}", f"--iters={args.iters}", f"--warmup={args.warmup}"]
    if args.filter:
        cmd.append(f"--filter={args.filter}")

    output = run(cmd, cwd=ROOT)
    results = parse_output(output)
    if not results:
        raise SystemExit("No benchmark samples parsed. Check output format.")

    meta = collect_meta(build_dir, args)
    payload = {"meta": meta, "results": results}

    out_dir = ROOT / args.out_dir
    out_dir.mkdir(parents=True, exist_ok=True)

    stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S")
    label = f"-{args.label}" if args.label else ""
    stem = f"bench-{stamp}{label}"

    json_path = out_dir / f"{stem}.json"
    log_path = out_dir / f"{stem}.log"
    json_path.write_text(json.dumps(payload, indent=2))
    log_path.write_text(output + "\n")

    print(json_path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
