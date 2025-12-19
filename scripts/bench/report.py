#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
from pathlib import Path

PAIRINGS = [
    {
        "id": "deque_push_pop",
        "title": "Deque push_back+pop_front",
        "mini": "Deque<int>::push_back+pop_front",
        "std": "std::deque<int>::push_back+pop_front",
    },
    {
        "id": "vector_push_back_no_reserve",
        "title": "Vector push_back (no reserve)",
        "mini": "Vector<int>::push_back (no reserve)",
        "std": "std::vector<int>::push_back (no reserve)",
    },
    {
        "id": "vector_push_back_reserve",
        "title": "Vector push_back (reserve)",
        "mini": "Vector<int>::push_back (reserve)",
        "std": "std::vector<int>::push_back (reserve)",
    },
    {
        "id": "unordered_map_emplace_reserve",
        "title": "unordered_map emplace (reserve)",
        "mini": "unordered_map<string,int>::emplace (reserve)",
        "std": "std::unordered_map<string,int>::emplace (reserve)",
    },
    {
        "id": "map_build_find",
        "title": "map build+find",
        "mini": "map/build+find (my-stl)",
        "std": "map/build+find (std::map)",
    },
    {
        "id": "set_build_find",
        "title": "set build+find",
        "mini": "set/build+find (my-stl)",
        "std": "set/build+find (std::set)",
    },
    {
        "id": "flat_map_build_find",
        "title": "flat_map build+find",
        "mini": "flat_map/build+find (sorted, my-stl)",
        "std": "flat_map/build+find (sorted, std::map)",
    },
    {
        "id": "flat_set_build_find",
        "title": "flat_set build+find",
        "mini": "flat_set/build+find (sorted, my-stl)",
        "std": "flat_set/build+find (sorted, std::set)",
    },
    {
        "id": "small_vector_push_back",
        "title": "small_vector push_back",
        "mini": "SmallVector<int,16>::push_back",
        "std": "std::vector<int>::push_back",
    },
    {
        "id": "stable_vector_push_back",
        "title": "stable_vector push_back",
        "mini": "StableVector<int>::push_back",
        "std": "std::vector<unique_ptr<int>>::push_back",
    },
]


def load_run(path: Path) -> dict:
    return json.loads(path.read_text())


def to_map(results: list[dict]) -> dict[str, dict]:
    return {item["name"]: item for item in results}


def format_ratio(value: float | None) -> str:
    if value is None:
        return "n/a"
    return f"{value:.2f}x"


def write_csv(rows: list[dict], out_path: Path) -> None:
    headers = [
        "case_id",
        "title",
        "n",
        "iters",
        "mini_median_ns_per_op",
        "std_median_ns_per_op",
        "ratio_mini_vs_std",
    ]
    lines = [",".join(headers)]
    for row in rows:
        lines.append(
            ",".join(
                [
                    row["case_id"],
                    row["title"],
                    str(row.get("n", "")),
                    str(row.get("iters", "")),
                    str(row.get("mini_median_ns_per_op", "")),
                    str(row.get("std_median_ns_per_op", "")),
                    str(row.get("ratio_mini_vs_std", "")),
                ]
            )
        )
    out_path.write_text("\n".join(lines) + "\n")


def svg_ratio_chart(rows: list[dict], out_path: Path, *, theme: dict[str, str]) -> None:
    width = 900
    row_h = 28
    top = 40
    left = 260
    right = 40
    height = top + row_h * len(rows) + 40

    ratios = [r["ratio_mini_vs_std"] for r in rows if r["ratio_mini_vs_std"]]
    max_ratio = max(ratios + [1.0]) * 1.1
    usable = width - left - right

    def x(value: float) -> float:
        return left + (value / max_ratio) * usable

    lines = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" '
        f'viewBox="0 0 {width} {height}" role="img">',
        '<style>'
        f'text{{font-family:Arial, sans-serif;font-size:12px;fill:{theme["text"]};}}'
        f'.axis{{stroke:{theme["axis"]};stroke-width:1;}}'
        f'.bar{{fill:{theme["bar"]};}}'
        f'.baseline{{stroke:{theme["baseline"]};stroke-width:1;stroke-dasharray:4 4;}}'
        "</style>",
        f'<rect width="100%" height="100%" fill="{theme["background"]}"/>',
        f'<text x="{left}" y="20">Median ns/op ratio (my-stl / std). Lower is better.</text>',
        f'<line class="axis" x1="{left}" y1="{top-6}" x2="{left}" y2="{height-20}"/>',
    ]

    baseline_x = x(1.0)
    lines.append(f'<line class="baseline" x1="{baseline_x}" y1="{top-6}" x2="{baseline_x}" y2="{height-20}"/>')

    for i, row in enumerate(rows):
        y = top + i * row_h
        label = row["title"]
        ratio = row["ratio_mini_vs_std"]
        value = ratio if ratio else 0.0
        bar_w = x(value) - left
        lines.append(f'<text x="10" y="{y+15}">{label}</text>')
        lines.append(f'<rect class="bar" x="{left}" y="{y}" width="{bar_w:.1f}" height="18"/>')
        lines.append(f'<text x="{left + bar_w + 6:.1f}" y="{y+15}">{format_ratio(ratio)}</text>')

    lines.append("</svg>")
    out_path.write_text("\n".join(lines))


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate summary CSV and charts from a run JSON.")
    parser.add_argument("--run", required=True, help="Path to run JSON produced by run.py")
    parser.add_argument("--out-dir", default="docs/benchmarks", help="Output directory")
    args = parser.parse_args()

    run_path = Path(args.run)
    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / "charts").mkdir(parents=True, exist_ok=True)

    payload = load_run(run_path)
    result_map = to_map(payload["results"])

    rows = []
    for pair in PAIRINGS:
        mini = result_map.get(pair["mini"])
        std = result_map.get(pair["std"])
        ratio = None
        if mini and std and std["median_ns_per_op"] > 0:
            ratio = mini["median_ns_per_op"] / std["median_ns_per_op"]
        row = {
            "case_id": pair["id"],
            "title": pair["title"],
            "n": mini["n"] if mini else std["n"] if std else "",
            "iters": mini["iters"] if mini else std["iters"] if std else "",
            "mini_median_ns_per_op": mini["median_ns_per_op"] if mini else "",
            "std_median_ns_per_op": std["median_ns_per_op"] if std else "",
            "ratio_mini_vs_std": ratio if ratio else "",
        }
        rows.append(row)

    summary_json = {
        "meta": payload.get("meta", {}),
        "run": str(run_path),
        "pairs": rows,
    }

    (out_dir / "bench_summary.json").write_text(json.dumps(summary_json, indent=2))
    write_csv(rows, out_dir / "bench_summary.csv")
    svg_ratio_chart(
        rows,
        out_dir / "charts" / "ratio-light.svg",
        theme={
            "text": "#24292f",
            "axis": "#57606a",
            "bar": "#2f6f8f",
            "baseline": "#c23a3a",
            "background": "#ffffff",
        },
    )
    svg_ratio_chart(
        rows,
        out_dir / "charts" / "ratio-dark.svg",
        theme={
            "text": "#e6edf3",
            "axis": "#8b949e",
            "bar": "#58a6ff",
            "baseline": "#f85149",
            "background": "#0d1117",
        },
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
