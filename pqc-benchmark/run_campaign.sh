#!/bin/bash
# Copyright (c) 2026 The pqc-esp32-experiments authors
# SPDX-License-Identifier: Apache-2.0
#
# Runs the complete benchmark campaign on one board and writes everything
# (logs, firmware sources and build sizes, tables) into OUTDIR.
#
# Usage: bash pqc-benchmark/run_campaign.sh PORT OUTDIR [SEGMENT ...]
#   e.g. bash pqc-benchmark/run_campaign.sh /dev/ttyACM0 pqc-benchmark/results/2026-10-09
#
# Segments (default: all, in this order; each one resets the board):
#   A1  build A, radio on: k d n; radio off: c (radio control)            ~0.6 h
#   B1  build B, radio on: k d; radio off: c, q (memory KEM + DSA)        ~0.6 h
#   B2  build B with an 8 KB measurement task, radio off: r (memory DSA)  ~5 min
#   B3  build B, radio never switched on: d                               ~5 min
#       (ML-DSA-87 fast-sign needs ~107 KB of internal heap: once Wi-Fi has
#       been started it keeps ~19 KB even after it is stopped, so the B1
#       fast-sign rows of ML-DSA-87 fail; B3 measures them from a clean boot)
#   A2  build A, radio on: s l x                                          ~12.5 h
#   A3  build A, radio off: m (memory, all schemes)                       ~5.3 h
# Build A: library defaults (ML-KEM heap mode, ML-DSA reduced-RAM mode).
# Build B: -DMLKEM_ESP32_USE_STACK -DMLDSA_ESP32_NO_REDUCE_RAM.
# All builds: -DLMS_ESP32_HW_SHA -DXMSS_ESP32_HW_SHA (added by build.sh).
#
# A failed segment (crash or timeout, see run_bench.py) is recorded in
# OUTDIR/campaign.log and the campaign continues with the next segment.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
EXP="$(cd "$HERE/.." && pwd)"
PORT="${1:?usage: run_campaign.sh PORT OUTDIR [SEGMENT ...]}"
OUT="${2:?usage: run_campaign.sh PORT OUTDIR [SEGMENT ...]}"
shift 2
SEGMENTS="${*:-A1 B1 B2 B3 A2 A3}"
GROUP_TIMEOUT=30000  # s per group (the longest group takes ~5.3 h)
B_FLAGS="-DMLKEM_ESP32_USE_STACK -DMLDSA_ESP32_NO_REDUCE_RAM"

mkdir -p "$OUT/firmware"
OUT="$(cd "$OUT" && pwd)"
LOG="$OUT/campaign.log"
note() { echo "$(date '+%Y-%m-%d %H:%M:%S') $*" | tee -a "$LOG"; }

# firmware sources used for this campaign
cp "$EXP"/libraries/PQCBench/src/* "$EXP/pqc-benchmark/pqc-benchmark.ino" "$OUT/firmware/"
(cd "$EXP" && sha256sum libraries/PQCBench/src/* pqc-benchmark/pqc-benchmark.ino) \
  > "$OUT/firmware/sources.sha256"

# object and code sizes (host only; runs alongside the device segments)
python3 "$HERE/sizes.py" "$OUT" > "$OUT/sizes.md" 2>> "$LOG" &
SIZES_PID=$!
bash "$HERE/codesize.sh" > "$OUT/codesize.csv" 2>> "$LOG" &
CODESIZE_PID=$!

segment() {  # name flags commands
  local name="$1" flags="$2" cmds="$3"
  note "segment $name: build (flags: ${flags:-none})"
  if ! BUILD="/tmp/pqc-campaign/$name" bash "$EXP/build.sh" pqc-benchmark "$PORT" "$flags" \
      > "$OUT/firmware/$name.build.log" 2>&1; then
    note "segment $name: BUILD/UPLOAD FAILED (see firmware/$name.build.log)"
    return
  fi
  note "segment $name: $(grep -h 'Sketch uses\|Global variables' "$OUT/firmware/$name.build.log" | tr '\n' ' ')"
  note "segment $name: run '$cmds'"
  python3 "$HERE/run_bench.py" "$PORT" "$OUT/run_$name.log" "$cmds" "$GROUP_TIMEOUT"
  local rc=$?
  if [ $rc -eq 0 ]; then note "segment $name: done"; else note "segment $name: FAILED (rc $rc)"; fi
}

note "campaign start: port $PORT, segments: $SEGMENTS"
for s in $SEGMENTS; do
  case "$s" in
    A1) segment A1 "" "wekdnoc" ;;
    B1) segment B1 "$B_FLAGS" "wekdocq" ;;
    B2) segment B2 "$B_FLAGS -DMEM_STACK_KB=8" "eor" ;;
    B3) segment B3 "$B_FLAGS" "ed" ;;
    A2) segment A2 "" "weslx" ;;
    A3) segment A3 "" "eom" ;;
    *) note "unknown segment $s" ;;
  esac
done

wait $SIZES_PID $CODESIZE_PID
logs=$(ls "$OUT"/run_*.log 2>/dev/null)
if [ -n "$logs" ]; then
  python3 "$HERE/analyze.py" $logs > "$OUT/summary.md" 2>> "$LOG" \
    && note "tables written: summary.md timings.csv runs.csv memory.csv memory_runs.csv state.csv"
fi
note "campaign end"
