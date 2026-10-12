# pqc-esp32-experiments

Benchmark and energy experiments for the five ESP32 PQC libraries
(fips203-mlkem-esp32, fips204-mldsa-esp32, fips205-slhdsa-esp32, sp800-208-lms-esp32, sp800-208-xmss-esp32) on the
M5StickC PLUS2 (ESP32-PICO-V3-02, 240 MHz, 2 MB PSRAM).

Every experiment exists twice:

| Experiment | Headless firmware (measurements) | GUI firmware (demonstration) |
|---|---|---|
| Uniform benchmark | `pqc-benchmark/` | `pqc-benchmark-gui/` |
| Energy phases | `pqc-energy-profile/` | `pqc-energy-gui/` |

Both firmwares of an experiment run the same workloads from
`libraries/PQCBench` and print the same serial output. Every operation runs
at least 10 times (`MIN_RUNS`). Every run is printed and saved, and results
are reported as mean ± sample standard deviation.

The two firmwares differ only in what else runs on the board:

- **Headless** (used for the numbers in the paper). The display is never
  initialised, so there is no backlight, no SPI traffic and no UI task. The
  workload runs on core 1, driven over the serial port by a host script.
- **GUI**. The live screen uses the same layout as the library demos
  (`libraries/PQCGui`). The buttons select and start a group or phase. The
  UI task runs on core 0 at 5 Hz. The display adds power and bus traffic,
  so use these firmwares for demonstrations, not for the reported figures.

## Shared libraries

- `libraries/PQCBench`: the workloads, with no display code.
  - `pqcbench::run(cmd)` runs the benchmark groups.
  - `pqcenergy::prepare/measure` run the energy phases.
  - Results go to an `Observer`. The headless sketches only print its lines;
    the GUIs also update the screen. Callbacks run outside the timed
    regions.
- `libraries/PQCGui`: the screen layout of all demos, header-only.
  - Teal header with the algorithm and the bold parameter set, then
    "HW" and the battery level.
  - Three timing cards; the running card is grey with orange text.
  - An info and status line, then the footer.
  - `pqcgui::dump()` sends a screenshot over the serial port (`#` in the
    GUIs).

## Setup

This repository holds only the experiments. The five libraries are separate
repositories:

| Library | Repository |
|---|---|
| ML-KEM (FIPS 203) | https://github.com/ANONYMIZED-FOR-REVIEW/fips203-mlkem-esp32 |
| ML-DSA (FIPS 204) | https://github.com/ANONYMIZED-FOR-REVIEW/fips204-mldsa-esp32 |
| SLH-DSA (FIPS 205) | https://github.com/ANONYMIZED-FOR-REVIEW/fips205-slhdsa-esp32 |
| LMS/HSS (SP 800-208) | https://github.com/ANONYMIZED-FOR-REVIEW/sp800-208-lms-esp32 |
| XMSS/XMSS^MT (SP 800-208) | https://github.com/ANONYMIZED-FOR-REVIEW/sp800-208-xmss-esp32 |

Clone them next to this repository, so that all six folders share one
parent directory:

```
for r in pqc-esp32-experiments fips203-mlkem-esp32 fips204-mldsa-esp32 \
         fips205-slhdsa-esp32 sp800-208-lms-esp32 sp800-208-xmss-esp32; do
  git clone https://github.com/ANONYMIZED-FOR-REVIEW/$r.git
done
```

```
<parent>/
  fips203-mlkem-esp32/
  fips204-mldsa-esp32/
  fips205-slhdsa-esp32/
  sp800-208-lms-esp32/
  sp800-208-xmss-esp32/
  pqc-esp32-experiments/   (this repository)
```

`build.sh` and `pqc-benchmark/codesize.sh` look for the libraries in the
parent directory. To keep them somewhere else, set `LIBS_DIR`, e.g.
`LIBS_DIR=~/src bash build.sh pqc-benchmark`.

Requirements, with the versions used for the results in
`pqc-benchmark/results/preliminary-2026-10-08/`:

| Component | Version |
|---|---|
| arduino-cli | 1.5.2 |
| Arduino-ESP32 core (`esp32:esp32`) | 3.3.11 (ESP-IDF 5.5.5) |
| Board | M5StickC PLUS2, FQBN `esp32:esp32:m5stack_stickc_plus2` |
| M5Unified | 0.2.21 (GUI firmwares only) |
| Python | 3.x with pyserial (3.5 used), for the host scripts |

Install the core and M5Unified with:

```
arduino-cli core install esp32:esp32@3.3.11
arduino-cli lib install M5Unified@0.2.21
pip install pyserial
```

## Build

```
bash build.sh pqc-benchmark                  # compile
bash build.sh pqc-benchmark /dev/ttyACM0     # compile and upload
bash build.sh pqc-benchmark /dev/ttyACM0 "-DMLDSA_ESP32_NO_REDUCE_RAM -DMLKEM_ESP32_USE_STACK"
                                          # alternative ML-KEM / ML-DSA memory modes (build B)
```

`build.sh` passes the five libraries, `libraries/PQCBench` and
`libraries/PQCGui`. It always sets `-DLMS_ESP32_HW_SHA
-DXMSS_ESP32_HW_SHA`; the software measurements switch the SHA
accelerator off at run time. Build outputs go to `build/`.

## Benchmark (`pqc-benchmark/`)

The complete campaign is a single command. It builds and flashes each
firmware variant, runs all groups, and writes the logs, firmware sources,
build sizes and tables into one folder per campaign:

```
bash pqc-benchmark/run_campaign.sh /dev/ttyACM0 pqc-benchmark/results/<date>
```

| Segment | Build | Groups | Time |
|---|---|---|---|
| A1 | A (library defaults) | radio on: `k d n`; radio off: `c` | ~0.6 h |
| B1 | B (`-DMLKEM_ESP32_USE_STACK -DMLDSA_ESP32_NO_REDUCE_RAM`) | radio on: `k d`; radio off: `c q` | ~0.6 h |
| B2 | B with `-DMEM_STACK_KB=8` | radio off: `r` | ~5 min |
| B3 | B | radio never switched on: `d` | ~5 min |
| A2 | A | radio on: `s l x` | ~12.5 h |
| A3 | A | radio off: `m` | ~5.3 h |

Each segment resets the board. B3 exists because ML-DSA-87 fast-sign needs
about 107 KB of internal heap: once Wi-Fi has been started, it keeps about
19 KB of internal RAM even after it is stopped, so these runs fail in B1. Single segments can be repeated, e.g.
`… run_campaign.sh PORT DIR A1`.

The steps can also be run by hand:

```
R=pqc-benchmark/results/<date>
python3 pqc-benchmark/run_bench.py /dev/ttyACM0 $R/run_A1.log wekdnoc 30000
python3 pqc-benchmark/analyze.py $R/run_*.log > $R/summary.md
python3 pqc-benchmark/sizes.py $R > $R/sizes.md   # also writes $R/sizes.csv
bash pqc-benchmark/codesize.sh > $R/codesize.csv
```

Groups:

| Command | Group |
|---|---|
| `k` | ML-KEM |
| `d` | ML-DSA |
| `s` | SLH-DSA |
| `l` | LMS/HSS |
| `x` | XMSS/XMSS^MT |
| `n` | NVS state costs |
| `c` | radio control (run with the radio off) |
| `m` | memory |
| `q` | memory, ML-KEM + ML-DSA |
| `r` | memory, ML-DSA |
| `w` / `o` | radio on / off |
| `e` | environment |

"Radio on" means the Wi-Fi station is started but not connected, so
`esp_fill_random()` returns true random numbers. The radio control group
`c` repeats all ML-KEM and ML-DSA sets and a subset of the hash-based sets
with the radio off.

Runs per operation:

| Operation | Runs |
|---|---|
| ML-KEM, ML-DSA | 1000 |
| SLH-DSA keygen and sign | 10 |
| SLH-DSA verify | 10 |
| LMS/XMSS key generation | 10 |
| LMS/XMSS sign and verify | 256 (32 for LMS H5 and the XMSS^MT sets with 2^5 signatures per tree) |
| NVS state: key generation, load, first and second sign after a load | 10 |
| NVS state: sign | 100 (+20 after loads) |
| NVS state: raw 8-byte write | 200 |
| memory group, each operation | 10 (fresh key each run) |

The full campaign takes about 19 hours. `analyze.py` combines the logs of
all segments.

`analyze.py` writes:

- `summary.md`: tables with mean ± SD; rows with fewer than 10 runs are
  flagged;
- `timings.csv`: one row per operation (n, mean, SD, median, min, max);
- `runs.csv`: every run;
- `memory.csv`: mean, SD and max per operation;
- `memory_runs.csv`: every memory run;
- `state.csv`.

The output format is described in `libraries/PQCBench/src/PQCBench.cpp`.
`results/` holds one folder per measurement campaign:

- `preliminary-2026-10-08/` is the first campaign. Some operations ran fewer
  than 10 times there, and its radio control covered fewer sets. See the
  README in that folder before using its numbers.

`pqc-benchmark-gui/` usage:

- Button B selects the group; button A runs it.
- The radio is switched on at start.
- Serial commands and the CSV output are the same as in the headless
  firmware.

## Energy (`pqc-energy-profile/`)

```
python3 pqc-energy-profile/run_energy.py /dev/ttyACM0 energy.csv
```

There are 13 phases. Each runs for at least 45 s, the time the meter needs
to settle and be read, and until every operation has completed at least 10
runs, so a phase can take longer than 45 s. The phases are:

- idle and busy loop;
- ML-KEM-768;
- ML-DSA-65;
- SLH-DSA-SHA2-128f in software and with the SHA accelerator, and
  SLH-DSA-SHAKE-128f;
- HSS H10+H10 and XMSS^MT 20/4_256, each in software and with the SHA
  accelerator;
- an NVS write loop;
- idle again, to check for drift.

The script starts each phase and shows the time in the phase, marking when
the reading has settled ("READ NOW"). It then asks for the power shown by
the inline meter (FNIRSI FNB-C2). It writes the energy per operation as
mean ± SD to `energy.csv`, and every run to `energy_runs.csv`:

- E_total = P × t_op;
- E_incremental = (P − P_idle) × t_op.

The meter gives one power value per phase, so the SD of the energy is the
SD of the run times scaled by P.

The battery must be fully charged, so that no charging current flows.

`pqc-energy-gui/` usage:

- Button B selects the first phase; button A runs from there to the end.
- Phases follow the same rule: at least 45 s and at least 10 runs per
  operation.
- The display stays on during every phase.

## License

Licensed under the **Apache License, Version 2.0** (see `LICENSE`). The
five PQC libraries are separate repositories with their own licenses and
credits.
