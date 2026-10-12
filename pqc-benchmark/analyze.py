#!/usr/bin/env python3
# Copyright (c) 2026 The pqc-esp32-experiments authors
# SPDX-License-Identifier: Apache-2.0
"""
Turns the pqc-benchmark logs into tables.

Usage: analyze.py results/run_A.log [results/run_B.log ...] > results/summary.md
Also writes results/timings.csv (one row per operation: n, mean, SD, ...),
results/runs.csv (every run), results/memory.csv (one row per operation:
runs, mean, SD and max of stack and heap), results/memory_runs.csv (every
run) and results/state.csv.

Every operation must have run at least MIN_RUNS (10) times; operations with
fewer runs (logs of the older firmware) are flagged in the summary.
"""
import csv
import os
import statistics
import sys
from collections import OrderedDict

MIN_RUNS = 10

R_FIELDS = ["group", "scheme", "impl", "op", "radio", "n", "fail", "mean_us", "sd_us",
            "median_us", "min_us", "max_us", "mean_cycles"]
M_FIELDS = ["scheme", "impl", "op", "stack", "heap_peak", "heap_peak_internal",
            "persistent", "persistent_internal", "rc", "idx"]
T_FIELDS = ["group", "scheme", "impl", "op", "radio", "idx", "us", "cycles"]


def parse(paths):
    rows, mem, state, env, runs = [], [], [], [], []
    for path in paths:
        run = os.path.basename(path).replace(".log", "")
        for line in open(path, encoding="utf-8", errors="replace"):
            t = line.rstrip("\n")
            t = t[20:] if len(t) > 20 and t[4] == "-" and t[19] == " " else t  # host timestamp
            parts = t.split(",")
            if parts[0] == "R" and len(parts) == 14:
                d = dict(zip(R_FIELDS, parts[1:]))
                d["run"] = run
                rows.append(d)
            elif parts[0] == "T" and len(parts) == 9:
                d = dict(zip(T_FIELDS, parts[1:]))
                d["run"] = run
                runs.append(d)
            elif parts[0] == "M" and len(parts) in (9, 10, 11):
                d = dict(zip(M_FIELDS, parts[1:]))
                d.setdefault("idx", "1")  # older firmware: one run per operation
                d["run"] = run
                mem.append(d)
            elif parts[0] == "S" and len(parts) == 5:
                state.append({"run": run, "scheme": parts[1], "impl": parts[2], "item": parts[3],
                              "value": parts[4]})
            elif parts[0] == "E":
                env.append((run, t))
    return rows, mem, state, env, runs


def mean_sd(xs):
    xs = [float(x) for x in xs]
    if not xs:
        return 0.0, 0.0
    return statistics.fmean(xs), (statistics.stdev(xs) if len(xs) > 1 else 0.0)


def aggregate_memory(mem):
    """One row per (log, scheme, impl, op) over its runs with rc 0; an
    operation measured in several logs counts from the first log only (the
    first memory runs of the older firmware had no return codes and were
    superseded)."""
    ok = [m for m in mem if m.get("rc") == "0"]
    fails = {}
    for m in mem:
        if m.get("rc") not in ("0", None):
            k = (m["run"], m["scheme"], m["impl"], m["op"])
            fails[k] = fails.get(k, 0) + 1
    groups = OrderedDict()
    for m in ok:
        groups.setdefault((m["run"], m["scheme"], m["impl"], m["op"]), []).append(m)
    out, seen = [], set()
    for (run, scheme, impl, op), ms in groups.items():
        if (scheme, impl, op) in seen:
            continue
        seen.add((scheme, impl, op))
        row = {"run": run, "scheme": scheme, "impl": impl, "op": op, "runs": len(ms),
               "failed": fails.get((run, scheme, impl, op), 0)}
        for fld in ("stack", "heap_peak", "heap_peak_internal", "persistent",
                    "persistent_internal"):
            mu, sd = mean_sd(m[fld] for m in ms)
            row[fld + "_mean"] = round(mu, 1)
            row[fld + "_sd"] = round(sd, 1)
            row[fld + "_max"] = max(int(m[fld]) for m in ms)
        out.append(row)
    return out, [m for m in ok if (m["run"], m["scheme"], m["impl"], m["op"]) in
                 {(r["run"], r["scheme"], r["impl"], r["op"]) for r in out}]


def fmt_pm(mean_us, sd_us):
    return f"{fmt_ms(mean_us)} ± {fmt_ms(sd_us)}"


def fmt_ms(us):
    ms = float(us) / 1000.0
    if ms < 10:
        return f"{ms:.2f} ms"
    if ms < 1000:
        return f"{ms:.1f} ms"
    if ms < 60000:
        return f"{ms / 1000:.2f} s"
    return f"{ms / 60000:.2f} min"


def fmt_cyc(c):
    c = float(c)
    if c < 1e6:
        return f"{c / 1e3:.0f} k"
    if c < 1e9:
        return f"{c / 1e6:.2f} M"
    return f"{c / 1e9:.2f} G"


def check_runs(rows, runs):
    """Cross-check the R summaries against the T lines (every run)."""
    by = {}
    for t in runs:
        by.setdefault((t["run"], t["group"], t["scheme"], t["impl"], t["op"], t["radio"]),
                      []).append(float(t["us"]))
    bad = []
    for r in rows:
        k = (r["run"], r["group"], r["scheme"], r["impl"], r["op"], r["radio"])
        if k not in by:
            continue
        mu, sd = mean_sd(by[k])
        if len(by[k]) != int(r["n"]) or abs(mu - float(r["mean_us"])) > 0.1 + 1e-6 * mu \
                or abs(sd - float(r["sd_us"])) > 0.1 + 1e-4 * sd:
            bad.append((k, len(by[k]), mu, sd))
    return bad


def main():
    rows, mem, state, env, runs = parse(sys.argv[1:])
    mem_rows, mem_runs = aggregate_memory(mem)
    out = os.path.dirname(sys.argv[1]) or "."
    with open(os.path.join(out, "timings.csv"), "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=["run"] + R_FIELDS)
        w.writeheader()
        w.writerows(rows)
    with open(os.path.join(out, "runs.csv"), "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=["run"] + T_FIELDS)
        w.writeheader()
        w.writerows(runs)
    with open(os.path.join(out, "memory.csv"), "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(mem_rows[0].keys()) if mem_rows else ["run"])
        w.writeheader()
        w.writerows(mem_rows)
    with open(os.path.join(out, "memory_runs.csv"), "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=["run"] + M_FIELDS)
        w.writeheader()
        w.writerows(mem_runs)
    with open(os.path.join(out, "state.csv"), "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=["run", "scheme", "impl", "item", "value"])
        w.writeheader()
        w.writerows(state)

    print("# pqc-benchmark summary\n")
    for run, e in OrderedDict(((r, e) for r, e in env)).items():
        print(f"- {run}: `{e}`")
    print()
    print("Times are mean ± sample standard deviation over N runs.\n")
    few = [r for r in rows if int(r["n"]) < MIN_RUNS]
    few_mem = [m for m in mem_rows if m["runs"] < MIN_RUNS and m["scheme"] != "baseline"]
    if few or few_mem:
        print(f"**Warning:** {len(few)} timing rows and {len(few_mem)} memory rows have fewer "
              f"than {MIN_RUNS} runs (older firmware); rerun them.\n")
    bad = check_runs(rows, runs)
    if bad:
        print(f"**Warning:** {len(bad)} summary rows do not match their per-run lines:\n")
        for k, n, mu, sd in bad[:20]:
            print(f"- {k}: n={n} mean={mu:.1f} sd={sd:.1f}")
        print()
    elif runs:
        print(f"All summary rows match their {len(runs)} per-run lines (`runs.csv`).\n")
    failed = [r for r in rows if int(r["fail"]) > 0]
    if failed:
        print(f"**Rows with failed operations** ({len(failed)}; excluded from the comparison "
              "tables):\n")
        for r in failed:
            print(f"- {r['run']} group {r['group']}: {r['scheme']} {r['impl']} {r['op']}, "
                  f"radio {r['radio']}: {r['fail']} of {r['n']} failed")
        print()

    # main timing table: one row per scheme/impl/config, columns per operation
    groups = OrderedDict()
    for r in rows:
        if r["group"] not in "kdslx":
            continue
        key = (r["run"], r["group"], r["scheme"], r["impl"])
        groups.setdefault(key, OrderedDict())[r["op"]] = r
    titles = {"k": "ML-KEM", "d": "ML-DSA", "s": "SLH-DSA", "l": "LMS / HSS", "x": "XMSS / XMSS^MT"}
    last = None
    for (run, g, scheme, impl), ops in groups.items():
        if (run, g) != last:
            print(f"\n## {titles[g]} ({run}, radio {next(iter(ops.values()))['radio']})\n")
            print("| Scheme | Impl | Operation | N | Mean ± SD | Median | Min | Max | "
                  "Cycles (mean) | Fail |")
            print("|---|---|---|--:|--:|--:|--:|--:|--:|--:|")
            last = (run, g)
        for op, r in ops.items():
            flag = " ⚠" if int(r["n"]) < MIN_RUNS else ""
            print(f"| {scheme} | {impl} | {op} | {r['n']}{flag} | "
                  f"{fmt_pm(r['mean_us'], r['sd_us'])} | {fmt_ms(r['median_us'])} | "
                  f"{fmt_ms(r['min_us'])} | {fmt_ms(r['max_us'])} | "
                  f"{fmt_cyc(r['mean_cycles'])} | {r['fail']} |")

    # radio control: compare group c (radio off) with the main groups (radio on)
    main = {(r["run"], r["scheme"], r["impl"], r["op"]): r for r in rows if r["group"] in "kdslx"}
    ctrl = [r for r in rows if r["group"] == "c"]
    if ctrl:
        print("\n## Radio interference control (same build, radio off vs on)\n")
        print("| Scheme | Impl | Operation | Radio on | Radio off | Difference |")
        print("|---|---|---|--:|--:|--:|")
        for r in ctrl:
            m = main.get((r["run"], r["scheme"], r["impl"], r["op"]))
            if m is None or int(m["fail"]) > 0 or int(r["fail"]) > 0:
                continue
            on, off = float(m["mean_us"]), float(r["mean_us"])
            print(f"| {r['scheme']} | {r['impl']} | {r['op']} | "
                  f"{fmt_pm(m['mean_us'], m['sd_us'])} | {fmt_pm(r['mean_us'], r['sd_us'])} | "
                  f"{(on - off) / off * 100:+.2f}% |")

    # hardware speed-ups
    hw = [(k, v) for k, v in main.items() if k[2] == "HW"]
    if hw:
        print("\n## SHA hardware speed-up (software mean / hardware mean)\n")
        print("| Scheme | Operation | Software | Hardware | Speed-up |")
        print("|---|---|--:|--:|--:|")
        for (run, scheme, impl, op), v in hw:
            sw = main.get((run, scheme, "SW", op))
            if sw is None or int(sw["fail"]) > 0 or int(v["fail"]) > 0:
                continue
            a, b = float(sw["mean_us"]), float(v["mean_us"])
            print(f"| {scheme} | {op} | {fmt_pm(sw['mean_us'], sw['sd_us'])} | "
                  f"{fmt_pm(v['mean_us'], v['sd_us'])} | {a / b:.2f}x |")

    if mem_rows:
        print("\n## Memory (bytes; stack and heap net of the empty measurement task; "
              "mean ± SD and max over the runs with return code 0)\n")
        print("| Run | Scheme | Impl | Operation | Runs | Stack | Stack max | Heap peak | "
              "Heap max | of which internal (max) | Persistent | of which internal | Failed |")
        print("|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|--:|")
        for m in mem_rows:
            flag = " ⚠" if m["runs"] < MIN_RUNS and m["scheme"] != "baseline" else ""
            print(f"| {m['run']} | {m['scheme']} | {m['impl']} | {m['op']} | {m['runs']}{flag} | "
                  f"{m['stack_mean']:.0f} ± {m['stack_sd']:.0f} | {m['stack_max']} | "
                  f"{m['heap_peak_mean']:.0f} ± {m['heap_peak_sd']:.0f} | {m['heap_peak_max']} | "
                  f"{m['heap_peak_internal_max']} | "
                  f"{m['persistent_mean']:.0f} ± {m['persistent_sd']:.0f} | "
                  f"{m['persistent_internal_max']} | {m['failed']} |")

    st = [r for r in rows if r["group"] == "n"]
    if st or state:
        print("\n## State management (NVS storage, SHA hardware on)\n")
        print("| Scheme | Item | N | Mean ± SD | Median | Max |")
        print("|---|---|--:|--:|--:|--:|")
        for r in st:
            flag = " ⚠" if int(r["n"]) < MIN_RUNS else ""
            print(f"| {r['scheme']} | {r['op']} | {r['n']}{flag} | "
                  f"{fmt_pm(r['mean_us'], r['sd_us'])} | "
                  f"{fmt_ms(r['median_us'])} | {fmt_ms(r['max_us'])} |")
        for s in state:
            print(f"| {s['scheme']} | {s['item']} | | {s['value']} | | |")


if __name__ == "__main__":
    main()
