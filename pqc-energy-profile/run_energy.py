#!/usr/bin/env python3
# Copyright (c) 2026 The pqc-esp32-experiments authors
# SPDX-License-Identifier: Apache-2.0
"""
Host driver for pqc-energy-profile (headless): runs the phases one after the
other and asks for the power shown by the inline USB meter (FNIRSI FNB-C2 or
similar, display only) after each phase.

Usage: run_energy.py PORT OUT.csv [settle_s]
  Each phase runs for at least phase_s seconds (45) and until every
  operation has run at least 10 times. Read the meter after the countdown
  shows "READ NOW" (the reading has settled, default 10 s into the phase)
  and before the phase ends; type the power in W when asked (e.g. 0.412, or
  0.40-0.43 for a range: the middle is used). Empty input skips a phase.

Writes
  OUT.csv       one row per phase and operation: power, number of runs, time
                per operation (mean, sample SD), energy per operation (mean,
                SD) total and incremental (above the power of phase 1, "Idle")
  OUT_runs.csv  every run: phase, operation, run, time, energies
Energy of a run: E = P * t (W * ms = mJ); the SD of the energy is that of the
run times scaled by P (the meter gives one power value per phase).
"""
import csv
import os
import statistics
import sys
import threading
import time

import serial


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    port, out = sys.argv[1], sys.argv[2]
    settle = float(sys.argv[3]) if len(sys.argv) > 3 else 10.0
    s = serial.Serial()
    s.port, s.baudrate, s.timeout = port, 115200, 0.2
    s.dtr = False
    s.rts = False
    s.open()
    s.rts = True  # reset the board
    time.sleep(0.1)
    s.rts = False

    def readline(deadline):
        while time.time() < deadline:
            ln = s.readline().decode(errors="replace").strip()
            if ln:
                return ln
        return None

    t_end = time.time() + 20
    nphases = phase_s = None
    while time.time() < t_end:
        ln = readline(t_end)
        if ln and ln.startswith("READY pqc-energy-profile"):
            kv = dict(x.split("=") for x in ln.split(",")[1:])
            nphases, phase_s = int(kv["phases"]), int(kv["phase_s"])
            break
    if nphases is None:
        sys.exit("no READY line from the board")
    print(f"{nphases} phases, each >= {phase_s} s and >= 10 runs per operation. "
          "Battery full, meter connected.")
    input("Press Enter to start...")

    base = os.path.splitext(out)[0]
    rows, run_rows, idle_w = [], [], None
    for k in range(nphases):
        s.write(b"g")
        name, result, t0 = "?", None, None
        runs = {}  # op -> [us, ...]
        stop = threading.Event()

        def countdown():
            while not stop.is_set():
                if t0 is not None:
                    el = time.time() - t0
                    tag = "READ NOW" if el >= settle else "settling"
                    print(f"\r  {name}: {el:6.1f} s (min {phase_s} s)  {tag}   ", end="",
                          flush=True)
                time.sleep(0.5)

        th = threading.Thread(target=countdown, daemon=True)
        th.start()
        deadline = time.time() + 6 * 3600
        while result is None:
            ln = readline(deadline)
            if ln is None:
                stop.set()
                sys.exit("\ntimeout")
            if ln.startswith("PHASE,") and ln.endswith(",ready"):
                name = ln.split(",")[2]
                print(f"\nPhase {k + 1}/{nphases}: {name}")
            elif ln.startswith("PHASE,") and ln.endswith(",measuring"):
                t0 = time.time()
            elif ln.startswith("T,"):
                if t0 is not None and not stop.is_set():
                    stop.set()  # the window has ended: the runs follow
                    print("\n  window ended, receiving the runs...")
                _, _, op, _, us = ln.split(",")
                runs.setdefault(op, []).append(int(us))
            elif ln.startswith("RESULT,"):
                result = ln
        stop.set()
        th.join()
        print(f"  {result}")
        f = result.split(",")
        names = [f[i] for i in range(4, len(f) - 3, 4)]
        txt = input("  meter power in W (empty: skip): ").strip()
        if not txt:
            continue
        if "-" in txt:
            lo, hi = (float(x) for x in txt.split("-"))
            p = (lo + hi) / 2
        else:
            p = float(txt)
        if k == 0:
            idle_w = p
        for op in names:
            t_ms = [u / 1000.0 for u in runs.get(op, [])]
            if not t_ms:
                continue
            mean = statistics.fmean(t_ms)
            sd = statistics.stdev(t_ms) if len(t_ms) > 1 else 0.0
            dp = p - idle_w if idle_w is not None else None
            rows.append([k + 1, name, p, op, len(t_ms), round(mean, 4), round(sd, 4),
                         round(p * mean, 4), round(p * sd, 4),
                         round(dp * mean, 4) if dp is not None else "",
                         round(abs(dp) * sd, 4) if dp is not None else ""])
            for i, t in enumerate(t_ms, 1):
                run_rows.append([k + 1, name, op, i, t, round(p * t, 4),
                                 round(dp * t, 4) if dp is not None else ""])
            print(f"  {op}: n={len(t_ms)}  t = {mean:.3f} ± {sd:.3f} ms  "
                  f"E_total = {p * mean:.3f} ± {p * sd:.3f} mJ"
                  + (f"  E_incr = {dp * mean:.3f} ± {abs(dp) * sd:.3f} mJ"
                     if dp is not None else ""))
    with open(out, "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["phase", "name", "power_w", "op", "runs", "t_mean_ms", "t_sd_ms",
                    "e_total_mean_mj", "e_total_sd_mj", "e_incr_mean_mj", "e_incr_sd_mj"])
        w.writerows(rows)
    with open(base + "_runs.csv", "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["phase", "name", "op", "run", "t_ms", "e_total_mj", "e_incr_mj"])
        w.writerows(run_rows)
    print(f"written {out} and {base}_runs.csv")


if __name__ == "__main__":
    main()
