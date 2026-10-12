# Preliminary benchmark campaign (2026-10-08)

This was the first full benchmark run, made with the firmware in
`firmware/`, before the benchmark code moved into `libraries/PQCBench`. It
does **not** meet the current measurement rules (at least 10 runs per
operation, every run recorded). Treat its numbers as preliminary and
replace them with a new campaign before publication.

## Conditions

- M5StickC PLUS2 (ESP32-PICO-V3-02), CPU at 240 MHz, benchmark task on
  core 1.
- Arduino-ESP32 3.3.11 (ESP-IDF 5.5.5).
- Display never initialised.
- Radio on: the Wi-Fi station is started but not connected, which gives
  true random numbers. Group `c` and the memory runs used radio off.
- All builds used `-DLMS_ESP32_HW_SHA -DXMSS_ESP32_HW_SHA`.

## Logs

| Log | Build | Groups | Firmware |
|---|---|---|---|
| `run_A.log` | A: ML-KEM heap mode, ML-DSA reduced-RAM mode (defaults) | `w e k d l n x s o c m` | `bench_runA_runB.cpp` |
| `run_B.log` | B: `-DMLKEM_ESP32_USE_STACK -DMLDSA_ESP32_NO_REDUCE_RAM` | `w e k d o q` | `bench_runA_runB.cpp` |
| `run_A2_mem.log` | A, memory rerun | `e o m` | `bench_runA2_runB2.cpp` |
| `run_B2.log` | B, memory rerun | `e o d q` | `bench_runA2_runB2.cpp` |
| `run_B3_dsamem.log` | B with `-DMEM_STACK_KB=8` | `e o r` | `bench_runB3.cpp` |

Notes on the firmware files:

- `bench_runB3.cpp` adds only group `r` and the `MEM_STACK_KB` option to
  `bench_runA2_runB2.cpp`. With the default `MEM_STACK_KB=32` both behave
  the same for groups `m` and `q`.
- `firmware/*.compile.log` give the flash and static RAM use of builds A
  and B.

The tables were made with
`analyze.py run_A.log run_B.log run_A2_mem.log run_B2.log run_B3_dsamem.log`
(the version at the time). They are `summary.md`, `timings.csv`,
`memory.csv` and `state.csv`. `sizes.md` / `sizes.csv` come from
`sizes.py`, and `codesize.csv` from `codesize.sh`.

## Known limitations

1. **Fewer than 10 runs for some operations:**
   - SLH-DSA "s" sets: keygen and sign 3 runs each.
   - LMS key generation: 1 run (5 for H5).
   - XMSS key generation: 1 run (3 for the sets with 2^5 signatures per tree).
   - NVS state group: keygen, load and the signatures after a load ran once.
2. **No per-run data.** Only summaries were recorded (mean, SD, median, min,
   max).
3. **Memory: one run per operation.**
   - The memory groups in `run_A.log` and `run_B.log` are invalid. They used
     a 64 KB measurement task that starved the heap, and recorded no return
     codes; for example, ML-DSA-87 keygen failed silently.
   - `run_A2_mem.log`, `run_B2.log` and `run_B3_dsamem.log` supersede them.
     `analyze.py` uses only rows with return code 0.
4. **Incomplete radio control.**
   - Group `c` covered only ML-KEM-768, ML-DSA-44, SLH-DSA-SHA2-128f (SW and
     HW), LMS H10 W4 (HW) and XMSS^MT 20/4_256 (HW).
   - Its radio-off rows were measured at a different point in the run than
     the radio-on rows they are compared with.
   - The resulting +2.9–4.8% for ML-KEM-768 underestimates the overhead. A
     matched comparison on 2026-10-09 (same binary, radio on vs off) gave
     +3.9–7.2% for ML-KEM-768 and up to +10.7% for ML-KEM-512.
   - That extra time was nearly constant per operation: about 250, 480 and
     640 µs for keygen, encaps and decaps.
5. **ML-DSA-87 fast-sign fails with the radio on.** In build B, ML-DSA-87
   fast-sign failed in 1000 of 1000 runs, because it needs about 107 KB of
   internal heap and Wi-Fi takes about 47 KB.
6. **Optimistic LMS H15 signing time.** All 256 signatures fell in the
   first, precomputed subtree, so the LMS H15 sign figure (2.66 ms) leaves
   out the subtree-switch cost.

`sizes.*` and `codesize.csv` do not depend on these runs. They stay valid
as long as the library code is unchanged.
