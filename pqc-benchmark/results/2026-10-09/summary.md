# pqc-benchmark summary

- run_A1: `E,cpu_mhz=240,idf=v5.5.5,arduino=3.3.11,radio=off,mlkem=heap,mldsa=reduce-ram,slh_hw=1,lms_hw=1,xmss_hw=1,free_heap=2218948,free_internal=123844,n_fast=1000,n_slh_f=10,n_slh_s=10,n_hbs_sign=256,min_runs=10,nvs_used=277,nvs_free=353,nvs_total=630`
- run_A2: `E,cpu_mhz=240,idf=v5.5.5,arduino=3.3.11,radio=on,mlkem=heap,mldsa=reduce-ram,slh_hw=1,lms_hw=1,xmss_hw=1,free_heap=2186816,free_internal=96196,n_fast=1000,n_slh_f=10,n_slh_s=10,n_hbs_sign=256,min_runs=10,nvs_used=277,nvs_free=353,nvs_total=630`
- run_A3: `E,cpu_mhz=240,idf=v5.5.5,arduino=3.3.11,radio=off,mlkem=heap,mldsa=reduce-ram,slh_hw=1,lms_hw=1,xmss_hw=1,free_heap=2237876,free_internal=142772,n_fast=1000,n_slh_f=10,n_slh_s=10,n_hbs_sign=256,min_runs=10,nvs_used=277,nvs_free=353,nvs_total=630`
- run_B1: `E,cpu_mhz=240,idf=v5.5.5,arduino=3.3.11,radio=off,mlkem=stack,mldsa=fast-sign,slh_hw=1,lms_hw=1,xmss_hw=1,free_heap=2219012,free_internal=123908,n_fast=1000,n_slh_f=10,n_slh_s=10,n_hbs_sign=256,min_runs=10,nvs_used=277,nvs_free=353,nvs_total=630`
- run_B2: `E,cpu_mhz=240,idf=v5.5.5,arduino=3.3.11,radio=off,mlkem=stack,mldsa=fast-sign,slh_hw=1,lms_hw=1,xmss_hw=1,free_heap=2237876,free_internal=142772,n_fast=1000,n_slh_f=10,n_slh_s=10,n_hbs_sign=256,min_runs=10,nvs_used=277,nvs_free=353,nvs_total=630`
- run_B3: `E,cpu_mhz=240,idf=v5.5.5,arduino=3.3.11,radio=off,mlkem=stack,mldsa=fast-sign,slh_hw=1,lms_hw=1,xmss_hw=1,free_heap=2237876,free_internal=142772,n_fast=1000,n_slh_f=10,n_slh_s=10,n_hbs_sign=256,min_runs=10,nvs_used=277,nvs_free=353,nvs_total=630`

Times are mean ± sample standard deviation over N runs.

All summary rows match their 97714 per-run lines (`runs.csv`).

**Rows with failed operations** (4; excluded from the comparison tables):

- run_B1 group d: ML-DSA-87 fast-sign sign, radio on: 1000 of 1000 failed
- run_B1 group d: ML-DSA-87 fast-sign verify, radio on: 1000 of 1000 failed
- run_B1 group c: ML-DSA-87 fast-sign sign, radio off: 1000 of 1000 failed
- run_B1 group c: ML-DSA-87 fast-sign verify, radio off: 1000 of 1000 failed


## ML-KEM (run_A1, radio on)

| Scheme | Impl | Operation | N | Mean ± SD | Median | Min | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| ML-KEM-512 | heap | keygen | 1000 | 4.05 ms ± 0.02 ms | 4.05 ms | 4.04 ms | 4.34 ms | 972 k | 0 |
| ML-KEM-512 | heap | encaps | 1000 | 5.13 ms ± 0.02 ms | 5.12 ms | 5.12 ms | 5.31 ms | 1.23 M | 0 |
| ML-KEM-512 | heap | decaps | 1000 | 6.62 ms ± 0.02 ms | 6.61 ms | 6.60 ms | 6.80 ms | 1.59 M | 0 |
| ML-KEM-768 | heap | keygen | 1000 | 6.41 ms ± 0.03 ms | 6.39 ms | 6.39 ms | 6.62 ms | 1.54 M | 0 |
| ML-KEM-768 | heap | encaps | 1000 | 7.86 ms ± 0.03 ms | 7.85 ms | 7.84 ms | 8.04 ms | 1.89 M | 0 |
| ML-KEM-768 | heap | decaps | 1000 | 9.85 ms ± 0.03 ms | 9.84 ms | 9.83 ms | 10.0 ms | 2.36 M | 0 |
| ML-KEM-1024 | heap | keygen | 1000 | 9.81 ms ± 0.04 ms | 9.79 ms | 9.78 ms | 10.0 ms | 2.35 M | 0 |
| ML-KEM-1024 | heap | encaps | 1000 | 11.5 ms ± 0.04 ms | 11.5 ms | 11.5 ms | 11.7 ms | 2.77 M | 0 |
| ML-KEM-1024 | heap | decaps | 1000 | 14.0 ms ± 0.04 ms | 13.9 ms | 13.9 ms | 14.2 ms | 3.35 M | 0 |

## ML-DSA (run_A1, radio on)

| Scheme | Impl | Operation | N | Mean ± SD | Median | Min | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| ML-DSA-44 | reduce-ram | keygen | 1000 | 13.1 ms ± 0.13 ms | 13.1 ms | 12.8 ms | 13.5 ms | 3.15 M | 0 |
| ML-DSA-44 | reduce-ram | sign | 1000 | 77.0 ms ± 60.9 ms | 56.1 ms | 21.3 ms | 412.4 ms | 18.49 M | 0 |
| ML-DSA-44 | reduce-ram | verify | 1000 | 13.9 ms ± 0.02 ms | 13.8 ms | 13.8 ms | 13.9 ms | 3.32 M | 0 |
| ML-DSA-65 | reduce-ram | keygen | 1000 | 22.7 ms ± 0.02 ms | 22.7 ms | 22.7 ms | 22.9 ms | 5.46 M | 0 |
| ML-DSA-65 | reduce-ram | sign | 1000 | 141.8 ms ± 119.8 ms | 108.4 ms | 32.5 ms | 905.5 ms | 34.03 M | 0 |
| ML-DSA-65 | reduce-ram | verify | 1000 | 22.9 ms ± 0.01 ms | 22.9 ms | 22.9 ms | 22.9 ms | 5.49 M | 0 |
| ML-DSA-87 | reduce-ram | keygen | 1000 | 38.1 ms ± 0.18 ms | 38.1 ms | 37.6 ms | 38.7 ms | 9.15 M | 0 |
| ML-DSA-87 | reduce-ram | sign | 1000 | 180.6 ms ± 145.6 ms | 139.6 ms | 52.2 ms | 1.03 s | 43.35 M | 0 |
| ML-DSA-87 | reduce-ram | verify | 1000 | 38.8 ms ± 0.01 ms | 38.8 ms | 38.8 ms | 38.9 ms | 9.32 M | 0 |

## SLH-DSA (run_A2, radio on)

| Scheme | Impl | Operation | N | Mean ± SD | Median | Min | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| SLH-DSA-SHA2-128f | SW | keygen | 10 | 111.1 ms ± 0.39 ms | 110.8 ms | 110.8 ms | 111.8 ms | 26.66 M | 0 |
| SLH-DSA-SHA2-128f | SW | sign | 10 | 2.61 s ± 1.20 ms | 2.61 s | 2.61 s | 2.61 s | 626.82 M | 0 |
| SLH-DSA-SHA2-128f | SW | verify | 10 | 153.2 ms ± 0.48 ms | 152.9 ms | 152.9 ms | 154.1 ms | 36.78 M | 0 |
| SLH-DSA-SHA2-128f | HW | keygen | 10 | 45.4 ms ± 0.36 ms | 45.2 ms | 45.2 ms | 46.4 ms | 10.88 M | 0 |
| SLH-DSA-SHA2-128f | HW | sign | 10 | 1.06 s ± 0.70 ms | 1.06 s | 1.06 s | 1.06 s | 255.07 M | 0 |
| SLH-DSA-SHA2-128f | HW | verify | 10 | 62.1 ms ± 0.36 ms | 62.0 ms | 62.0 ms | 63.2 ms | 14.91 M | 0 |
| SLH-DSA-SHA2-192f | SW | keygen | 10 | 166.5 ms ± 0.48 ms | 166.3 ms | 166.0 ms | 167.2 ms | 39.96 M | 0 |
| SLH-DSA-SHA2-192f | SW | sign | 10 | 4.87 s ± 0.66 ms | 4.87 s | 4.87 s | 4.87 s | 1.17 G | 0 |
| SLH-DSA-SHA2-192f | SW | verify | 10 | 263.3 ms ± 0.50 ms | 263.1 ms | 262.8 ms | 263.9 ms | 63.19 M | 0 |
| SLH-DSA-SHA2-192f | HW | keygen | 10 | 72.6 ms ± 0.35 ms | 72.5 ms | 72.5 ms | 73.6 ms | 17.43 M | 0 |
| SLH-DSA-SHA2-192f | HW | sign | 10 | 1.98 s ± 0.34 ms | 1.98 s | 1.98 s | 1.98 s | 474.62 M | 0 |
| SLH-DSA-SHA2-192f | HW | verify | 10 | 103.8 ms ± 0.53 ms | 103.5 ms | 103.5 ms | 104.6 ms | 24.91 M | 0 |
| SLH-DSA-SHA2-256f | SW | keygen | 10 | 442.8 ms ± 0.42 ms | 442.9 ms | 441.9 ms | 443.3 ms | 106.27 M | 0 |
| SLH-DSA-SHA2-256f | SW | sign | 10 | 10.12 s ± 1.11 ms | 10.12 s | 10.11 s | 10.12 s | 2.43 G | 0 |
| SLH-DSA-SHA2-256f | SW | verify | 10 | 278.3 ms ± 0.57 ms | 278.7 ms | 277.6 ms | 278.8 ms | 66.79 M | 0 |
| SLH-DSA-SHA2-256f | HW | keygen | 10 | 212.6 ms ± 0.56 ms | 212.2 ms | 212.1 ms | 213.3 ms | 51.01 M | 0 |
| SLH-DSA-SHA2-256f | HW | sign | 10 | 4.48 s ± 0.38 ms | 4.48 s | 4.48 s | 4.48 s | 1.07 G | 0 |
| SLH-DSA-SHA2-256f | HW | verify | 10 | 120.2 ms ± 0.47 ms | 120.0 ms | 119.9 ms | 121.1 ms | 28.84 M | 0 |
| SLH-DSA-SHAKE-128f | SW | keygen | 10 | 525.3 ms ± 0.41 ms | 525.5 ms | 524.7 ms | 525.7 ms | 126.06 M | 0 |
| SLH-DSA-SHAKE-128f | SW | sign | 10 | 12.32 s ± 2.87 ms | 12.32 s | 12.31 s | 12.32 s | 2.96 G | 0 |
| SLH-DSA-SHAKE-128f | SW | verify | 10 | 724.6 ms ± 0.59 ms | 724.4 ms | 724.0 ms | 725.5 ms | 173.91 M | 0 |
| SLH-DSA-SHAKE-192f | SW | keygen | 10 | 768.8 ms ± 0.50 ms | 768.8 ms | 768.0 ms | 769.7 ms | 184.51 M | 0 |
| SLH-DSA-SHAKE-192f | SW | sign | 10 | 20.14 s ± 2.04 ms | 20.14 s | 20.13 s | 20.14 s | 4.83 G | 0 |
| SLH-DSA-SHAKE-192f | SW | verify | 10 | 1.09 s ± 0.69 ms | 1.09 s | 1.09 s | 1.09 s | 261.44 M | 0 |
| SLH-DSA-SHAKE-256f | SW | keygen | 10 | 2.03 s ± 0.41 ms | 2.03 s | 2.03 s | 2.03 s | 487.28 M | 0 |
| SLH-DSA-SHAKE-256f | SW | sign | 10 | 41.58 s ± 6.56 ms | 41.59 s | 41.57 s | 41.59 s | 9.98 G | 0 |
| SLH-DSA-SHAKE-256f | SW | verify | 10 | 1.11 s ± 0.59 ms | 1.11 s | 1.11 s | 1.11 s | 267.15 M | 0 |
| SLH-DSA-SHA2-128s | SW | keygen | 10 | 7.11 s ± 0.53 ms | 7.11 s | 7.11 s | 7.11 s | 1.71 G | 0 |
| SLH-DSA-SHA2-128s | SW | sign | 10 | 54.43 s ± 0.85 ms | 54.43 s | 54.43 s | 54.43 s | 13.06 G | 0 |
| SLH-DSA-SHA2-128s | SW | verify | 10 | 50.6 ms ± 0.35 ms | 50.5 ms | 50.5 ms | 51.6 ms | 12.14 M | 0 |
| SLH-DSA-SHA2-128s | HW | keygen | 10 | 2.90 s ± 0.09 ms | 2.90 s | 2.90 s | 2.90 s | 696.38 M | 0 |
| SLH-DSA-SHA2-128s | HW | sign | 10 | 22.09 s ± 0.45 ms | 22.09 s | 22.09 s | 22.09 s | 5.30 G | 0 |
| SLH-DSA-SHA2-128s | HW | verify | 10 | 23.2 ms ± 0.02 ms | 23.2 ms | 23.2 ms | 23.2 ms | 5.57 M | 0 |
| SLH-DSA-SHA2-192s | SW | keygen | 10 | 10.64 s ± 0.22 ms | 10.64 s | 10.64 s | 10.64 s | 2.55 G | 0 |
| SLH-DSA-SHA2-192s | SW | sign | 10 | 1.92 min ± 7.63 ms | 1.92 min | 1.92 min | 1.92 min | 27.64 G | 0 |
| SLH-DSA-SHA2-192s | SW | verify | 10 | 101.2 ms ± 0.44 ms | 101.0 ms | 101.0 ms | 102.1 ms | 24.30 M | 0 |
| SLH-DSA-SHA2-192s | HW | keygen | 10 | 4.65 s ± 0.59 ms | 4.65 s | 4.65 s | 4.65 s | 1.12 G | 0 |
| SLH-DSA-SHA2-192s | HW | sign | 10 | 45.08 s ± 0.60 ms | 45.08 s | 45.08 s | 45.08 s | 10.82 G | 0 |
| SLH-DSA-SHA2-192s | HW | verify | 10 | 37.7 ms ± 0.34 ms | 37.6 ms | 37.5 ms | 38.6 ms | 9.04 M | 0 |
| SLH-DSA-SHA2-256s | SW | keygen | 10 | 7.08 s ± 0.77 ms | 7.08 s | 7.08 s | 7.09 s | 1.70 G | 0 |
| SLH-DSA-SHA2-256s | SW | sign | 10 | 1.82 min ± 8.02 ms | 1.82 min | 1.82 min | 1.82 min | 26.16 G | 0 |
| SLH-DSA-SHA2-256s | SW | verify | 10 | 145.3 ms ± 0.53 ms | 145.0 ms | 144.9 ms | 146.1 ms | 34.87 M | 0 |
| SLH-DSA-SHA2-256s | HW | keygen | 10 | 3.40 s ± 0.52 ms | 3.40 s | 3.40 s | 3.40 s | 815.99 M | 0 |
| SLH-DSA-SHA2-256s | HW | sign | 10 | 44.63 s ± 0.70 ms | 44.63 s | 44.63 s | 44.63 s | 10.71 G | 0 |
| SLH-DSA-SHA2-256s | HW | verify | 10 | 62.8 ms ± 0.37 ms | 62.7 ms | 62.7 ms | 63.8 ms | 15.07 M | 0 |
| SLH-DSA-SHAKE-128s | SW | keygen | 10 | 33.62 s ± 1.66 ms | 33.62 s | 33.62 s | 33.62 s | 8.07 G | 0 |
| SLH-DSA-SHAKE-128s | SW | sign | 10 | 4.28 min ± 3.90 ms | 4.28 min | 4.28 min | 4.28 min | 61.68 G | 0 |
| SLH-DSA-SHAKE-128s | SW | verify | 10 | 235.5 ms ± 0.42 ms | 235.3 ms | 235.1 ms | 236.1 ms | 56.51 M | 0 |
| SLH-DSA-SHAKE-192s | SW | keygen | 10 | 49.21 s ± 1.52 ms | 49.21 s | 49.21 s | 49.21 s | 11.81 G | 0 |
| SLH-DSA-SHAKE-192s | SW | sign | 10 | 7.54 min ± 3.14 ms | 7.54 min | 7.54 min | 7.54 min | 108.59 G | 0 |
| SLH-DSA-SHAKE-192s | SW | verify | 10 | 358.0 ms ± 0.51 ms | 358.2 ms | 357.3 ms | 358.4 ms | 85.91 M | 0 |
| SLH-DSA-SHAKE-256s | SW | keygen | 10 | 32.49 s ± 0.83 ms | 32.49 s | 32.48 s | 32.49 s | 7.80 G | 0 |
| SLH-DSA-SHAKE-256s | SW | sign | 10 | 6.72 min ± 4.14 ms | 6.72 min | 6.72 min | 6.72 min | 96.75 G | 0 |
| SLH-DSA-SHAKE-256s | SW | verify | 10 | 548.0 ms ± 0.54 ms | 548.1 ms | 547.3 ms | 549.3 ms | 131.53 M | 0 |

## LMS / HSS (run_A2, radio on)

| Scheme | Impl | Operation | N | Mean ± SD | Median | Min | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| LMS-SHA256-M32-H5-W1 | SW | keygen | 10 | 513.0 ms ± 0.02 ms | 513.0 ms | 513.0 ms | 513.0 ms | 123.12 M | 0 |
| LMS-SHA256-M32-H5-W1 | SW | sign | 32 | 18.1 ms ± 8.23 ms | 18.1 ms | 9.59 ms | 27.1 ms | 4.34 M | 0 |
| LMS-SHA256-M32-H5-W1 | SW | verify | 32 | 8.02 ms ± 0.19 ms | 7.97 ms | 7.66 ms | 8.45 ms | 1.92 M | 0 |
| LMS-SHA256-M32-H5-W1 | HW | keygen | 10 | 221.5 ms ± 0.45 ms | 221.5 ms | 221.1 ms | 222.2 ms | 53.17 M | 0 |
| LMS-SHA256-M32-H5-W1 | HW | sign | 32 | 6.64 ms ± 3.52 ms | 6.78 ms | 3.09 ms | 10.7 ms | 1.59 M | 0 |
| LMS-SHA256-M32-H5-W1 | HW | verify | 32 | 5.79 ms ± 0.04 ms | 5.79 ms | 5.73 ms | 5.85 ms | 1.39 M | 0 |
| LMS-SHA256-M32-H5-W2 | SW | keygen | 10 | 439.1 ms ± 0.39 ms | 439.0 ms | 438.3 ms | 439.5 ms | 105.38 M | 0 |
| LMS-SHA256-M32-H5-W2 | SW | sign | 32 | 14.7 ms ± 6.95 ms | 14.9 ms | 7.27 ms | 22.2 ms | 3.53 M | 0 |
| LMS-SHA256-M32-H5-W2 | SW | verify | 32 | 7.09 ms ± 0.28 ms | 7.14 ms | 6.45 ms | 7.64 ms | 1.70 M | 0 |
| LMS-SHA256-M32-H5-W2 | HW | keygen | 10 | 146.3 ms ± 0.35 ms | 146.1 ms | 146.1 ms | 147.1 ms | 35.11 M | 0 |
| LMS-SHA256-M32-H5-W2 | HW | sign | 32 | 4.47 ms ± 2.34 ms | 4.56 ms | 2.06 ms | 7.85 ms | 1.07 M | 0 |
| LMS-SHA256-M32-H5-W2 | HW | verify | 32 | 3.62 ms ± 0.05 ms | 3.62 ms | 3.54 ms | 3.73 ms | 869 k | 0 |
| LMS-SHA256-M32-H5-W4 | SW | keygen | 10 | 764.5 ms ± 0.37 ms | 764.7 ms | 764.1 ms | 765.0 ms | 183.48 M | 0 |
| LMS-SHA256-M32-H5-W4 | SW | sign | 32 | 24.2 ms ± 12.0 ms | 24.2 ms | 11.1 ms | 37.2 ms | 5.81 M | 0 |
| LMS-SHA256-M32-H5-W4 | SW | verify | 32 | 12.4 ms ± 0.70 ms | 12.5 ms | 11.1 ms | 13.6 ms | 2.98 M | 0 |
| LMS-SHA256-M32-H5-W4 | HW | keygen | 10 | 174.2 ms ± 0.46 ms | 173.9 ms | 173.9 ms | 175.1 ms | 41.82 M | 0 |
| LMS-SHA256-M32-H5-W4 | HW | sign | 32 | 5.40 ms ± 2.69 ms | 5.44 ms | 2.29 ms | 8.60 ms | 1.30 M | 0 |
| LMS-SHA256-M32-H5-W4 | HW | verify | 32 | 3.56 ms ± 0.19 ms | 3.58 ms | 3.20 ms | 3.94 ms | 855 k | 0 |
| LMS-SHA256-M32-H5-W8 | SW | keygen | 10 | 5.88 s ± 0.32 ms | 5.88 s | 5.88 s | 5.88 s | 1.41 G | 0 |
| LMS-SHA256-M32-H5-W8 | SW | sign | 32 | 183.4 ms ± 92.4 ms | 178.9 ms | 81.4 ms | 287.3 ms | 44.01 M | 0 |
| LMS-SHA256-M32-H5-W8 | SW | verify | 32 | 92.7 ms ± 7.54 ms | 92.1 ms | 81.3 ms | 113.5 ms | 22.26 M | 0 |
| LMS-SHA256-M32-H5-W8 | HW | keygen | 10 | 1.08 s ± 0.22 ms | 1.08 s | 1.08 s | 1.08 s | 259.56 M | 0 |
| LMS-SHA256-M32-H5-W8 | HW | sign | 32 | 33.4 ms ± 17.1 ms | 34.1 ms | 13.0 ms | 53.4 ms | 8.01 M | 0 |
| LMS-SHA256-M32-H5-W8 | HW | verify | 32 | 17.8 ms ± 1.60 ms | 17.5 ms | 14.5 ms | 21.3 ms | 4.28 M | 0 |
| LMS-SHA256-M32-H10-W2 | SW | keygen | 10 | 14.04 s ± 0.78 ms | 14.04 s | 14.04 s | 14.04 s | 3.37 G | 0 |
| LMS-SHA256-M32-H10-W2 | SW | sign | 256 | 19.9 ms ± 4.57 ms | 21.4 ms | 7.40 ms | 24.0 ms | 4.76 M | 0 |
| LMS-SHA256-M32-H10-W2 | SW | verify | 256 | 7.38 ms ± 0.29 ms | 7.44 ms | 6.62 ms | 8.13 ms | 1.77 M | 0 |
| LMS-SHA256-M32-H10-W2 | HW | keygen | 10 | 4.67 s ± 0.29 ms | 4.67 s | 4.67 s | 4.67 s | 1.12 G | 0 |
| LMS-SHA256-M32-H10-W2 | HW | sign | 256 | 6.15 ms ± 1.54 ms | 6.64 ms | 2.06 ms | 8.66 ms | 1.48 M | 0 |
| LMS-SHA256-M32-H10-W2 | HW | verify | 256 | 3.92 ms ± 0.05 ms | 3.93 ms | 3.78 ms | 4.05 ms | 942 k | 0 |
| LMS-SHA256-M32-H10-W4 | SW | keygen | 10 | 24.45 s ± 0.21 ms | 24.45 s | 24.45 s | 24.45 s | 5.87 G | 0 |
| LMS-SHA256-M32-H10-W4 | SW | sign | 256 | 33.3 ms ± 7.87 ms | 36.1 ms | 10.8 ms | 40.5 ms | 8.00 M | 0 |
| LMS-SHA256-M32-H10-W4 | SW | verify | 256 | 12.6 ms ± 0.85 ms | 12.7 ms | 10.8 ms | 14.9 ms | 3.02 M | 0 |
| LMS-SHA256-M32-H10-W4 | HW | keygen | 10 | 5.56 s ± 0.27 ms | 5.56 s | 5.56 s | 5.56 s | 1.33 G | 0 |
| LMS-SHA256-M32-H10-W4 | HW | sign | 256 | 7.42 ms ± 1.84 ms | 8.05 ms | 2.40 ms | 10.0 ms | 1.78 M | 0 |
| LMS-SHA256-M32-H10-W4 | HW | verify | 256 | 3.87 ms ± 0.16 ms | 3.83 ms | 3.50 ms | 4.29 ms | 930 k | 0 |
| LMS-SHA256-M32-H10-W8 | SW | keygen | 10 | 3.14 min ± 0.27 ms | 3.14 min | 3.14 min | 3.14 min | 45.15 G | 0 |
| LMS-SHA256-M32-H10-W8 | SW | sign | 256 | 251.5 ms ± 61.8 ms | 271.1 ms | 70.6 ms | 297.0 ms | 60.36 M | 0 |
| LMS-SHA256-M32-H10-W8 | SW | verify | 256 | 93.8 ms ± 8.89 ms | 92.4 ms | 70.9 ms | 113.8 ms | 22.52 M | 0 |
| LMS-SHA256-M32-H10-W8 | HW | keygen | 10 | 34.61 s ± 0.38 ms | 34.61 s | 34.61 s | 34.61 s | 8.31 G | 0 |
| LMS-SHA256-M32-H10-W8 | HW | sign | 256 | 45.9 ms ± 11.3 ms | 49.5 ms | 13.9 ms | 55.5 ms | 11.01 M | 0 |
| LMS-SHA256-M32-H10-W8 | HW | verify | 256 | 18.3 ms ± 1.56 ms | 18.7 ms | 14.8 ms | 22.6 ms | 4.40 M | 0 |
| LMS-SHA256-M24-H10-W4 | SW | keygen | 10 | 18.28 s ± 0.19 ms | 18.28 s | 18.28 s | 18.28 s | 4.39 G | 0 |
| LMS-SHA256-M24-H10-W4 | SW | sign | 256 | 24.9 ms ± 5.96 ms | 27.0 ms | 7.66 ms | 30.3 ms | 5.96 M | 0 |
| LMS-SHA256-M24-H10-W4 | SW | verify | 256 | 9.60 ms ± 0.63 ms | 9.57 ms | 8.00 ms | 11.8 ms | 2.30 M | 0 |
| LMS-SHA256-M24-H10-W4 | HW | keygen | 10 | 3.92 s ± 1.82 ms | 3.92 s | 3.91 s | 3.92 s | 939.90 M | 0 |
| LMS-SHA256-M24-H10-W4 | HW | sign | 256 | 5.24 ms ± 1.32 ms | 5.66 ms | 1.53 ms | 7.69 ms | 1.26 M | 0 |
| LMS-SHA256-M24-H10-W4 | HW | verify | 256 | 2.88 ms ± 0.12 ms | 2.87 ms | 2.55 ms | 3.23 ms | 691 k | 0 |
| LMS-SHA256-M24-H10-W8 | SW | keygen | 10 | 2.38 min ± 0.27 ms | 2.38 min | 2.38 min | 2.38 min | 34.24 G | 0 |
| LMS-SHA256-M24-H10-W8 | SW | sign | 256 | 188.9 ms ± 46.4 ms | 203.9 ms | 53.9 ms | 225.6 ms | 45.34 M | 0 |
| LMS-SHA256-M24-H10-W8 | SW | verify | 256 | 73.2 ms ± 7.40 ms | 75.5 ms | 54.2 ms | 91.4 ms | 17.57 M | 0 |
| LMS-SHA256-M24-H10-W8 | HW | keygen | 10 | 25.28 s ± 0.40 ms | 25.28 s | 25.28 s | 25.28 s | 6.07 G | 0 |
| LMS-SHA256-M24-H10-W8 | HW | sign | 256 | 33.6 ms ± 8.38 ms | 36.9 ms | 8.65 ms | 40.6 ms | 8.06 M | 0 |
| LMS-SHA256-M24-H10-W8 | HW | verify | 256 | 13.5 ms ± 1.42 ms | 13.1 ms | 10.3 ms | 16.8 ms | 3.23 M | 0 |
| LMS-SHAKE-M32-H10-W4 | SW | keygen | 10 | 2.32 min ± 0.03 ms | 2.32 min | 2.32 min | 2.32 min | 33.34 G | 0 |
| LMS-SHAKE-M32-H10-W4 | SW | sign | 256 | 189.0 ms ± 45.2 ms | 205.2 ms | 60.6 ms | 221.8 ms | 45.36 M | 0 |
| LMS-SHAKE-M32-H10-W4 | SW | verify | 256 | 67.2 ms ± 4.69 ms | 65.7 ms | 50.8 ms | 80.5 ms | 16.12 M | 0 |
| LMS-SHAKE-M24-H10-W4 | SW | keygen | 10 | 1.74 min ± 0.50 ms | 1.74 min | 1.74 min | 1.74 min | 25.01 G | 0 |
| LMS-SHAKE-M24-H10-W4 | SW | sign | 256 | 141.4 ms ± 34.1 ms | 153.6 ms | 46.9 ms | 169.1 ms | 33.94 M | 0 |
| LMS-SHAKE-M24-H10-W4 | SW | verify | 256 | 51.3 ms ± 3.81 ms | 51.1 ms | 36.5 ms | 60.3 ms | 12.31 M | 0 |
| LMS-SHA256-M32-H15-W4 | HW | keygen | 10 | 2.96 min ± 0.20 ms | 2.96 min | 2.96 min | 2.96 min | 42.67 G | 0 |
| LMS-SHA256-M32-H15-W4 | HW | sign | 256 | 2.66 ms ± 0.14 ms | 2.66 ms | 2.19 ms | 3.00 ms | 639 k | 0 |
| LMS-SHA256-M32-H15-W4 | HW | verify | 256 | 4.19 ms ± 0.15 ms | 4.18 ms | 3.86 ms | 4.66 ms | 1.01 M | 0 |
| HSS-SHA256-M32-H10+H10-W4 | SW | keygen | 10 | 48.91 s ± 1.04 ms | 48.91 s | 48.91 s | 48.91 s | 11.74 G | 0 |
| HSS-SHA256-M32-H10+H10-W4 | SW | sign | 256 | 33.4 ms ± 8.04 ms | 36.3 ms | 11.1 ms | 39.0 ms | 8.02 M | 0 |
| HSS-SHA256-M32-H10+H10-W4 | SW | verify | 256 | 23.9 ms ± 0.77 ms | 23.8 ms | 21.9 ms | 26.6 ms | 5.74 M | 0 |
| HSS-SHA256-M32-H10+H10-W4 | HW | keygen | 10 | 11.12 s ± 0.21 ms | 11.12 s | 11.12 s | 11.13 s | 2.67 G | 0 |
| HSS-SHA256-M32-H10+H10-W4 | HW | sign | 256 | 7.43 ms ± 1.86 ms | 8.02 ms | 2.31 ms | 10.8 ms | 1.78 M | 0 |
| HSS-SHA256-M32-H10+H10-W4 | HW | verify | 256 | 7.73 ms ± 0.16 ms | 7.71 ms | 7.32 ms | 8.12 ms | 1.85 M | 0 |
| HSS-SHA256-M24-H10+H10-W4 | SW | keygen | 10 | 36.57 s ± 0.36 ms | 36.57 s | 36.57 s | 36.57 s | 8.78 G | 0 |
| HSS-SHA256-M24-H10+H10-W4 | SW | sign | 256 | 25.0 ms ± 6.04 ms | 27.0 ms | 7.36 ms | 29.8 ms | 5.99 M | 0 |
| HSS-SHA256-M24-H10+H10-W4 | SW | verify | 256 | 19.4 ms ± 0.65 ms | 19.4 ms | 17.6 ms | 21.4 ms | 4.66 M | 0 |
| HSS-SHA256-M24-H10+H10-W4 | HW | keygen | 10 | 7.84 s ± 3.16 ms | 7.84 s | 7.83 s | 7.84 s | 1.88 G | 0 |
| HSS-SHA256-M24-H10+H10-W4 | HW | sign | 256 | 5.27 ms ± 1.32 ms | 5.67 ms | 1.70 ms | 7.65 ms | 1.27 M | 0 |
| HSS-SHA256-M24-H10+H10-W4 | HW | verify | 256 | 5.63 ms ± 0.11 ms | 5.63 ms | 5.32 ms | 5.88 ms | 1.35 M | 0 |

## XMSS / XMSS^MT (run_A2, radio on)

| Scheme | Impl | Operation | N | Mean ± SD | Median | Min | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| XMSS-SHA2_10_256 | SW | keygen | 10 | 1.66 min ± 0.45 ms | 1.66 min | 1.66 min | 1.66 min | 23.95 G | 0 |
| XMSS-SHA2_10_256 | SW | sign | 256 | 130.9 ms ± 32.3 ms | 142.6 ms | 40.3 ms | 150.4 ms | 31.43 M | 0 |
| XMSS-SHA2_10_256 | SW | verify | 256 | 53.5 ms ± 3.28 ms | 53.7 ms | 46.0 ms | 62.7 ms | 12.83 M | 0 |
| XMSS-SHA2_10_256 | HW | keygen | 10 | 23.93 s ± 0.50 ms | 23.93 s | 23.93 s | 23.93 s | 5.74 G | 0 |
| XMSS-SHA2_10_256 | HW | sign | 256 | 31.6 ms ± 7.75 ms | 34.3 ms | 9.23 ms | 38.6 ms | 7.58 M | 0 |
| XMSS-SHA2_10_256 | HW | verify | 256 | 13.1 ms ± 0.91 ms | 13.1 ms | 10.3 ms | 15.6 ms | 3.14 M | 0 |
| XMSS-SHA2_10_192 | SW | keygen | 10 | 1.58 min ± 1.71 ms | 1.58 min | 1.58 min | 1.58 min | 22.78 G | 0 |
| XMSS-SHA2_10_192 | SW | sign | 256 | 123.6 ms ± 30.8 ms | 134.9 ms | 36.1 ms | 145.7 ms | 29.66 M | 0 |
| XMSS-SHA2_10_192 | SW | verify | 256 | 52.3 ms ± 3.42 ms | 52.1 ms | 42.5 ms | 61.7 ms | 12.55 M | 0 |
| XMSS-SHA2_10_192 | HW | keygen | 10 | 16.07 s ± 0.06 ms | 16.07 s | 16.07 s | 16.07 s | 3.86 G | 0 |
| XMSS-SHA2_10_192 | HW | sign | 256 | 21.0 ms ± 5.29 ms | 22.9 ms | 6.28 ms | 27.8 ms | 5.04 M | 0 |
| XMSS-SHA2_10_192 | HW | verify | 256 | 9.02 ms ± 0.54 ms | 8.91 ms | 7.81 ms | 10.6 ms | 2.17 M | 0 |
| XMSS-SHAKE256_10_256 | SW | keygen | 10 | 8.90 min ± 0.21 ms | 8.90 min | 8.90 min | 8.90 min | 128.19 G | 0 |
| XMSS-SHAKE256_10_256 | SW | sign | 256 | 700.0 ms ± 171.3 ms | 762.8 ms | 214.0 ms | 804.4 ms | 167.99 M | 0 |
| XMSS-SHAKE256_10_256 | SW | verify | 256 | 283.9 ms ± 17.1 ms | 285.7 ms | 243.8 ms | 327.6 ms | 68.13 M | 0 |
| XMSS-SHAKE256_10_192 | SW | keygen | 10 | 6.13 min ± 0.23 ms | 6.13 min | 6.13 min | 6.13 min | 88.25 G | 0 |
| XMSS-SHAKE256_10_192 | SW | sign | 256 | 481.1 ms ± 120.8 ms | 524.5 ms | 134.3 ms | 562.9 ms | 115.47 M | 0 |
| XMSS-SHAKE256_10_192 | SW | verify | 256 | 198.2 ms ± 12.9 ms | 198.8 ms | 160.9 ms | 243.0 ms | 47.56 M | 0 |
| XMSSMT-SHA2_20/2_256 | SW | keygen | 10 | 3.33 min ± 3.17 ms | 3.33 min | 3.33 min | 3.33 min | 47.92 G | 0 |
| XMSSMT-SHA2_20/2_256 | SW | sign | 256 | 131.3 ms ± 32.5 ms | 142.7 ms | 41.6 ms | 153.0 ms | 31.51 M | 0 |
| XMSSMT-SHA2_20/2_256 | SW | verify | 256 | 105.5 ms ± 3.24 ms | 104.7 ms | 97.0 ms | 113.7 ms | 25.31 M | 0 |
| XMSSMT-SHA2_20/2_256 | HW | keygen | 10 | 47.87 s ± 0.90 ms | 47.87 s | 47.87 s | 47.88 s | 11.49 G | 0 |
| XMSSMT-SHA2_20/2_256 | HW | sign | 256 | 31.6 ms ± 7.85 ms | 34.6 ms | 8.59 ms | 38.7 ms | 7.59 M | 0 |
| XMSSMT-SHA2_20/2_256 | HW | verify | 256 | 25.2 ms ± 0.78 ms | 24.9 ms | 23.1 ms | 27.7 ms | 6.04 M | 0 |
| XMSSMT-SHA2_20/4_256 | SW | keygen | 10 | 12.61 s ± 6.78 ms | 12.61 s | 12.60 s | 12.62 s | 3.03 G | 0 |
| XMSSMT-SHA2_20/4_256 | SW | sign | 32 | 94.8 ms ± 49.2 ms | 95.1 ms | 40.3 ms | 147.8 ms | 22.76 M | 0 |
| XMSSMT-SHA2_20/4_256 | SW | verify | 32 | 207.4 ms ± 2.62 ms | 207.9 ms | 202.8 ms | 213.0 ms | 49.77 M | 0 |
| XMSSMT-SHA2_20/4_256 | HW | keygen | 10 | 3.03 s ± 1.41 ms | 3.03 s | 3.03 s | 3.03 s | 726.74 M | 0 |
| XMSSMT-SHA2_20/4_256 | HW | sign | 32 | 22.9 ms ± 12.1 ms | 23.3 ms | 9.24 ms | 35.5 ms | 5.49 M | 0 |
| XMSSMT-SHA2_20/4_256 | HW | verify | 32 | 51.8 ms ± 0.79 ms | 51.9 ms | 50.3 ms | 53.7 ms | 12.44 M | 0 |
| XMSSMT-SHA2_20/4_192 | SW | keygen | 10 | 12.00 s ± 5.10 ms | 12.00 s | 11.99 s | 12.01 s | 2.88 G | 0 |
| XMSSMT-SHA2_20/4_192 | SW | sign | 32 | 90.0 ms ± 48.1 ms | 90.7 ms | 36.1 ms | 143.6 ms | 21.60 M | 0 |
| XMSSMT-SHA2_20/4_192 | SW | verify | 32 | 205.4 ms ± 3.61 ms | 205.6 ms | 198.4 ms | 212.9 ms | 49.31 M | 0 |
| XMSSMT-SHA2_20/4_192 | HW | keygen | 10 | 2.03 s ± 0.62 ms | 2.03 s | 2.03 s | 2.03 s | 487.78 M | 0 |
| XMSSMT-SHA2_20/4_192 | HW | sign | 32 | 15.2 ms ± 7.93 ms | 14.9 ms | 6.30 ms | 24.2 ms | 3.64 M | 0 |
| XMSSMT-SHA2_20/4_192 | HW | verify | 32 | 34.7 ms ± 0.55 ms | 34.6 ms | 33.8 ms | 36.0 ms | 8.33 M | 0 |
| XMSSMT-SHA2_40/8_256 | SW | keygen | 10 | 25.28 s ± 9.36 ms | 25.28 s | 25.26 s | 25.29 s | 6.07 G | 0 |
| XMSSMT-SHA2_40/8_256 | SW | sign | 32 | 95.1 ms ± 51.2 ms | 93.2 ms | 37.8 ms | 151.4 ms | 22.83 M | 0 |
| XMSSMT-SHA2_40/8_256 | SW | verify | 32 | 431.8 ms ± 3.83 ms | 431.1 ms | 424.7 ms | 440.1 ms | 103.63 M | 0 |
| XMSSMT-SHA2_40/8_256 | HW | keygen | 10 | 6.07 s ± 1.58 ms | 6.07 s | 6.06 s | 6.07 s | 1.46 G | 0 |
| XMSSMT-SHA2_40/8_256 | HW | sign | 32 | 22.8 ms ± 12.0 ms | 23.2 ms | 9.58 ms | 36.4 ms | 5.47 M | 0 |
| XMSSMT-SHA2_40/8_256 | HW | verify | 32 | 100.0 ms ± 0.77 ms | 99.9 ms | 97.7 ms | 101.4 ms | 24.00 M | 0 |
| XMSSMT-SHA2_60/12_256 | SW | keygen | 10 | 37.94 s ± 10.8 ms | 37.94 s | 37.92 s | 37.95 s | 9.11 G | 0 |
| XMSSMT-SHA2_60/12_256 | SW | sign | 32 | 95.2 ms ± 48.9 ms | 92.7 ms | 40.4 ms | 151.9 ms | 22.84 M | 0 |
| XMSSMT-SHA2_60/12_256 | SW | verify | 32 | 631.8 ms ± 3.67 ms | 631.3 ms | 623.6 ms | 641.5 ms | 151.64 M | 0 |
| XMSSMT-SHA2_60/12_256 | HW | keygen | 10 | 9.11 s ± 2.51 ms | 9.11 s | 9.10 s | 9.11 s | 2.19 G | 0 |
| XMSSMT-SHA2_60/12_256 | HW | sign | 32 | 22.9 ms ± 12.0 ms | 22.6 ms | 9.92 ms | 36.6 ms | 5.51 M | 0 |
| XMSSMT-SHA2_60/12_256 | HW | verify | 32 | 152.6 ms ± 0.81 ms | 152.6 ms | 151.0 ms | 154.1 ms | 36.63 M | 0 |
| XMSSMT-SHAKE256_20/4_256 | SW | keygen | 10 | 1.12 min ± 13.9 ms | 1.12 min | 1.12 min | 1.13 min | 16.20 G | 0 |
| XMSSMT-SHAKE256_20/4_256 | SW | sign | 32 | 507.1 ms ± 263.9 ms | 505.8 ms | 193.1 ms | 801.8 ms | 121.71 M | 0 |
| XMSSMT-SHAKE256_20/4_256 | SW | verify | 32 | 1.11 s ± 16.7 ms | 1.11 s | 1.08 s | 1.16 s | 266.62 M | 0 |
| XMSSMT-SHAKE256_20/4_192 | SW | keygen | 10 | 46.46 s ± 18.6 ms | 46.46 s | 46.43 s | 46.48 s | 11.15 G | 0 |
| XMSSMT-SHAKE256_20/4_192 | SW | sign | 32 | 350.8 ms ± 179.8 ms | 358.1 ms | 147.0 ms | 543.8 ms | 84.19 M | 0 |
| XMSSMT-SHAKE256_20/4_192 | SW | verify | 32 | 797.3 ms ± 15.5 ms | 799.0 ms | 751.7 ms | 827.5 ms | 191.34 M | 0 |

## ML-KEM (run_B1, radio on)

| Scheme | Impl | Operation | N | Mean ± SD | Median | Min | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| ML-KEM-512 | stack | keygen | 1000 | 3.70 ms ± 0.02 ms | 3.69 ms | 3.69 ms | 3.98 ms | 887 k | 0 |
| ML-KEM-512 | stack | encaps | 1000 | 4.42 ms ± 0.02 ms | 4.42 ms | 4.41 ms | 4.53 ms | 1.06 M | 0 |
| ML-KEM-512 | stack | decaps | 1000 | 5.67 ms ± 0.02 ms | 5.67 ms | 5.66 ms | 5.79 ms | 1.36 M | 0 |
| ML-KEM-768 | stack | keygen | 1000 | 6.05 ms ± 0.03 ms | 6.04 ms | 6.03 ms | 6.25 ms | 1.45 M | 0 |
| ML-KEM-768 | stack | encaps | 1000 | 7.18 ms ± 0.03 ms | 7.17 ms | 7.16 ms | 7.38 ms | 1.72 M | 0 |
| ML-KEM-768 | stack | decaps | 1000 | 8.89 ms ± 0.03 ms | 8.88 ms | 8.87 ms | 9.09 ms | 2.13 M | 0 |
| ML-KEM-1024 | stack | keygen | 1000 | 9.41 ms ± 0.04 ms | 9.39 ms | 9.38 ms | 9.61 ms | 2.26 M | 0 |
| ML-KEM-1024 | stack | encaps | 1000 | 10.8 ms ± 0.04 ms | 10.7 ms | 10.7 ms | 11.0 ms | 2.58 M | 0 |
| ML-KEM-1024 | stack | decaps | 1000 | 12.9 ms ± 0.04 ms | 12.9 ms | 12.9 ms | 13.1 ms | 3.11 M | 0 |

## ML-DSA (run_B1, radio on)

| Scheme | Impl | Operation | N | Mean ± SD | Median | Min | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| ML-DSA-44 | fast-sign | keygen | 1000 | 13.1 ms ± 0.13 ms | 13.1 ms | 12.7 ms | 13.5 ms | 3.14 M | 0 |
| ML-DSA-44 | fast-sign | sign | 1000 | 42.5 ms ± 27.0 ms | 33.5 ms | 19.6 ms | 247.2 ms | 10.19 M | 0 |
| ML-DSA-44 | fast-sign | verify | 1000 | 13.8 ms ± 0.02 ms | 13.8 ms | 13.8 ms | 13.8 ms | 3.31 M | 0 |
| ML-DSA-65 | fast-sign | keygen | 1000 | 22.6 ms ± 0.01 ms | 22.6 ms | 22.6 ms | 22.8 ms | 5.43 M | 0 |
| ML-DSA-65 | fast-sign | sign | 1000 | 66.9 ms ± 41.7 ms | 52.8 ms | 30.3 ms | 378.7 ms | 16.05 M | 0 |
| ML-DSA-65 | fast-sign | verify | 1000 | 22.8 ms ± 0.01 ms | 22.7 ms | 22.7 ms | 22.8 ms | 5.46 M | 0 |
| ML-DSA-87 | fast-sign | keygen | 1000 | 37.8 ms ± 0.17 ms | 37.8 ms | 37.4 ms | 38.4 ms | 9.07 M | 0 |
| ML-DSA-87 | fast-sign | sign | 1000 | 33.1 ms ± 0.01 ms | 33.1 ms | 33.1 ms | 33.2 ms | 7.94 M | 1000 |
| ML-DSA-87 | fast-sign | verify | 1000 | 0.00 ms ± 0.00 ms | 0.00 ms | 0.00 ms | 0.00 ms | 0 k | 1000 |

## ML-DSA (run_B3, radio off)

| Scheme | Impl | Operation | N | Mean ± SD | Median | Min | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| ML-DSA-44 | fast-sign | keygen | 1000 | 12.8 ms ± 0.13 ms | 12.8 ms | 12.4 ms | 13.3 ms | 3.08 M | 0 |
| ML-DSA-44 | fast-sign | sign | 1000 | 40.8 ms ± 25.6 ms | 32.7 ms | 19.1 ms | 272.2 ms | 9.78 M | 0 |
| ML-DSA-44 | fast-sign | verify | 1000 | 13.5 ms ± 0.02 ms | 13.5 ms | 13.5 ms | 13.6 ms | 3.25 M | 0 |
| ML-DSA-65 | fast-sign | keygen | 1000 | 22.4 ms ± 0.01 ms | 22.4 ms | 22.4 ms | 22.6 ms | 5.37 M | 0 |
| ML-DSA-65 | fast-sign | sign | 1000 | 65.0 ms ± 41.7 ms | 52.9 ms | 29.9 ms | 412.8 ms | 15.60 M | 0 |
| ML-DSA-65 | fast-sign | verify | 1000 | 22.5 ms ± 0.01 ms | 22.5 ms | 22.5 ms | 22.6 ms | 5.41 M | 0 |
| ML-DSA-87 | fast-sign | keygen | 1000 | 37.6 ms ± 0.18 ms | 37.6 ms | 36.9 ms | 38.1 ms | 9.02 M | 0 |
| ML-DSA-87 | fast-sign | sign | 1000 | 85.3 ms ± 41.0 ms | 72.5 ms | 48.4 ms | 343.2 ms | 20.46 M | 0 |
| ML-DSA-87 | fast-sign | verify | 1000 | 38.2 ms ± 0.01 ms | 38.2 ms | 38.2 ms | 38.3 ms | 9.18 M | 0 |

## Radio interference control (same build, radio off vs on)

| Scheme | Impl | Operation | Radio on | Radio off | Difference |
|---|---|---|--:|--:|--:|
| ML-KEM-512 | heap | keygen | 4.05 ms ± 0.02 ms | 3.86 ms ± 0.02 ms | +5.11% |
| ML-KEM-512 | heap | encaps | 5.13 ms ± 0.02 ms | 4.79 ms ± 0.02 ms | +7.04% |
| ML-KEM-512 | heap | decaps | 6.62 ms ± 0.02 ms | 6.14 ms ± 0.02 ms | +7.78% |
| ML-KEM-768 | heap | keygen | 6.41 ms ± 0.03 ms | 6.22 ms ± 0.03 ms | +2.91% |
| ML-KEM-768 | heap | encaps | 7.86 ms ± 0.03 ms | 7.55 ms ± 0.03 ms | +4.12% |
| ML-KEM-768 | heap | decaps | 9.85 ms ± 0.03 ms | 9.35 ms ± 0.03 ms | +5.35% |
| ML-KEM-1024 | heap | keygen | 9.81 ms ± 0.04 ms | 9.61 ms ± 0.04 ms | +1.98% |
| ML-KEM-1024 | heap | encaps | 11.5 ms ± 0.04 ms | 11.2 ms ± 0.04 ms | +3.31% |
| ML-KEM-1024 | heap | decaps | 14.0 ms ± 0.04 ms | 13.6 ms ± 0.04 ms | +2.95% |
| ML-DSA-44 | reduce-ram | keygen | 13.1 ms ± 0.13 ms | 13.0 ms ± 0.13 ms | +1.27% |
| ML-DSA-44 | reduce-ram | sign | 77.0 ms ± 60.9 ms | 76.7 ms ± 65.1 ms | +0.49% |
| ML-DSA-44 | reduce-ram | verify | 13.9 ms ± 0.02 ms | 13.7 ms ± 0.02 ms | +1.11% |
| ML-DSA-65 | reduce-ram | keygen | 22.7 ms ± 0.02 ms | 22.6 ms ± 0.02 ms | +0.81% |
| ML-DSA-65 | reduce-ram | sign | 141.8 ms ± 119.8 ms | 131.4 ms ± 114.2 ms | +7.93% |
| ML-DSA-65 | reduce-ram | verify | 22.9 ms ± 0.01 ms | 22.7 ms ± 0.01 ms | +0.86% |
| ML-DSA-87 | reduce-ram | keygen | 38.1 ms ± 0.18 ms | 38.0 ms ± 0.17 ms | +0.45% |
| ML-DSA-87 | reduce-ram | sign | 180.6 ms ± 145.6 ms | 185.8 ms ± 154.4 ms | -2.81% |
| ML-DSA-87 | reduce-ram | verify | 38.8 ms ± 0.01 ms | 38.6 ms ± 0.01 ms | +0.57% |
| ML-KEM-512 | stack | keygen | 3.70 ms ± 0.02 ms | 3.70 ms ± 0.02 ms | +0.00% |
| ML-KEM-512 | stack | encaps | 4.42 ms ± 0.02 ms | 4.42 ms ± 0.02 ms | -0.00% |
| ML-KEM-512 | stack | decaps | 5.67 ms ± 0.02 ms | 5.67 ms ± 0.02 ms | +0.00% |
| ML-KEM-768 | stack | keygen | 6.05 ms ± 0.03 ms | 6.05 ms ± 0.03 ms | +0.00% |
| ML-KEM-768 | stack | encaps | 7.18 ms ± 0.03 ms | 7.18 ms ± 0.03 ms | +0.00% |
| ML-KEM-768 | stack | decaps | 8.89 ms ± 0.03 ms | 8.89 ms ± 0.03 ms | -0.00% |
| ML-KEM-1024 | stack | keygen | 9.41 ms ± 0.04 ms | 9.41 ms ± 0.04 ms | +0.00% |
| ML-KEM-1024 | stack | encaps | 10.8 ms ± 0.04 ms | 10.8 ms ± 0.04 ms | +0.00% |
| ML-KEM-1024 | stack | decaps | 12.9 ms ± 0.04 ms | 12.9 ms ± 0.04 ms | +0.00% |
| ML-DSA-44 | fast-sign | keygen | 13.1 ms ± 0.13 ms | 12.9 ms ± 0.13 ms | +1.09% |
| ML-DSA-44 | fast-sign | sign | 42.5 ms ± 27.0 ms | 40.9 ms ± 24.2 ms | +3.80% |
| ML-DSA-44 | fast-sign | verify | 13.8 ms ± 0.02 ms | 13.7 ms ± 0.02 ms | +0.95% |
| ML-DSA-65 | fast-sign | keygen | 22.6 ms ± 0.01 ms | 22.5 ms ± 0.01 ms | +0.63% |
| ML-DSA-65 | fast-sign | sign | 66.9 ms ± 41.7 ms | 64.2 ms ± 38.7 ms | +4.11% |
| ML-DSA-65 | fast-sign | verify | 22.8 ms ± 0.01 ms | 22.6 ms ± 0.01 ms | +0.70% |
| ML-DSA-87 | fast-sign | keygen | 37.8 ms ± 0.17 ms | 37.7 ms ± 0.18 ms | +0.33% |

## SHA hardware speed-up (software mean / hardware mean)

| Scheme | Operation | Software | Hardware | Speed-up |
|---|---|--:|--:|--:|
| SLH-DSA-SHA2-128f | keygen | 111.1 ms ± 0.39 ms | 45.4 ms ± 0.36 ms | 2.45x |
| SLH-DSA-SHA2-128f | sign | 2.61 s ± 1.20 ms | 1.06 s ± 0.70 ms | 2.46x |
| SLH-DSA-SHA2-128f | verify | 153.2 ms ± 0.48 ms | 62.1 ms ± 0.36 ms | 2.47x |
| SLH-DSA-SHA2-192f | keygen | 166.5 ms ± 0.48 ms | 72.6 ms ± 0.35 ms | 2.29x |
| SLH-DSA-SHA2-192f | sign | 4.87 s ± 0.66 ms | 1.98 s ± 0.34 ms | 2.46x |
| SLH-DSA-SHA2-192f | verify | 263.3 ms ± 0.50 ms | 103.8 ms ± 0.53 ms | 2.54x |
| SLH-DSA-SHA2-256f | keygen | 442.8 ms ± 0.42 ms | 212.6 ms ± 0.56 ms | 2.08x |
| SLH-DSA-SHA2-256f | sign | 10.12 s ± 1.11 ms | 4.48 s ± 0.38 ms | 2.26x |
| SLH-DSA-SHA2-256f | verify | 278.3 ms ± 0.57 ms | 120.2 ms ± 0.47 ms | 2.32x |
| SLH-DSA-SHA2-128s | keygen | 7.11 s ± 0.53 ms | 2.90 s ± 0.09 ms | 2.45x |
| SLH-DSA-SHA2-128s | sign | 54.43 s ± 0.85 ms | 22.09 s ± 0.45 ms | 2.46x |
| SLH-DSA-SHA2-128s | verify | 50.6 ms ± 0.35 ms | 23.2 ms ± 0.02 ms | 2.18x |
| SLH-DSA-SHA2-192s | keygen | 10.64 s ± 0.22 ms | 4.65 s ± 0.59 ms | 2.29x |
| SLH-DSA-SHA2-192s | sign | 1.92 min ± 7.63 ms | 45.08 s ± 0.60 ms | 2.56x |
| SLH-DSA-SHA2-192s | verify | 101.2 ms ± 0.44 ms | 37.7 ms ± 0.34 ms | 2.69x |
| SLH-DSA-SHA2-256s | keygen | 7.08 s ± 0.77 ms | 3.40 s ± 0.52 ms | 2.08x |
| SLH-DSA-SHA2-256s | sign | 1.82 min ± 8.02 ms | 44.63 s ± 0.70 ms | 2.44x |
| SLH-DSA-SHA2-256s | verify | 145.3 ms ± 0.53 ms | 62.8 ms ± 0.37 ms | 2.31x |
| LMS-SHA256-M32-H5-W1 | keygen | 513.0 ms ± 0.02 ms | 221.5 ms ± 0.45 ms | 2.32x |
| LMS-SHA256-M32-H5-W1 | sign | 18.1 ms ± 8.23 ms | 6.64 ms ± 3.52 ms | 2.72x |
| LMS-SHA256-M32-H5-W1 | verify | 8.02 ms ± 0.19 ms | 5.79 ms ± 0.04 ms | 1.39x |
| LMS-SHA256-M32-H5-W2 | keygen | 439.1 ms ± 0.39 ms | 146.3 ms ± 0.35 ms | 3.00x |
| LMS-SHA256-M32-H5-W2 | sign | 14.7 ms ± 6.95 ms | 4.47 ms ± 2.34 ms | 3.29x |
| LMS-SHA256-M32-H5-W2 | verify | 7.09 ms ± 0.28 ms | 3.62 ms ± 0.05 ms | 1.96x |
| LMS-SHA256-M32-H5-W4 | keygen | 764.5 ms ± 0.37 ms | 174.2 ms ± 0.46 ms | 4.39x |
| LMS-SHA256-M32-H5-W4 | sign | 24.2 ms ± 12.0 ms | 5.40 ms ± 2.69 ms | 4.49x |
| LMS-SHA256-M32-H5-W4 | verify | 12.4 ms ± 0.70 ms | 3.56 ms ± 0.19 ms | 3.49x |
| LMS-SHA256-M32-H5-W8 | keygen | 5.88 s ± 0.32 ms | 1.08 s ± 0.22 ms | 5.44x |
| LMS-SHA256-M32-H5-W8 | sign | 183.4 ms ± 92.4 ms | 33.4 ms ± 17.1 ms | 5.50x |
| LMS-SHA256-M32-H5-W8 | verify | 92.7 ms ± 7.54 ms | 17.8 ms ± 1.60 ms | 5.20x |
| LMS-SHA256-M32-H10-W2 | keygen | 14.04 s ± 0.78 ms | 4.67 s ± 0.29 ms | 3.01x |
| LMS-SHA256-M32-H10-W2 | sign | 19.9 ms ± 4.57 ms | 6.15 ms ± 1.54 ms | 3.23x |
| LMS-SHA256-M32-H10-W2 | verify | 7.38 ms ± 0.29 ms | 3.92 ms ± 0.05 ms | 1.88x |
| LMS-SHA256-M32-H10-W4 | keygen | 24.45 s ± 0.21 ms | 5.56 s ± 0.27 ms | 4.40x |
| LMS-SHA256-M32-H10-W4 | sign | 33.3 ms ± 7.87 ms | 7.42 ms ± 1.84 ms | 4.49x |
| LMS-SHA256-M32-H10-W4 | verify | 12.6 ms ± 0.85 ms | 3.87 ms ± 0.16 ms | 3.25x |
| LMS-SHA256-M32-H10-W8 | keygen | 3.14 min ± 0.27 ms | 34.61 s ± 0.38 ms | 5.44x |
| LMS-SHA256-M32-H10-W8 | sign | 251.5 ms ± 61.8 ms | 45.9 ms ± 11.3 ms | 5.48x |
| LMS-SHA256-M32-H10-W8 | verify | 93.8 ms ± 8.89 ms | 18.3 ms ± 1.56 ms | 5.12x |
| LMS-SHA256-M24-H10-W4 | keygen | 18.28 s ± 0.19 ms | 3.92 s ± 1.82 ms | 4.67x |
| LMS-SHA256-M24-H10-W4 | sign | 24.9 ms ± 5.96 ms | 5.24 ms ± 1.32 ms | 4.74x |
| LMS-SHA256-M24-H10-W4 | verify | 9.60 ms ± 0.63 ms | 2.88 ms ± 0.12 ms | 3.33x |
| LMS-SHA256-M24-H10-W8 | keygen | 2.38 min ± 0.27 ms | 25.28 s ± 0.40 ms | 5.64x |
| LMS-SHA256-M24-H10-W8 | sign | 188.9 ms ± 46.4 ms | 33.6 ms ± 8.38 ms | 5.62x |
| LMS-SHA256-M24-H10-W8 | verify | 73.2 ms ± 7.40 ms | 13.5 ms ± 1.42 ms | 5.44x |
| HSS-SHA256-M32-H10+H10-W4 | keygen | 48.91 s ± 1.04 ms | 11.12 s ± 0.21 ms | 4.40x |
| HSS-SHA256-M32-H10+H10-W4 | sign | 33.4 ms ± 8.04 ms | 7.43 ms ± 1.86 ms | 4.50x |
| HSS-SHA256-M32-H10+H10-W4 | verify | 23.9 ms ± 0.77 ms | 7.73 ms ± 0.16 ms | 3.10x |
| HSS-SHA256-M24-H10+H10-W4 | keygen | 36.57 s ± 0.36 ms | 7.84 s ± 3.16 ms | 4.67x |
| HSS-SHA256-M24-H10+H10-W4 | sign | 25.0 ms ± 6.04 ms | 5.27 ms ± 1.32 ms | 4.73x |
| HSS-SHA256-M24-H10+H10-W4 | verify | 19.4 ms ± 0.65 ms | 5.63 ms ± 0.11 ms | 3.45x |
| XMSS-SHA2_10_256 | keygen | 1.66 min ± 0.45 ms | 23.93 s ± 0.50 ms | 4.17x |
| XMSS-SHA2_10_256 | sign | 130.9 ms ± 32.3 ms | 31.6 ms ± 7.75 ms | 4.15x |
| XMSS-SHA2_10_256 | verify | 53.5 ms ± 3.28 ms | 13.1 ms ± 0.91 ms | 4.09x |
| XMSS-SHA2_10_192 | keygen | 1.58 min ± 1.71 ms | 16.07 s ± 0.06 ms | 5.91x |
| XMSS-SHA2_10_192 | sign | 123.6 ms ± 30.8 ms | 21.0 ms ± 5.29 ms | 5.88x |
| XMSS-SHA2_10_192 | verify | 52.3 ms ± 3.42 ms | 9.02 ms ± 0.54 ms | 5.80x |
| XMSSMT-SHA2_20/2_256 | keygen | 3.33 min ± 3.17 ms | 47.87 s ± 0.90 ms | 4.17x |
| XMSSMT-SHA2_20/2_256 | sign | 131.3 ms ± 32.5 ms | 31.6 ms ± 7.85 ms | 4.15x |
| XMSSMT-SHA2_20/2_256 | verify | 105.5 ms ± 3.24 ms | 25.2 ms ± 0.78 ms | 4.19x |
| XMSSMT-SHA2_20/4_256 | keygen | 12.61 s ± 6.78 ms | 3.03 s ± 1.41 ms | 4.16x |
| XMSSMT-SHA2_20/4_256 | sign | 94.8 ms ± 49.2 ms | 22.9 ms ± 12.1 ms | 4.15x |
| XMSSMT-SHA2_20/4_256 | verify | 207.4 ms ± 2.62 ms | 51.8 ms ± 0.79 ms | 4.00x |
| XMSSMT-SHA2_20/4_192 | keygen | 12.00 s ± 5.10 ms | 2.03 s ± 0.62 ms | 5.91x |
| XMSSMT-SHA2_20/4_192 | sign | 90.0 ms ± 48.1 ms | 15.2 ms ± 7.93 ms | 5.93x |
| XMSSMT-SHA2_20/4_192 | verify | 205.4 ms ± 3.61 ms | 34.7 ms ± 0.55 ms | 5.92x |
| XMSSMT-SHA2_40/8_256 | keygen | 25.28 s ± 9.36 ms | 6.07 s ± 1.58 ms | 4.17x |
| XMSSMT-SHA2_40/8_256 | sign | 95.1 ms ± 51.2 ms | 22.8 ms ± 12.0 ms | 4.18x |
| XMSSMT-SHA2_40/8_256 | verify | 431.8 ms ± 3.83 ms | 100.0 ms ± 0.77 ms | 4.32x |
| XMSSMT-SHA2_60/12_256 | keygen | 37.94 s ± 10.8 ms | 9.11 s ± 2.51 ms | 4.17x |
| XMSSMT-SHA2_60/12_256 | sign | 95.2 ms ± 48.9 ms | 22.9 ms ± 12.0 ms | 4.15x |
| XMSSMT-SHA2_60/12_256 | verify | 631.8 ms ± 3.67 ms | 152.6 ms ± 0.81 ms | 4.14x |

## Memory (bytes; stack and heap net of the empty measurement task; mean ± SD and max over the runs with return code 0)

| Run | Scheme | Impl | Operation | Runs | Stack | Stack max | Heap peak | Heap max | of which internal (max) | Persistent | of which internal | Failed |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|--:|
| run_A3 | baseline | - | empty-task | 1 | 444 ± 0 | 444 | 64 ± 0 | 64 | 64 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-KEM-512 | heap | keygen | 10 | 1816 ± 64 | 1836 | 5952 ± 0 | 5952 | 5952 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-KEM-512 | heap | encaps | 10 | 1868 ± 0 | 1868 | 8592 ± 0 | 8592 | 8592 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-KEM-512 | heap | decaps | 10 | 1632 ± 0 | 1632 | 9360 ± 0 | 9360 | 9360 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-KEM-768 | heap | keygen | 10 | 1588 ± 27 | 1664 | 10304 ± 0 | 10304 | 10304 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-KEM-768 | heap | encaps | 10 | 1598 ± 5 | 1612 | 13520 ± 0 | 13520 | 13520 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-KEM-768 | heap | decaps | 10 | 1580 ± 0 | 1580 | 14544 ± 0 | 14544 | 14544 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-KEM-1024 | heap | keygen | 10 | 1623 ± 39 | 1680 | 15680 ± 0 | 15680 | 15680 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-KEM-1024 | heap | encaps | 10 | 1598 ± 5 | 1612 | 19344 ± 0 | 19344 | 19344 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-KEM-1024 | heap | decaps | 10 | 1566 ± 5 | 1580 | 20912 ± 0 | 20912 | 20912 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-DSA-44 | reduce-ram | keygen | 10 | 1700 ± 27 | 1776 | 11728 ± 0 | 11728 | 11728 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-DSA-44 | reduce-ram | sign | 10 | 1822 ± 43 | 1856 | 10016 ± 0 | 10016 | 10016 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-DSA-44 | reduce-ram | verify | 10 | 2212 ± 27 | 2288 | 9264 ± 0 | 9264 | 9264 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-DSA-65 | reduce-ram | keygen | 10 | 1709 ± 35 | 1776 | 14800 ± 0 | 14800 | 14800 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-DSA-65 | reduce-ram | sign | 10 | 1848 ± 27 | 1856 | 12096 ± 0 | 12096 | 12096 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-DSA-65 | reduce-ram | verify | 10 | 2212 ± 27 | 2288 | 10352 ± 0 | 10352 | 10352 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-DSA-87 | reduce-ram | keygen | 10 | 1709 ± 35 | 1776 | 18896 ± 0 | 18896 | 18896 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-DSA-87 | reduce-ram | sign | 10 | 1816 ± 43 | 1856 | 14400 ± 0 | 14400 | 14400 | 0 ± 0 | 0 | 0 |
| run_A3 | ML-DSA-87 | reduce-ram | verify | 10 | 2238 ± 43 | 2288 | 12656 ± 0 | 12656 | 12656 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-128f | SW | keygen | 10 | 4077 ± 22 | 4128 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-128f | SW | sign | 10 | 4288 ± 0 | 4288 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-128f | SW | verify | 10 | 4186 ± 16 | 4208 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-128f | HW | keygen | 10 | 4036 ± 72 | 4156 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-128f | HW | sign | 10 | 4305 ± 31 | 4368 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-128f | HW | verify | 10 | 4162 ± 53 | 4268 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-192f | SW | keygen | 10 | 4579 ± 34 | 4640 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-192f | SW | sign | 10 | 4824 ± 25 | 4864 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-192f | SW | verify | 10 | 4772 ± 10 | 4784 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-192f | HW | keygen | 10 | 4025 ± 29 | 4092 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-192f | HW | sign | 10 | 4302 ± 32 | 4368 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-192f | HW | verify | 10 | 4193 ± 49 | 4268 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-256f | SW | keygen | 10 | 4627 ± 17 | 4640 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-256f | SW | sign | 10 | 4834 ± 31 | 4896 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-256f | SW | verify | 10 | 4756 ± 30 | 4784 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-256f | HW | keygen | 10 | 4052 ± 52 | 4156 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-256f | HW | sign | 10 | 4316 ± 0 | 4316 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-256f | HW | verify | 10 | 4238 ± 46 | 4320 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-128f | SW | keygen | 10 | 3950 ± 23 | 3984 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-128f | SW | sign | 10 | 4134 ± 20 | 4144 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-128f | SW | verify | 10 | 4054 ± 14 | 4080 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-192f | SW | keygen | 10 | 3965 ± 25 | 3984 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-192f | SW | sign | 10 | 4144 ± 0 | 4144 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-192f | SW | verify | 10 | 4070 ± 16 | 4080 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-256f | SW | keygen | 10 | 3970 ± 23 | 3984 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-256f | SW | sign | 10 | 4144 ± 0 | 4144 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-256f | SW | verify | 10 | 4061 ± 16 | 4080 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-128s | SW | keygen | 10 | 4128 ± 0 | 4128 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-128s | SW | sign | 10 | 4288 ± 0 | 4288 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-128s | SW | verify | 10 | 4173 ± 15 | 4208 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-192s | SW | keygen | 10 | 4666 ± 33 | 4704 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-192s | SW | sign | 10 | 4856 ± 17 | 4864 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-192s | SW | verify | 10 | 4726 ± 28 | 4764 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-256s | SW | keygen | 10 | 4674 ± 23 | 4704 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-256s | SW | sign | 10 | 4867 ± 10 | 4896 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHA2-256s | SW | verify | 10 | 4772 ± 18 | 4796 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-128s | SW | keygen | 10 | 3984 ± 0 | 3984 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-128s | SW | sign | 10 | 4144 ± 0 | 4144 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-128s | SW | verify | 10 | 4048 ± 0 | 4048 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-192s | SW | keygen | 10 | 3984 ± 0 | 3984 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-192s | SW | sign | 10 | 4144 ± 0 | 4144 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-192s | SW | verify | 10 | 4048 ± 0 | 4048 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-256s | SW | keygen | 10 | 3984 ± 0 | 3984 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-256s | SW | sign | 10 | 4144 ± 0 | 4144 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | SLH-DSA-SHAKE-256s | SW | verify | 10 | 4058 ± 16 | 4080 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | LMS-SHA256-M32-H5-W4 | HW | keygen | 10 | 1498 ± 45 | 1532 | 3324 ± 0 | 3324 | 3324 | 0 ± 0 | 0 | 0 |
| run_A3 | LMS-SHA256-M32-H5-W4 | HW | key-in-ram | 10 | 0 ± 0 | 0 | 0 ± 0 | 0 | 0 | 3040 ± 0 | 3040 | 0 |
| run_A3 | LMS-SHA256-M32-H5-W4 | HW | sign | 10 | 960 ± 0 | 960 | 16 ± 0 | 16 | 16 | 0 ± 0 | 0 | 0 |
| run_A3 | LMS-SHA256-M32-H5-W4 | HW | verify | 10 | 1414 ± 12 | 1436 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | LMS-SHA256-M32-H10-W4 | HW | keygen | 10 | 1568 ± 25 | 1584 | 11736 ± 0 | 11736 | 11736 | 0 ± 0 | 0 | 0 |
| run_A3 | LMS-SHA256-M32-H10-W4 | HW | key-in-ram | 10 | 0 ± 0 | 0 | 0 ± 0 | 0 | 0 | 9472 ± 0 | 9472 | 0 |
| run_A3 | LMS-SHA256-M32-H10-W4 | HW | sign | 10 | 960 ± 0 | 960 | 28 ± 0 | 28 | 28 | 0 ± 0 | 0 | 0 |
| run_A3 | LMS-SHA256-M32-H10-W4 | HW | verify | 10 | 1411 ± 9 | 1436 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | LMS-SHA256-M32-H15-W4 | HW | keygen | 10 | 1584 ± 0 | 1584 | 52376 ± 0 | 52376 | 1668 | 0 ± 0 | 0 | 0 |
| run_A3 | LMS-SHA256-M32-H15-W4 | HW | key-in-ram | 10 | 0 ± 0 | 0 | 0 ± 0 | 0 | 0 | 43840 ± 0 | 1584 | 0 |
| run_A3 | LMS-SHA256-M32-H15-W4 | HW | sign | 10 | 960 ± 0 | 960 | 28 ± 0 | 28 | 28 | 0 ± 0 | 0 | 0 |
| run_A3 | LMS-SHA256-M32-H15-W4 | HW | verify | 10 | 1408 ± 0 | 1408 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | HSS-SHA256-M32-H10+H10-W4 | HW | keygen | 10 | 1777 ± 27 | 1808 | 26340 ± 0 | 26340 | 26340 | 0 ± 0 | 0 | 0 |
| run_A3 | HSS-SHA256-M32-H10+H10-W4 | HW | key-in-ram | 10 | 0 ± 0 | 0 | 0 ± 0 | 0 | 0 | 21276 ± 0 | 21276 | 0 |
| run_A3 | HSS-SHA256-M32-H10+H10-W4 | HW | sign | 10 | 960 ± 0 | 960 | 24 ± 0 | 24 | 24 | 0 ± 0 | 0 | 0 |
| run_A3 | HSS-SHA256-M32-H10+H10-W4 | HW | verify | 10 | 1472 ± 0 | 1472 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSS-SHA2_10_256 | HW | keygen | 10 | 3485 ± 20 | 3500 | 13496 ± 0 | 13496 | 13496 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSS-SHA2_10_256 | HW | key-in-ram | 10 | 0 ± 0 | 0 | 0 ± 0 | 0 | 0 | 11372 ± 0 | 11372 | 0 |
| run_A3 | XMSS-SHA2_10_256 | HW | sign | 10 | 1161 ± 39 | 1244 | 32 ± 0 | 32 | 32 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSS-SHA2_10_256 | HW | verify | 10 | 3540 ± 44 | 3616 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSSMT-SHA2_20/2_256 | HW | keygen | 10 | 3601 ± 18 | 3612 | 27856 ± 0 | 27856 | 27856 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSSMT-SHA2_20/2_256 | HW | key-in-ram | 10 | 0 ± 0 | 0 | 0 ± 0 | 0 | 0 | 23040 ± 0 | 23040 | 0 |
| run_A3 | XMSSMT-SHA2_20/2_256 | HW | sign | 10 | 1176 ± 52 | 1308 | 32 ± 0 | 32 | 32 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSSMT-SHA2_20/2_256 | HW | verify | 10 | 3549 ± 14 | 3564 | 8 ± 0 | 8 | 8 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSSMT-SHA2_20/4_256 | HW | keygen | 10 | 3546 ± 34 | 3612 | 26060 ± 0 | 26060 | 26060 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSSMT-SHA2_20/4_256 | HW | key-in-ram | 10 | 0 ± 0 | 0 | 0 ± 0 | 0 | 0 | 23160 ± 0 | 23160 | 0 |
| run_A3 | XMSSMT-SHA2_20/4_256 | HW | sign | 10 | 1180 ± 65 | 1308 | 32 ± 0 | 32 | 32 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSSMT-SHA2_20/4_256 | HW | verify | 10 | 3585 ± 56 | 3708 | 8 ± 0 | 8 | 8 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSSMT-SHA2_60/12_256 | HW | keygen | 10 | 3592 ± 18 | 3612 | 74332 ± 0 | 74332 | 74332 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSSMT-SHA2_60/12_256 | HW | key-in-ram | 10 | 0 ± 0 | 0 | 0 ± 0 | 0 | 0 | 71432 ± 0 | 71432 | 0 |
| run_A3 | XMSSMT-SHA2_60/12_256 | HW | sign | 10 | 1206 ± 66 | 1308 | 32 ± 0 | 32 | 32 | 0 ± 0 | 0 | 0 |
| run_A3 | XMSSMT-SHA2_60/12_256 | HW | verify | 10 | 3628 ± 56 | 3740 | 8 ± 0 | 8 | 8 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-KEM-512 | stack | keygen | 10 | 7708 ± 0 | 7708 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-KEM-512 | stack | encaps | 10 | 10193 ± 106 | 10432 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-KEM-512 | stack | decaps | 10 | 10834 ± 5 | 10848 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-KEM-768 | stack | keygen | 10 | 11788 ± 0 | 11788 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-KEM-768 | stack | encaps | 10 | 14979 ± 22 | 15040 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-KEM-768 | stack | decaps | 10 | 15972 ± 27 | 16048 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-KEM-1024 | stack | keygen | 10 | 17187 ± 22 | 17248 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-KEM-1024 | stack | encaps | 10 | 20860 ± 0 | 20860 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-KEM-1024 | stack | decaps | 10 | 22353 ± 28 | 22432 | 0 ± 0 | 0 | 0 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-DSA-44 | fast-sign | keygen | 10 | 1876 ± 27 | 1952 | 27056 ± 0 | 27056 | 27056 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-DSA-44 | fast-sign | sign | 10 | 1933 ± 35 | 2000 | 44672 ± 0 | 44672 | 44672 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-DSA-44 | fast-sign | verify | 10 | 2319 ± 98 | 2380 | 24592 ± 0 | 24592 | 24592 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-DSA-65 | fast-sign | keygen | 10 | 1876 ± 27 | 1952 | 44464 ± 0 | 44464 | 44464 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-DSA-65 | fast-sign | sign | 10 | 1949 ± 35 | 2016 | 68256 ± 0 | 68256 | 68256 | 0 ± 0 | 0 | 0 |
| run_B1 | ML-DSA-65 | fast-sign | verify | 10 | 2380 ± 0 | 2380 | 40016 ± 0 | 40016 | 40016 | 0 ± 0 | 0 | 0 |
| run_B2 | ML-DSA-87 | fast-sign | keygen | 10 | 1910 ± 44 | 1952 | 75184 ± 0 | 75184 | 75184 | 0 ± 0 | 0 | 0 |
| run_B2 | ML-DSA-87 | fast-sign | sign | 10 | 1982 ± 43 | 2016 | 107424 ± 0 | 107424 | 107424 | 0 ± 0 | 0 | 0 |
| run_B2 | ML-DSA-87 | fast-sign | verify | 10 | 2422 ± 44 | 2464 | 68944 ± 0 | 68944 | 68944 | 0 ± 0 | 0 | 0 |

## State management (NVS storage, SHA hardware on)

| Scheme | Item | N | Mean ± SD | Median | Max |
|---|---|--:|--:|--:|--:|
| NVS | nvs-write-8B | 200 | 4.24 ms ± 10.5 ms | 2.07 ms | 70.2 ms |
| LMS-SHA256-M32-H10-W4 | keygen-nvs | 10 | 5.65 s ± 5.28 ms | 5.65 s | 5.65 s |
| LMS-SHA256-M32-H10-W4 | sign-nvs | 100 | 14.2 ms ± 18.4 ms | 11.9 ms | 98.2 ms |
| LMS-SHA256-M32-H10-W4 | load-nvs | 10 | 12.6 ms ± 0.31 ms | 12.5 ms | 13.4 ms |
| LMS-SHA256-M32-H10-W4 | first-sign-after-load | 10 | 193.1 ms ± 3.55 ms | 191.2 ms | 201.1 ms |
| LMS-SHA256-M32-H10-W4 | second-sign-after-load | 10 | 27.5 ms ± 28.1 ms | 17.6 ms | 107.4 ms |
| XMSSMT-SHA2_20/4_192 | keygen-nvs | 10 | 2.32 s ± 26.0 ms | 2.32 s | 2.33 s |
| XMSSMT-SHA2_20/4_192 | sign-nvs | 100 | 59.6 ms ± 132.5 ms | 26.4 ms | 842.8 ms |
| XMSSMT-SHA2_20/4_192 | load-nvs | 10 | 25.4 ms ± 0.16 ms | 25.4 ms | 25.6 ms |
| XMSSMT-SHA2_20/4_192 | first-sign-after-load | 10 | 195.0 ms ± 44.6 ms | 183.2 ms | 274.5 ms |
| XMSSMT-SHA2_20/4_192 | second-sign-after-load | 10 | 71.8 ms ± 62.9 ms | 42.7 ms | 190.2 ms |
| LMS-SHA256-M32-H10-W4 | cache_ram_bytes | | 5120.0 | | |
| XMSSMT-SHA2_20/4_192 | cache_ram_bytes | | 2496.0 | | |
| LMS-SHA256-M32-H5-W4 | cache_ram_bytes | | 1024 | | |
| LMS-SHA256-M32-H10-W4 | cache_ram_bytes | | 5120 | | |
| LMS-SHA256-M32-H15-W4 | cache_ram_bytes | | 32768 | | |
| HSS-SHA256-M32-H10+H10-W4 | cache_ram_bytes | | 9216 | | |
| XMSS-SHA2_10_256 | cache_ram_bytes | | 5120 | | |
| XMSSMT-SHA2_20/2_256 | cache_ram_bytes | | 9216 | | |
| XMSSMT-SHA2_20/4_256 | cache_ram_bytes | | 3328 | | |
| XMSSMT-SHA2_60/12_256 | cache_ram_bytes | | 9472 | | |
