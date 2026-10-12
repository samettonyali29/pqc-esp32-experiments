# pqc-benchmark summary

- run_A: `E,cpu_mhz=240,idf=v5.5.5,arduino=3.3.11,radio=off,mlkem=heap,mldsa=reduce-ram,slh_hw=1,lms_hw=1,xmss_hw=1,free_heap=2219112,free_internal=124008,n_fast=1000,n_slh_f=10,n_slh_s=3,n_hbs_sign=256`
- run_B: `E,cpu_mhz=240,idf=v5.5.5,arduino=3.3.11,radio=off,mlkem=stack,mldsa=fast-sign,slh_hw=1,lms_hw=1,xmss_hw=1,free_heap=2219180,free_internal=124076,n_fast=1000,n_slh_f=10,n_slh_s=3,n_hbs_sign=256`
- run_B2: `E,cpu_mhz=240,idf=v5.5.5,arduino=3.3.11,radio=off,mlkem=stack,mldsa=fast-sign,slh_hw=1,lms_hw=1,xmss_hw=1,free_heap=2237916,free_internal=142812,n_fast=1000,n_slh_f=10,n_slh_s=3,n_hbs_sign=256`
- run_A2_mem: `E,cpu_mhz=240,idf=v5.5.5,arduino=3.3.11,radio=off,mlkem=heap,mldsa=reduce-ram,slh_hw=1,lms_hw=1,xmss_hw=1,free_heap=2238048,free_internal=142944,n_fast=1000,n_slh_f=10,n_slh_s=3,n_hbs_sign=256`
- run_B3_dsamem: `E,cpu_mhz=240,idf=v5.5.5,arduino=3.3.11,radio=off,mlkem=stack,mldsa=fast-sign,slh_hw=1,lms_hw=1,xmss_hw=1,free_heap=2238048,free_internal=142944,n_fast=1000,n_slh_f=10,n_slh_s=3,n_hbs_sign=256`


## ML-KEM (run_A, radio on)

| Scheme | Impl | Operation | N | Mean | SD | Median | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| ML-KEM-512 | heap | keygen | 1000 | 4.05 ms | 0.02 ms | 4.05 ms | 4.34 ms | 972 k | 0 |
| ML-KEM-512 | heap | encaps | 1000 | 5.13 ms | 0.02 ms | 5.12 ms | 5.25 ms | 1.23 M | 0 |
| ML-KEM-512 | heap | decaps | 1000 | 6.62 ms | 0.02 ms | 6.61 ms | 6.73 ms | 1.59 M | 0 |
| ML-KEM-768 | heap | keygen | 1000 | 6.40 ms | 0.03 ms | 6.39 ms | 6.73 ms | 1.54 M | 0 |
| ML-KEM-768 | heap | encaps | 1000 | 7.88 ms | 0.03 ms | 7.88 ms | 8.11 ms | 1.89 M | 0 |
| ML-KEM-768 | heap | decaps | 1000 | 9.80 ms | 0.03 ms | 9.79 ms | 10.0 ms | 2.35 M | 0 |
| ML-KEM-1024 | heap | keygen | 1000 | 9.76 ms | 0.03 ms | 9.75 ms | 9.96 ms | 2.34 M | 0 |
| ML-KEM-1024 | heap | encaps | 1000 | 11.5 ms | 0.03 ms | 11.5 ms | 11.7 ms | 2.75 M | 0 |
| ML-KEM-1024 | heap | decaps | 1000 | 13.9 ms | 0.03 ms | 13.8 ms | 14.1 ms | 3.33 M | 0 |

## ML-DSA (run_A, radio on)

| Scheme | Impl | Operation | N | Mean | SD | Median | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| ML-DSA-44 | reduce-ram | keygen | 1000 | 13.1 ms | 0.12 ms | 13.1 ms | 13.5 ms | 3.15 M | 0 |
| ML-DSA-44 | reduce-ram | sign | 1000 | 77.5 ms | 65.6 ms | 56.1 ms | 548.0 ms | 18.61 M | 0 |
| ML-DSA-44 | reduce-ram | verify | 1000 | 13.8 ms | 0.02 ms | 13.8 ms | 13.9 ms | 3.32 M | 0 |
| ML-DSA-65 | reduce-ram | keygen | 1000 | 22.7 ms | 0.01 ms | 22.7 ms | 22.9 ms | 5.45 M | 0 |
| ML-DSA-65 | reduce-ram | sign | 1000 | 141.7 ms | 119.0 ms | 110.1 ms | 900.0 ms | 34.00 M | 0 |
| ML-DSA-65 | reduce-ram | verify | 1000 | 22.9 ms | 0.01 ms | 22.9 ms | 22.9 ms | 5.49 M | 0 |
| ML-DSA-87 | reduce-ram | keygen | 1000 | 38.2 ms | 0.17 ms | 38.2 ms | 38.7 ms | 9.17 M | 0 |
| ML-DSA-87 | reduce-ram | sign | 1000 | 185.1 ms | 144.8 ms | 142.2 ms | 896.9 ms | 44.43 M | 0 |
| ML-DSA-87 | reduce-ram | verify | 1000 | 38.8 ms | 0.01 ms | 38.8 ms | 38.9 ms | 9.31 M | 0 |

## LMS / HSS (run_A, radio on)

| Scheme | Impl | Operation | N | Mean | SD | Median | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| LMS-SHA256-M32-H5-W1 | SW | keygen | 5 | 513.0 ms | 0.06 ms | 513.0 ms | 513.0 ms | 123.11 M | 0 |
| LMS-SHA256-M32-H5-W1 | SW | sign | 32 | 18.1 ms | 8.12 ms | 18.0 ms | 26.9 ms | 4.34 M | 0 |
| LMS-SHA256-M32-H5-W1 | SW | verify | 32 | 8.04 ms | 0.20 ms | 7.96 ms | 8.50 ms | 1.93 M | 0 |
| LMS-SHA256-M32-H5-W1 | HW | keygen | 5 | 221.1 ms | 0.46 ms | 220.8 ms | 221.7 ms | 53.05 M | 0 |
| LMS-SHA256-M32-H5-W1 | HW | sign | 32 | 6.64 ms | 3.52 ms | 6.75 ms | 11.0 ms | 1.59 M | 0 |
| LMS-SHA256-M32-H5-W1 | HW | verify | 32 | 5.79 ms | 0.03 ms | 5.80 ms | 5.84 ms | 1.39 M | 0 |
| LMS-SHA256-M32-H5-W2 | SW | keygen | 5 | 438.8 ms | 0.33 ms | 438.9 ms | 439.0 ms | 105.30 M | 0 |
| LMS-SHA256-M32-H5-W2 | SW | sign | 32 | 14.7 ms | 6.95 ms | 14.9 ms | 22.4 ms | 3.53 M | 0 |
| LMS-SHA256-M32-H5-W2 | SW | verify | 32 | 7.09 ms | 0.25 ms | 7.19 ms | 7.49 ms | 1.70 M | 0 |
| LMS-SHA256-M32-H5-W2 | HW | keygen | 5 | 146.0 ms | 0.34 ms | 145.7 ms | 146.4 ms | 35.03 M | 0 |
| LMS-SHA256-M32-H5-W2 | HW | sign | 32 | 4.44 ms | 2.31 ms | 4.59 ms | 7.10 ms | 1.07 M | 0 |
| LMS-SHA256-M32-H5-W2 | HW | verify | 32 | 3.63 ms | 0.05 ms | 3.63 ms | 3.72 ms | 871 k | 0 |
| LMS-SHA256-M32-H5-W4 | SW | keygen | 5 | 764.4 ms | 0.39 ms | 764.7 ms | 764.8 ms | 183.47 M | 0 |
| LMS-SHA256-M32-H5-W4 | SW | sign | 32 | 24.2 ms | 12.4 ms | 24.2 ms | 38.8 ms | 5.82 M | 0 |
| LMS-SHA256-M32-H5-W4 | SW | verify | 32 | 12.4 ms | 0.83 ms | 12.4 ms | 14.3 ms | 2.98 M | 0 |
| LMS-SHA256-M32-H5-W4 | HW | keygen | 5 | 173.7 ms | 0.51 ms | 173.3 ms | 174.4 ms | 41.68 M | 0 |
| LMS-SHA256-M32-H5-W4 | HW | sign | 32 | 5.37 ms | 2.82 ms | 5.51 ms | 8.49 ms | 1.29 M | 0 |
| LMS-SHA256-M32-H5-W4 | HW | verify | 32 | 3.61 ms | 0.17 ms | 3.58 ms | 3.99 ms | 865 k | 0 |
| LMS-SHA256-M32-H5-W8 | SW | keygen | 5 | 5.88 s | 0.40 ms | 5.88 s | 5.88 s | 1.41 G | 0 |
| LMS-SHA256-M32-H5-W8 | SW | sign | 32 | 184.0 ms | 93.9 ms | 181.1 ms | 298.0 ms | 44.15 M | 0 |
| LMS-SHA256-M32-H5-W8 | SW | verify | 32 | 92.1 ms | 9.71 ms | 92.1 ms | 108.1 ms | 22.09 M | 0 |
| LMS-SHA256-M32-H5-W8 | HW | keygen | 5 | 1.08 s | 0.24 ms | 1.08 s | 1.08 s | 259.05 M | 0 |
| LMS-SHA256-M32-H5-W8 | HW | sign | 32 | 33.4 ms | 17.0 ms | 33.2 ms | 52.4 ms | 8.01 M | 0 |
| LMS-SHA256-M32-H5-W8 | HW | verify | 32 | 17.7 ms | 1.35 ms | 17.4 ms | 20.3 ms | 4.25 M | 0 |
| LMS-SHA256-M32-H10-W2 | SW | keygen | 1 | 14.02 s | 0.00 ms | 14.02 s | 14.02 s | 3.36 G | 0 |
| LMS-SHA256-M32-H10-W2 | SW | sign | 256 | 19.9 ms | 4.57 ms | 21.4 ms | 24.2 ms | 4.78 M | 0 |
| LMS-SHA256-M32-H10-W2 | SW | verify | 256 | 7.34 ms | 0.28 ms | 7.26 ms | 8.20 ms | 1.76 M | 0 |
| LMS-SHA256-M32-H10-W2 | HW | keygen | 1 | 4.65 s | 0.00 ms | 4.65 s | 4.65 s | 1.12 G | 0 |
| LMS-SHA256-M32-H10-W2 | HW | sign | 256 | 6.16 ms | 1.55 ms | 6.64 ms | 8.85 ms | 1.48 M | 0 |
| LMS-SHA256-M32-H10-W2 | HW | verify | 256 | 3.93 ms | 0.05 ms | 3.94 ms | 4.07 ms | 943 k | 0 |
| LMS-SHA256-M32-H10-W4 | SW | keygen | 1 | 24.45 s | 0.00 ms | 24.45 s | 24.45 s | 5.87 G | 0 |
| LMS-SHA256-M32-H10-W4 | SW | sign | 256 | 33.3 ms | 8.08 ms | 36.1 ms | 38.9 ms | 7.99 M | 0 |
| LMS-SHA256-M32-H10-W4 | SW | verify | 256 | 12.6 ms | 0.85 ms | 12.7 ms | 14.9 ms | 3.02 M | 0 |
| LMS-SHA256-M32-H10-W4 | HW | keygen | 1 | 5.54 s | 0.00 ms | 5.54 s | 5.54 s | 1.33 G | 0 |
| LMS-SHA256-M32-H10-W4 | HW | sign | 256 | 7.43 ms | 1.82 ms | 8.01 ms | 9.96 ms | 1.78 M | 0 |
| LMS-SHA256-M32-H10-W4 | HW | verify | 256 | 3.87 ms | 0.15 ms | 3.83 ms | 4.29 ms | 929 k | 0 |
| LMS-SHA256-M32-H10-W8 | SW | keygen | 1 | 3.14 min | 0.00 ms | 3.14 min | 3.14 min | 45.15 G | 0 |
| LMS-SHA256-M32-H10-W8 | SW | sign | 256 | 250.1 ms | 61.3 ms | 270.2 ms | 296.9 ms | 60.03 M | 0 |
| LMS-SHA256-M32-H10-W8 | SW | verify | 256 | 95.1 ms | 8.92 ms | 97.7 ms | 119.1 ms | 22.83 M | 0 |
| LMS-SHA256-M32-H10-W8 | HW | keygen | 1 | 34.54 s | 0.00 ms | 34.54 s | 34.54 s | 8.29 G | 0 |
| LMS-SHA256-M32-H10-W8 | HW | sign | 256 | 46.1 ms | 11.3 ms | 50.2 ms | 54.3 ms | 11.06 M | 0 |
| LMS-SHA256-M32-H10-W8 | HW | verify | 256 | 18.0 ms | 1.60 ms | 17.7 ms | 21.6 ms | 4.31 M | 0 |
| LMS-SHA256-M24-H10-W4 | SW | keygen | 1 | 18.28 s | 0.00 ms | 18.28 s | 18.28 s | 4.39 G | 0 |
| LMS-SHA256-M24-H10-W4 | SW | sign | 256 | 24.9 ms | 5.96 ms | 27.0 ms | 29.2 ms | 5.97 M | 0 |
| LMS-SHA256-M24-H10-W4 | SW | verify | 256 | 9.59 ms | 0.64 ms | 9.57 ms | 11.4 ms | 2.30 M | 0 |
| LMS-SHA256-M24-H10-W4 | HW | keygen | 1 | 3.90 s | 0.00 ms | 3.90 s | 3.90 s | 935.73 M | 0 |
| LMS-SHA256-M24-H10-W4 | HW | sign | 256 | 5.26 ms | 1.31 ms | 5.66 ms | 8.44 ms | 1.26 M | 0 |
| LMS-SHA256-M24-H10-W4 | HW | verify | 256 | 2.86 ms | 0.11 ms | 2.87 ms | 3.23 ms | 687 k | 0 |
| LMS-SHA256-M24-H10-W8 | SW | keygen | 1 | 2.38 min | 0.00 ms | 2.38 min | 2.38 min | 34.24 G | 0 |
| LMS-SHA256-M24-H10-W8 | SW | sign | 256 | 189.2 ms | 45.8 ms | 204.0 ms | 226.0 ms | 45.41 M | 0 |
| LMS-SHA256-M24-H10-W8 | SW | verify | 256 | 72.9 ms | 7.51 ms | 75.5 ms | 91.4 ms | 17.49 M | 0 |
| LMS-SHA256-M24-H10-W8 | HW | keygen | 1 | 25.23 s | 0.00 ms | 25.23 s | 25.23 s | 6.05 G | 0 |
| LMS-SHA256-M24-H10-W8 | HW | sign | 256 | 33.4 ms | 8.10 ms | 36.0 ms | 41.3 ms | 8.02 M | 0 |
| LMS-SHA256-M24-H10-W8 | HW | verify | 256 | 13.6 ms | 1.37 ms | 13.1 ms | 16.8 ms | 3.26 M | 0 |
| LMS-SHAKE-M32-H10-W4 | SW | keygen | 1 | 2.32 min | 0.00 ms | 2.32 min | 2.32 min | 33.34 G | 0 |
| LMS-SHAKE-M32-H10-W4 | SW | sign | 256 | 189.3 ms | 45.5 ms | 205.6 ms | 216.7 ms | 45.43 M | 0 |
| LMS-SHAKE-M32-H10-W4 | SW | verify | 256 | 66.8 ms | 4.33 ms | 65.7 ms | 80.5 ms | 16.04 M | 0 |
| LMS-SHAKE-M24-H10-W4 | SW | keygen | 1 | 1.74 min | 0.00 ms | 1.74 min | 1.74 min | 25.01 G | 0 |
| LMS-SHAKE-M24-H10-W4 | SW | sign | 256 | 141.7 ms | 33.7 ms | 154.0 ms | 165.3 ms | 34.01 M | 0 |
| LMS-SHAKE-M24-H10-W4 | SW | verify | 256 | 51.0 ms | 3.70 ms | 51.1 ms | 60.3 ms | 12.24 M | 0 |
| LMS-SHA256-M32-H15-W4 | HW | keygen | 1 | 2.95 min | 0.00 ms | 2.95 min | 2.95 min | 42.55 G | 0 |
| LMS-SHA256-M32-H15-W4 | HW | sign | 256 | 2.66 ms | 0.14 ms | 2.66 ms | 3.01 ms | 639 k | 0 |
| LMS-SHA256-M32-H15-W4 | HW | verify | 256 | 4.19 ms | 0.15 ms | 4.18 ms | 4.65 ms | 1.01 M | 0 |
| HSS-SHA256-M32-H10+H10-W4 | SW | keygen | 1 | 48.91 s | 0.00 ms | 48.91 s | 48.91 s | 11.74 G | 0 |
| HSS-SHA256-M32-H10+H10-W4 | SW | sign | 256 | 33.3 ms | 8.08 ms | 36.1 ms | 38.7 ms | 7.99 M | 0 |
| HSS-SHA256-M32-H10+H10-W4 | SW | verify | 256 | 23.7 ms | 0.82 ms | 23.7 ms | 25.7 ms | 5.70 M | 0 |
| HSS-SHA256-M32-H10+H10-W4 | HW | keygen | 1 | 11.08 s | 0.00 ms | 11.08 s | 11.08 s | 2.66 G | 0 |
| HSS-SHA256-M32-H10+H10-W4 | HW | sign | 256 | 7.45 ms | 1.86 ms | 8.02 ms | 10.9 ms | 1.79 M | 0 |
| HSS-SHA256-M32-H10+H10-W4 | HW | verify | 256 | 7.64 ms | 0.14 ms | 7.64 ms | 8.01 ms | 1.83 M | 0 |
| HSS-SHA256-M24-H10+H10-W4 | SW | keygen | 1 | 36.57 s | 0.00 ms | 36.57 s | 36.57 s | 8.78 G | 0 |
| HSS-SHA256-M24-H10+H10-W4 | SW | sign | 256 | 24.9 ms | 5.99 ms | 27.0 ms | 29.4 ms | 5.98 M | 0 |
| HSS-SHA256-M24-H10+H10-W4 | SW | verify | 256 | 19.5 ms | 0.65 ms | 19.5 ms | 21.4 ms | 4.68 M | 0 |
| HSS-SHA256-M24-H10+H10-W4 | HW | keygen | 1 | 7.80 s | 0.00 ms | 7.80 s | 7.80 s | 1.87 G | 0 |
| HSS-SHA256-M24-H10+H10-W4 | HW | sign | 256 | 5.28 ms | 1.31 ms | 5.67 ms | 8.27 ms | 1.27 M | 0 |
| HSS-SHA256-M24-H10+H10-W4 | HW | verify | 256 | 5.78 ms | 0.11 ms | 5.79 ms | 6.10 ms | 1.39 M | 0 |

## XMSS / XMSS^MT (run_A, radio on)

| Scheme | Impl | Operation | N | Mean | SD | Median | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| XMSS-SHA2_10_256 | SW | keygen | 1 | 1.66 min | 0.00 ms | 1.66 min | 1.66 min | 23.95 G | 0 |
| XMSS-SHA2_10_256 | SW | sign | 256 | 131.1 ms | 32.4 ms | 142.6 ms | 152.3 ms | 31.47 M | 0 |
| XMSS-SHA2_10_256 | SW | verify | 256 | 53.3 ms | 3.44 ms | 52.4 ms | 65.2 ms | 12.78 M | 0 |
| XMSS-SHA2_10_256 | HW | keygen | 1 | 23.93 s | 0.00 ms | 23.93 s | 23.93 s | 5.74 G | 0 |
| XMSS-SHA2_10_256 | HW | sign | 256 | 31.5 ms | 7.78 ms | 34.3 ms | 38.4 ms | 7.57 M | 0 |
| XMSS-SHA2_10_256 | HW | verify | 256 | 13.1 ms | 0.82 ms | 13.1 ms | 15.3 ms | 3.14 M | 0 |
| XMSS-SHA2_10_192 | SW | keygen | 1 | 1.58 min | 0.00 ms | 1.58 min | 1.58 min | 22.78 G | 0 |
| XMSS-SHA2_10_192 | SW | sign | 256 | 123.8 ms | 31.1 ms | 134.9 ms | 144.5 ms | 29.71 M | 0 |
| XMSS-SHA2_10_192 | SW | verify | 256 | 52.1 ms | 3.29 ms | 52.1 ms | 61.7 ms | 12.50 M | 0 |
| XMSS-SHA2_10_192 | HW | keygen | 1 | 16.07 s | 0.00 ms | 16.07 s | 16.07 s | 3.86 G | 0 |
| XMSS-SHA2_10_192 | HW | sign | 256 | 21.2 ms | 5.29 ms | 22.9 ms | 27.7 ms | 5.08 M | 0 |
| XMSS-SHA2_10_192 | HW | verify | 256 | 8.86 ms | 0.54 ms | 8.91 ms | 10.3 ms | 2.13 M | 0 |
| XMSS-SHAKE256_10_256 | SW | keygen | 1 | 8.90 min | 0.00 ms | 8.90 min | 8.90 min | 128.19 G | 0 |
| XMSS-SHAKE256_10_256 | SW | sign | 256 | 699.6 ms | 173.6 ms | 762.3 ms | 804.4 ms | 167.91 M | 0 |
| XMSS-SHAKE256_10_256 | SW | verify | 256 | 283.9 ms | 17.2 ms | 285.7 ms | 327.6 ms | 68.15 M | 0 |
| XMSS-SHAKE256_10_192 | SW | keygen | 1 | 6.13 min | 0.00 ms | 6.13 min | 6.13 min | 88.25 G | 0 |
| XMSS-SHAKE256_10_192 | SW | sign | 256 | 481.5 ms | 120.1 ms | 524.6 ms | 563.2 ms | 115.56 M | 0 |
| XMSS-SHAKE256_10_192 | SW | verify | 256 | 197.8 ms | 12.8 ms | 198.8 ms | 236.7 ms | 47.46 M | 0 |
| XMSSMT-SHA2_20/2_256 | SW | keygen | 1 | 3.33 min | 0.00 ms | 3.33 min | 3.33 min | 47.91 G | 0 |
| XMSSMT-SHA2_20/2_256 | SW | sign | 256 | 131.1 ms | 32.6 ms | 143.4 ms | 152.5 ms | 31.47 M | 0 |
| XMSSMT-SHA2_20/2_256 | SW | verify | 256 | 110.8 ms | 3.49 ms | 109.8 ms | 120.0 ms | 26.59 M | 0 |
| XMSSMT-SHA2_20/2_256 | HW | keygen | 1 | 47.87 s | 0.00 ms | 47.87 s | 47.87 s | 11.49 G | 0 |
| XMSSMT-SHA2_20/2_256 | HW | sign | 256 | 31.7 ms | 7.76 ms | 34.3 ms | 39.3 ms | 7.60 M | 0 |
| XMSSMT-SHA2_20/2_256 | HW | verify | 256 | 26.9 ms | 0.86 ms | 26.8 ms | 29.2 ms | 6.46 M | 0 |
| XMSSMT-SHA2_20/4_256 | SW | keygen | 3 | 12.61 s | 3.42 ms | 12.61 s | 12.62 s | 3.03 G | 0 |
| XMSSMT-SHA2_20/4_256 | SW | sign | 32 | 94.6 ms | 49.4 ms | 93.9 ms | 146.5 ms | 22.71 M | 0 |
| XMSSMT-SHA2_20/4_256 | SW | verify | 32 | 207.6 ms | 2.61 ms | 206.6 ms | 213.0 ms | 49.81 M | 0 |
| XMSSMT-SHA2_20/4_256 | HW | keygen | 3 | 3.03 s | 1.66 ms | 3.03 s | 3.03 s | 726.47 M | 0 |
| XMSSMT-SHA2_20/4_256 | HW | sign | 32 | 23.1 ms | 11.7 ms | 23.1 ms | 36.2 ms | 5.55 M | 0 |
| XMSSMT-SHA2_20/4_256 | HW | verify | 32 | 52.8 ms | 0.68 ms | 52.7 ms | 54.3 ms | 12.67 M | 0 |
| XMSSMT-SHA2_20/4_192 | SW | keygen | 3 | 12.01 s | 4.76 ms | 12.01 s | 12.01 s | 2.88 G | 0 |
| XMSSMT-SHA2_20/4_192 | SW | sign | 32 | 89.0 ms | 46.9 ms | 90.7 ms | 141.6 ms | 21.37 M | 0 |
| XMSSMT-SHA2_20/4_192 | SW | verify | 32 | 198.4 ms | 3.63 ms | 199.2 ms | 204.8 ms | 47.61 M | 0 |
| XMSSMT-SHA2_20/4_192 | HW | keygen | 3 | 2.03 s | 0.87 ms | 2.03 s | 2.03 s | 487.75 M | 0 |
| XMSSMT-SHA2_20/4_192 | HW | sign | 32 | 15.4 ms | 7.85 ms | 15.4 ms | 24.3 ms | 3.69 M | 0 |
| XMSSMT-SHA2_20/4_192 | HW | verify | 32 | 35.3 ms | 0.46 ms | 35.4 ms | 36.5 ms | 8.47 M | 0 |
| XMSSMT-SHA2_40/8_256 | SW | keygen | 3 | 25.27 s | 4.94 ms | 25.27 s | 25.27 s | 6.06 G | 0 |
| XMSSMT-SHA2_40/8_256 | SW | sign | 32 | 95.0 ms | 51.6 ms | 95.7 ms | 148.8 ms | 22.79 M | 0 |
| XMSSMT-SHA2_40/8_256 | SW | verify | 32 | 422.9 ms | 3.30 ms | 422.8 ms | 432.4 ms | 101.49 M | 0 |
| XMSSMT-SHA2_40/8_256 | HW | keygen | 3 | 6.07 s | 1.70 ms | 6.07 s | 6.07 s | 1.46 G | 0 |
| XMSSMT-SHA2_40/8_256 | HW | sign | 32 | 23.0 ms | 12.0 ms | 23.0 ms | 35.6 ms | 5.51 M | 0 |
| XMSSMT-SHA2_40/8_256 | HW | verify | 32 | 101.3 ms | 0.83 ms | 101.1 ms | 102.9 ms | 24.30 M | 0 |
| XMSSMT-SHA2_60/12_256 | SW | keygen | 3 | 37.94 s | 1.65 ms | 37.94 s | 37.94 s | 9.10 G | 0 |
| XMSSMT-SHA2_60/12_256 | SW | sign | 32 | 95.1 ms | 49.6 ms | 94.4 ms | 150.0 ms | 22.81 M | 0 |
| XMSSMT-SHA2_60/12_256 | SW | verify | 32 | 633.3 ms | 3.09 ms | 633.8 ms | 638.9 ms | 151.98 M | 0 |
| XMSSMT-SHA2_60/12_256 | HW | keygen | 3 | 9.10 s | 3.83 ms | 9.10 s | 9.11 s | 2.18 G | 0 |
| XMSSMT-SHA2_60/12_256 | HW | sign | 32 | 23.1 ms | 12.0 ms | 23.1 ms | 36.1 ms | 5.55 M | 0 |
| XMSSMT-SHA2_60/12_256 | HW | verify | 32 | 155.4 ms | 0.90 ms | 155.3 ms | 157.1 ms | 37.29 M | 0 |
| XMSSMT-SHAKE256_20/4_256 | SW | keygen | 3 | 1.13 min | 40.1 ms | 1.13 min | 1.13 min | 16.20 G | 0 |
| XMSSMT-SHAKE256_20/4_256 | SW | sign | 32 | 505.1 ms | 264.6 ms | 502.3 ms | 804.7 ms | 121.23 M | 0 |
| XMSSMT-SHAKE256_20/4_256 | SW | verify | 32 | 1.08 s | 18.1 ms | 1.08 s | 1.12 s | 260.37 M | 0 |
| XMSSMT-SHAKE256_20/4_192 | SW | keygen | 3 | 46.44 s | 32.2 ms | 46.44 s | 46.48 s | 11.15 G | 0 |
| XMSSMT-SHAKE256_20/4_192 | SW | sign | 32 | 348.3 ms | 186.3 ms | 342.2 ms | 556.4 ms | 83.60 M | 0 |
| XMSSMT-SHAKE256_20/4_192 | SW | verify | 32 | 761.7 ms | 12.8 ms | 764.2 ms | 795.8 ms | 182.80 M | 0 |

## SLH-DSA (run_A, radio on)

| Scheme | Impl | Operation | N | Mean | SD | Median | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| SLH-DSA-SHA2-128f | SW | keygen | 10 | 111.1 ms | 0.41 ms | 110.8 ms | 111.9 ms | 26.66 M | 0 |
| SLH-DSA-SHA2-128f | SW | sign | 10 | 2.61 s | 0.57 ms | 2.61 s | 2.61 s | 626.88 M | 0 |
| SLH-DSA-SHA2-128f | SW | verify | 10 | 156.6 ms | 0.44 ms | 156.4 ms | 157.3 ms | 37.60 M | 0 |
| SLH-DSA-SHA2-128f | HW | keygen | 10 | 45.3 ms | 0.34 ms | 45.2 ms | 46.3 ms | 10.88 M | 0 |
| SLH-DSA-SHA2-128f | HW | sign | 10 | 1.06 s | 0.37 ms | 1.06 s | 1.06 s | 255.00 M | 0 |
| SLH-DSA-SHA2-128f | HW | verify | 10 | 63.7 ms | 0.46 ms | 63.5 ms | 64.6 ms | 15.29 M | 0 |
| SLH-DSA-SHA2-192f | SW | keygen | 10 | 166.4 ms | 0.41 ms | 166.2 ms | 167.2 ms | 39.93 M | 0 |
| SLH-DSA-SHA2-192f | SW | sign | 10 | 4.92 s | 0.43 ms | 4.92 s | 4.92 s | 1.18 G | 0 |
| SLH-DSA-SHA2-192f | SW | verify | 10 | 269.8 ms | 0.55 ms | 270.2 ms | 270.3 ms | 64.76 M | 0 |
| SLH-DSA-SHA2-192f | HW | keygen | 10 | 72.6 ms | 0.37 ms | 72.5 ms | 73.7 ms | 17.43 M | 0 |
| SLH-DSA-SHA2-192f | HW | sign | 10 | 1.98 s | 0.28 ms | 1.98 s | 1.98 s | 474.68 M | 0 |
| SLH-DSA-SHA2-192f | HW | verify | 10 | 104.8 ms | 0.48 ms | 104.5 ms | 105.7 ms | 25.15 M | 0 |
| SLH-DSA-SHA2-256f | SW | keygen | 10 | 442.9 ms | 0.32 ms | 442.9 ms | 443.1 ms | 106.29 M | 0 |
| SLH-DSA-SHA2-256f | SW | sign | 10 | 10.20 s | 0.97 ms | 10.20 s | 10.20 s | 2.45 G | 0 |
| SLH-DSA-SHA2-256f | SW | verify | 10 | 274.1 ms | 0.58 ms | 274.1 ms | 274.8 ms | 65.79 M | 0 |
| SLH-DSA-SHA2-256f | HW | keygen | 10 | 212.5 ms | 0.58 ms | 212.1 ms | 213.2 ms | 51.00 M | 0 |
| SLH-DSA-SHA2-256f | HW | sign | 10 | 4.48 s | 0.34 ms | 4.48 s | 4.48 s | 1.07 G | 0 |
| SLH-DSA-SHA2-256f | HW | verify | 10 | 114.7 ms | 0.46 ms | 114.4 ms | 115.5 ms | 27.52 M | 0 |
| SLH-DSA-SHAKE-128f | SW | keygen | 10 | 525.3 ms | 0.22 ms | 525.3 ms | 525.6 ms | 126.07 M | 0 |
| SLH-DSA-SHAKE-128f | SW | sign | 10 | 12.32 s | 3.38 ms | 12.32 s | 12.32 s | 2.96 G | 0 |
| SLH-DSA-SHAKE-128f | SW | verify | 10 | 717.8 ms | 0.61 ms | 717.8 ms | 718.7 ms | 172.28 M | 0 |
| SLH-DSA-SHAKE-192f | SW | keygen | 10 | 768.6 ms | 0.57 ms | 768.8 ms | 769.3 ms | 184.46 M | 0 |
| SLH-DSA-SHAKE-192f | SW | sign | 10 | 20.14 s | 4.27 ms | 20.14 s | 20.14 s | 4.83 G | 0 |
| SLH-DSA-SHAKE-192f | SW | verify | 10 | 1.06 s | 0.52 ms | 1.06 s | 1.06 s | 255.25 M | 0 |
| SLH-DSA-SHAKE-256f | SW | keygen | 10 | 2.03 s | 0.29 ms | 2.03 s | 2.03 s | 487.25 M | 0 |
| SLH-DSA-SHAKE-256f | SW | sign | 10 | 41.59 s | 5.08 ms | 41.59 s | 41.59 s | 9.98 G | 0 |
| SLH-DSA-SHAKE-256f | SW | verify | 10 | 1.08 s | 0.57 ms | 1.08 s | 1.08 s | 259.33 M | 0 |
| SLH-DSA-SHA2-128s | SW | keygen | 3 | 7.11 s | 0.15 ms | 7.11 s | 7.11 s | 1.71 G | 0 |
| SLH-DSA-SHA2-128s | SW | sign | 3 | 54.43 s | 1.13 ms | 54.43 s | 54.43 s | 13.06 G | 0 |
| SLH-DSA-SHA2-128s | SW | verify | 10 | 54.9 ms | 0.28 ms | 54.8 ms | 55.7 ms | 13.18 M | 0 |
| SLH-DSA-SHA2-128s | HW | keygen | 3 | 2.90 s | 0.23 ms | 2.90 s | 2.90 s | 696.29 M | 0 |
| SLH-DSA-SHA2-128s | HW | sign | 3 | 22.09 s | 0.33 ms | 22.09 s | 22.09 s | 5.30 G | 0 |
| SLH-DSA-SHA2-128s | HW | verify | 10 | 21.2 ms | 0.36 ms | 21.1 ms | 22.2 ms | 5.09 M | 0 |
| SLH-DSA-SHA2-192s | SW | keygen | 3 | 10.65 s | 0.20 ms | 10.65 s | 10.65 s | 2.56 G | 0 |
| SLH-DSA-SHA2-192s | SW | sign | 3 | 1.94 min | 7.60 ms | 1.94 min | 1.94 min | 27.89 G | 0 |
| SLH-DSA-SHA2-192s | SW | verify | 10 | 105.5 ms | 0.38 ms | 105.3 ms | 106.4 ms | 25.32 M | 0 |
| SLH-DSA-SHA2-192s | HW | keygen | 3 | 4.65 s | 0.05 ms | 4.65 s | 4.65 s | 1.12 G | 0 |
| SLH-DSA-SHA2-192s | HW | sign | 3 | 45.08 s | 0.76 ms | 45.08 s | 45.08 s | 10.82 G | 0 |
| SLH-DSA-SHA2-192s | HW | verify | 10 | 38.6 ms | 0.37 ms | 38.5 ms | 39.7 ms | 9.27 M | 0 |
| SLH-DSA-SHA2-256s | SW | keygen | 3 | 7.09 s | 0.52 ms | 7.08 s | 7.09 s | 1.70 G | 0 |
| SLH-DSA-SHA2-256s | SW | sign | 3 | 1.84 min | 6.05 ms | 1.84 min | 1.84 min | 26.56 G | 0 |
| SLH-DSA-SHA2-256s | SW | verify | 10 | 145.1 ms | 0.47 ms | 144.8 ms | 145.9 ms | 34.82 M | 0 |
| SLH-DSA-SHA2-256s | HW | keygen | 3 | 3.40 s | 0.12 ms | 3.40 s | 3.40 s | 816.10 M | 0 |
| SLH-DSA-SHA2-256s | HW | sign | 3 | 44.63 s | 0.36 ms | 44.63 s | 44.63 s | 10.71 G | 0 |
| SLH-DSA-SHA2-256s | HW | verify | 10 | 61.4 ms | 0.36 ms | 61.3 ms | 62.4 ms | 14.74 M | 0 |
| SLH-DSA-SHAKE-128s | SW | keygen | 3 | 33.62 s | 0.71 ms | 33.62 s | 33.62 s | 8.07 G | 0 |
| SLH-DSA-SHAKE-128s | SW | sign | 3 | 4.28 min | 0.73 ms | 4.28 min | 4.28 min | 61.67 G | 0 |
| SLH-DSA-SHAKE-128s | SW | verify | 10 | 254.5 ms | 0.48 ms | 254.3 ms | 255.1 ms | 61.07 M | 0 |
| SLH-DSA-SHAKE-192s | SW | keygen | 3 | 49.21 s | 0.61 ms | 49.21 s | 49.21 s | 11.81 G | 0 |
| SLH-DSA-SHAKE-192s | SW | sign | 3 | 7.54 min | 6.90 ms | 7.54 min | 7.54 min | 108.59 G | 0 |
| SLH-DSA-SHAKE-192s | SW | verify | 10 | 364.6 ms | 0.36 ms | 364.6 ms | 365.1 ms | 87.50 M | 0 |
| SLH-DSA-SHAKE-256s | SW | keygen | 3 | 32.49 s | 0.60 ms | 32.49 s | 32.49 s | 7.80 G | 0 |
| SLH-DSA-SHAKE-256s | SW | sign | 3 | 6.72 min | 4.34 ms | 6.72 min | 6.72 min | 96.76 G | 0 |
| SLH-DSA-SHAKE-256s | SW | verify | 10 | 555.0 ms | 0.30 ms | 555.0 ms | 555.5 ms | 133.20 M | 0 |

## ML-KEM (run_B, radio on)

| Scheme | Impl | Operation | N | Mean | SD | Median | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| ML-KEM-512 | stack | keygen | 1000 | 3.70 ms | 0.02 ms | 3.70 ms | 3.98 ms | 889 k | 0 |
| ML-KEM-512 | stack | encaps | 1000 | 4.43 ms | 0.02 ms | 4.43 ms | 4.61 ms | 1.06 M | 0 |
| ML-KEM-512 | stack | decaps | 1000 | 5.67 ms | 0.02 ms | 5.67 ms | 5.85 ms | 1.36 M | 0 |
| ML-KEM-768 | stack | keygen | 1000 | 6.05 ms | 0.03 ms | 6.04 ms | 6.26 ms | 1.45 M | 0 |
| ML-KEM-768 | stack | encaps | 1000 | 7.19 ms | 0.03 ms | 7.18 ms | 7.37 ms | 1.72 M | 0 |
| ML-KEM-768 | stack | decaps | 1000 | 8.88 ms | 0.03 ms | 8.87 ms | 9.06 ms | 2.13 M | 0 |
| ML-KEM-1024 | stack | keygen | 1000 | 9.41 ms | 0.04 ms | 9.40 ms | 9.61 ms | 2.26 M | 0 |
| ML-KEM-1024 | stack | encaps | 1000 | 10.8 ms | 0.04 ms | 10.8 ms | 11.0 ms | 2.59 M | 0 |
| ML-KEM-1024 | stack | decaps | 1000 | 12.9 ms | 0.04 ms | 12.9 ms | 13.1 ms | 3.11 M | 0 |

## ML-DSA (run_B, radio on)

| Scheme | Impl | Operation | N | Mean | SD | Median | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| ML-DSA-44 | fast-sign | keygen | 1000 | 13.1 ms | 0.13 ms | 13.1 ms | 13.6 ms | 3.14 M | 0 |
| ML-DSA-44 | fast-sign | sign | 1000 | 44.2 ms | 27.8 ms | 33.9 ms | 260.6 ms | 10.62 M | 0 |
| ML-DSA-44 | fast-sign | verify | 1000 | 13.8 ms | 0.01 ms | 13.8 ms | 13.9 ms | 3.31 M | 0 |
| ML-DSA-65 | fast-sign | keygen | 1000 | 22.6 ms | 0.01 ms | 22.6 ms | 22.8 ms | 5.43 M | 0 |
| ML-DSA-65 | fast-sign | sign | 1000 | 66.9 ms | 40.7 ms | 55.6 ms | 334.1 ms | 16.05 M | 0 |
| ML-DSA-65 | fast-sign | verify | 1000 | 22.8 ms | 0.01 ms | 22.7 ms | 22.8 ms | 5.46 M | 0 |
| ML-DSA-87 | fast-sign | keygen | 1000 | 37.8 ms | 0.17 ms | 37.8 ms | 38.3 ms | 9.07 M | 0 |
| ML-DSA-87 | fast-sign | sign | 1000 | 33.1 ms | 0.01 ms | 33.1 ms | 33.2 ms | 7.94 M | 1000 |
| ML-DSA-87 | fast-sign | verify | 1000 | 0.00 ms | 0.00 ms | 0.00 ms | 0.01 ms | 0 k | 1000 |

## ML-DSA (run_B2, radio off)

| Scheme | Impl | Operation | N | Mean | SD | Median | Max | Cycles (mean) | Fail |
|---|---|---|--:|--:|--:|--:|--:|--:|--:|
| ML-DSA-44 | fast-sign | keygen | 1000 | 12.8 ms | 0.12 ms | 12.8 ms | 13.2 ms | 3.08 M | 0 |
| ML-DSA-44 | fast-sign | sign | 1000 | 41.6 ms | 25.4 ms | 32.9 ms | 224.1 ms | 9.99 M | 0 |
| ML-DSA-44 | fast-sign | verify | 1000 | 13.5 ms | 0.01 ms | 13.5 ms | 13.6 ms | 3.25 M | 0 |
| ML-DSA-65 | fast-sign | keygen | 1000 | 22.4 ms | 0.01 ms | 22.4 ms | 22.6 ms | 5.37 M | 0 |
| ML-DSA-65 | fast-sign | sign | 1000 | 65.3 ms | 39.7 ms | 53.6 ms | 321.7 ms | 15.66 M | 0 |
| ML-DSA-65 | fast-sign | verify | 1000 | 22.5 ms | 0.01 ms | 22.5 ms | 22.6 ms | 5.41 M | 0 |
| ML-DSA-87 | fast-sign | keygen | 1000 | 37.6 ms | 0.17 ms | 37.6 ms | 38.1 ms | 9.02 M | 0 |
| ML-DSA-87 | fast-sign | sign | 1000 | 82.2 ms | 40.2 ms | 71.0 ms | 343.6 ms | 19.72 M | 0 |
| ML-DSA-87 | fast-sign | verify | 1000 | 38.2 ms | 0.01 ms | 38.2 ms | 38.3 ms | 9.18 M | 0 |

## Radio interference control (same build, radio off vs on)

| Scheme | Impl | Operation | Radio on (mean) | Radio off (mean) | Difference |
|---|---|---|--:|--:|--:|
| ML-KEM-768 | heap | keygen | 6.40 ms | 6.22 ms | +2.90% |
| ML-KEM-768 | heap | encaps | 7.88 ms | 7.55 ms | +4.44% |
| ML-KEM-768 | heap | decaps | 9.80 ms | 9.35 ms | +4.83% |
| ML-DSA-44 | reduce-ram | keygen | 13.1 ms | 13.0 ms | +1.03% |
| ML-DSA-44 | reduce-ram | sign | 77.5 ms | 73.7 ms | +5.21% |
| ML-DSA-44 | reduce-ram | verify | 13.8 ms | 13.7 ms | +1.13% |
| SLH-DSA-SHA2-128f | SW | keygen | 111.1 ms | 111.2 ms | -0.06% |
| SLH-DSA-SHA2-128f | SW | sign | 2.61 s | 2.61 s | +0.00% |
| SLH-DSA-SHA2-128f | SW | verify | 156.6 ms | 159.2 ms | -1.62% |
| SLH-DSA-SHA2-128f | HW | keygen | 45.3 ms | 45.5 ms | -0.26% |
| SLH-DSA-SHA2-128f | HW | sign | 1.06 s | 1.06 s | +0.04% |
| SLH-DSA-SHA2-128f | HW | verify | 63.7 ms | 66.8 ms | -4.68% |
| LMS-SHA256-M32-H10-W4 | HW | keygen | 5.54 s | 5.54 s | -0.01% |
| LMS-SHA256-M32-H10-W4 | HW | sign | 7.43 ms | 7.40 ms | +0.30% |
| LMS-SHA256-M32-H10-W4 | HW | verify | 3.87 ms | 3.87 ms | +0.14% |
| XMSSMT-SHA2_20/4_256 | HW | keygen | 3.03 s | 3.03 s | +0.04% |
| XMSSMT-SHA2_20/4_256 | HW | sign | 23.1 ms | 23.1 ms | +0.05% |
| XMSSMT-SHA2_20/4_256 | HW | verify | 52.8 ms | 51.2 ms | +3.02% |

## SHA hardware speed-up (software mean / hardware mean)

| Scheme | Operation | Software | Hardware | Speed-up |
|---|---|--:|--:|--:|
| LMS-SHA256-M32-H5-W1 | keygen | 513.0 ms | 221.1 ms | 2.32x |
| LMS-SHA256-M32-H5-W1 | sign | 18.1 ms | 6.64 ms | 2.72x |
| LMS-SHA256-M32-H5-W1 | verify | 8.04 ms | 5.79 ms | 1.39x |
| LMS-SHA256-M32-H5-W2 | keygen | 438.8 ms | 146.0 ms | 3.01x |
| LMS-SHA256-M32-H5-W2 | sign | 14.7 ms | 4.44 ms | 3.31x |
| LMS-SHA256-M32-H5-W2 | verify | 7.09 ms | 3.63 ms | 1.95x |
| LMS-SHA256-M32-H5-W4 | keygen | 764.4 ms | 173.7 ms | 4.40x |
| LMS-SHA256-M32-H5-W4 | sign | 24.2 ms | 5.37 ms | 4.51x |
| LMS-SHA256-M32-H5-W4 | verify | 12.4 ms | 3.61 ms | 3.45x |
| LMS-SHA256-M32-H5-W8 | keygen | 5.88 s | 1.08 s | 5.45x |
| LMS-SHA256-M32-H5-W8 | sign | 184.0 ms | 33.4 ms | 5.51x |
| LMS-SHA256-M32-H5-W8 | verify | 92.1 ms | 17.7 ms | 5.19x |
| LMS-SHA256-M32-H10-W2 | keygen | 14.02 s | 4.65 s | 3.01x |
| LMS-SHA256-M32-H10-W2 | sign | 19.9 ms | 6.16 ms | 3.23x |
| LMS-SHA256-M32-H10-W2 | verify | 7.34 ms | 3.93 ms | 1.87x |
| LMS-SHA256-M32-H10-W4 | keygen | 24.45 s | 5.54 s | 4.41x |
| LMS-SHA256-M32-H10-W4 | sign | 33.3 ms | 7.43 ms | 4.48x |
| LMS-SHA256-M32-H10-W4 | verify | 12.6 ms | 3.87 ms | 3.25x |
| LMS-SHA256-M32-H10-W8 | keygen | 3.14 min | 34.54 s | 5.45x |
| LMS-SHA256-M32-H10-W8 | sign | 250.1 ms | 46.1 ms | 5.43x |
| LMS-SHA256-M32-H10-W8 | verify | 95.1 ms | 18.0 ms | 5.29x |
| LMS-SHA256-M24-H10-W4 | keygen | 18.28 s | 3.90 s | 4.69x |
| LMS-SHA256-M24-H10-W4 | sign | 24.9 ms | 5.26 ms | 4.73x |
| LMS-SHA256-M24-H10-W4 | verify | 9.59 ms | 2.86 ms | 3.35x |
| LMS-SHA256-M24-H10-W8 | keygen | 2.38 min | 25.23 s | 5.65x |
| LMS-SHA256-M24-H10-W8 | sign | 189.2 ms | 33.4 ms | 5.66x |
| LMS-SHA256-M24-H10-W8 | verify | 72.9 ms | 13.6 ms | 5.36x |
| HSS-SHA256-M32-H10+H10-W4 | keygen | 48.91 s | 11.08 s | 4.41x |
| HSS-SHA256-M32-H10+H10-W4 | sign | 33.3 ms | 7.45 ms | 4.47x |
| HSS-SHA256-M32-H10+H10-W4 | verify | 23.7 ms | 7.64 ms | 3.11x |
| HSS-SHA256-M24-H10+H10-W4 | keygen | 36.57 s | 7.80 s | 4.69x |
| HSS-SHA256-M24-H10+H10-W4 | sign | 24.9 ms | 5.28 ms | 4.71x |
| HSS-SHA256-M24-H10+H10-W4 | verify | 19.5 ms | 5.78 ms | 3.37x |
| XMSS-SHA2_10_256 | keygen | 1.66 min | 23.93 s | 4.17x |
| XMSS-SHA2_10_256 | sign | 131.1 ms | 31.5 ms | 4.16x |
| XMSS-SHA2_10_256 | verify | 53.3 ms | 13.1 ms | 4.07x |
| XMSS-SHA2_10_192 | keygen | 1.58 min | 16.07 s | 5.91x |
| XMSS-SHA2_10_192 | sign | 123.8 ms | 21.2 ms | 5.85x |
| XMSS-SHA2_10_192 | verify | 52.1 ms | 8.86 ms | 5.88x |
| XMSSMT-SHA2_20/2_256 | keygen | 3.33 min | 47.87 s | 4.17x |
| XMSSMT-SHA2_20/2_256 | sign | 131.1 ms | 31.7 ms | 4.14x |
| XMSSMT-SHA2_20/2_256 | verify | 110.8 ms | 26.9 ms | 4.11x |
| XMSSMT-SHA2_20/4_256 | keygen | 12.61 s | 3.03 s | 4.17x |
| XMSSMT-SHA2_20/4_256 | sign | 94.6 ms | 23.1 ms | 4.09x |
| XMSSMT-SHA2_20/4_256 | verify | 207.6 ms | 52.8 ms | 3.93x |
| XMSSMT-SHA2_20/4_192 | keygen | 12.01 s | 2.03 s | 5.91x |
| XMSSMT-SHA2_20/4_192 | sign | 89.0 ms | 15.4 ms | 5.79x |
| XMSSMT-SHA2_20/4_192 | verify | 198.4 ms | 35.3 ms | 5.62x |
| XMSSMT-SHA2_40/8_256 | keygen | 25.27 s | 6.07 s | 4.17x |
| XMSSMT-SHA2_40/8_256 | sign | 95.0 ms | 23.0 ms | 4.13x |
| XMSSMT-SHA2_40/8_256 | verify | 422.9 ms | 101.3 ms | 4.18x |
| XMSSMT-SHA2_60/12_256 | keygen | 37.94 s | 9.10 s | 4.17x |
| XMSSMT-SHA2_60/12_256 | sign | 95.1 ms | 23.1 ms | 4.11x |
| XMSSMT-SHA2_60/12_256 | verify | 633.3 ms | 155.4 ms | 4.08x |
| SLH-DSA-SHA2-128f | keygen | 111.1 ms | 45.3 ms | 2.45x |
| SLH-DSA-SHA2-128f | sign | 2.61 s | 1.06 s | 2.46x |
| SLH-DSA-SHA2-128f | verify | 156.6 ms | 63.7 ms | 2.46x |
| SLH-DSA-SHA2-192f | keygen | 166.4 ms | 72.6 ms | 2.29x |
| SLH-DSA-SHA2-192f | sign | 4.92 s | 1.98 s | 2.49x |
| SLH-DSA-SHA2-192f | verify | 269.8 ms | 104.8 ms | 2.58x |
| SLH-DSA-SHA2-256f | keygen | 442.9 ms | 212.5 ms | 2.08x |
| SLH-DSA-SHA2-256f | sign | 10.20 s | 4.48 s | 2.28x |
| SLH-DSA-SHA2-256f | verify | 274.1 ms | 114.7 ms | 2.39x |
| SLH-DSA-SHA2-128s | keygen | 7.11 s | 2.90 s | 2.45x |
| SLH-DSA-SHA2-128s | sign | 54.43 s | 22.09 s | 2.46x |
| SLH-DSA-SHA2-128s | verify | 54.9 ms | 21.2 ms | 2.59x |
| SLH-DSA-SHA2-192s | keygen | 10.65 s | 4.65 s | 2.29x |
| SLH-DSA-SHA2-192s | sign | 1.94 min | 45.08 s | 2.58x |
| SLH-DSA-SHA2-192s | verify | 105.5 ms | 38.6 ms | 2.73x |
| SLH-DSA-SHA2-256s | keygen | 7.09 s | 3.40 s | 2.08x |
| SLH-DSA-SHA2-256s | sign | 1.84 min | 44.63 s | 2.48x |
| SLH-DSA-SHA2-256s | verify | 145.1 ms | 61.4 ms | 2.36x |

## Memory (one run per operation; stack and heap net of the empty task)

| Run | Scheme | Impl | Operation | Stack | Heap peak | of which internal | Persistent | of which internal | rc |
|---|---|---|---|--:|--:|--:|--:|--:|--:|
| run_B2 | baseline | - | empty-task | 444 | 64 | 64 | 0 | 0 | 0 |
| run_B2 | ML-KEM-512 | stack | keygen | 7708 | 0 | 0 | 0 | 0 | 0 |
| run_B2 | ML-KEM-512 | stack | encaps | 10432 | 0 | 0 | 0 | 0 | 0 |
| run_B2 | ML-KEM-512 | stack | decaps | 10832 | 0 | 0 | 0 | 0 | 0 |
| run_B2 | ML-KEM-768 | stack | keygen | 11788 | 0 | 0 | 0 | 0 | 0 |
| run_B2 | ML-KEM-768 | stack | encaps | 14972 | 0 | 0 | 0 | 0 | 0 |
| run_B2 | ML-KEM-768 | stack | decaps | 15964 | 0 | 0 | 0 | 0 | 0 |
| run_B2 | ML-KEM-1024 | stack | keygen | 17164 | 0 | 0 | 0 | 0 | 0 |
| run_B2 | ML-KEM-1024 | stack | encaps | 20860 | 0 | 0 | 0 | 0 | 0 |
| run_B2 | ML-KEM-1024 | stack | decaps | 22416 | 0 | 0 | 0 | 0 | 0 |
| run_B2 | ML-DSA-44 | fast-sign | keygen | 1868 | 27056 | 27056 | 0 | 0 | 0 |
| run_B2 | ML-DSA-44 | fast-sign | sign | 1916 | 44672 | 44672 | 0 | 0 | 0 |
| run_B2 | ML-DSA-44 | fast-sign | verify | 2380 | 24592 | 24592 | 0 | 0 | 0 |
| run_B2 | ML-DSA-65 | fast-sign | keygen | 1952 | 44464 | 44464 | 0 | 0 | 0 |
| run_B2 | ML-DSA-65 | fast-sign | sign | 1932 | 68256 | 68256 | 0 | 0 | 0 |
| run_B2 | ML-DSA-65 | fast-sign | verify | 2464 | 40016 | 40016 | 0 | 0 | 0 |
| run_A2_mem | ML-KEM-512 | heap | keygen | 1836 | 5952 | 5952 | 0 | 0 | 0 |
| run_A2_mem | ML-KEM-512 | heap | encaps | 1868 | 8592 | 8592 | 0 | 0 | 0 |
| run_A2_mem | ML-KEM-512 | heap | decaps | 1632 | 9360 | 9360 | 0 | 0 | 0 |
| run_A2_mem | ML-KEM-768 | heap | keygen | 1580 | 10304 | 10304 | 0 | 0 | 0 |
| run_A2_mem | ML-KEM-768 | heap | encaps | 1596 | 13520 | 13520 | 0 | 0 | 0 |
| run_A2_mem | ML-KEM-768 | heap | decaps | 1580 | 14544 | 14544 | 0 | 0 | 0 |
| run_A2_mem | ML-KEM-1024 | heap | keygen | 1596 | 15680 | 15680 | 0 | 0 | 0 |
| run_A2_mem | ML-KEM-1024 | heap | encaps | 1596 | 19344 | 19344 | 0 | 0 | 0 |
| run_A2_mem | ML-KEM-1024 | heap | decaps | 1564 | 20912 | 20912 | 0 | 0 | 0 |
| run_A2_mem | ML-DSA-44 | reduce-ram | keygen | 1692 | 11728 | 11728 | 0 | 0 | 0 |
| run_A2_mem | ML-DSA-44 | reduce-ram | sign | 1772 | 10016 | 10016 | 0 | 0 | 0 |
| run_A2_mem | ML-DSA-44 | reduce-ram | verify | 2204 | 9264 | 9264 | 0 | 0 | 0 |
| run_A2_mem | ML-DSA-65 | reduce-ram | keygen | 1692 | 14800 | 14800 | 0 | 0 | 0 |
| run_A2_mem | ML-DSA-65 | reduce-ram | sign | 1856 | 12096 | 12096 | 0 | 0 | 0 |
| run_A2_mem | ML-DSA-65 | reduce-ram | verify | 2204 | 10352 | 10352 | 0 | 0 | 0 |
| run_A2_mem | ML-DSA-87 | reduce-ram | keygen | 1692 | 18896 | 18896 | 0 | 0 | 0 |
| run_A2_mem | ML-DSA-87 | reduce-ram | sign | 1856 | 14400 | 14400 | 0 | 0 | 0 |
| run_A2_mem | ML-DSA-87 | reduce-ram | verify | 2204 | 12656 | 12656 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-128f | SW | keygen | 4128 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-128f | SW | sign | 4288 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-128f | SW | verify | 4192 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-128f | HW | keygen | 4060 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-128f | HW | sign | 4316 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-128f | HW | verify | 4172 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-192f | SW | keygen | 4572 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-192f | SW | sign | 4816 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-192f | SW | verify | 4764 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-192f | HW | keygen | 4124 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-192f | HW | sign | 4316 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-192f | HW | verify | 4112 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-256f | SW | keygen | 4588 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-256f | SW | sign | 4816 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-256f | SW | verify | 4784 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-256f | HW | keygen | 4032 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-256f | HW | sign | 4316 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-256f | HW | verify | 4108 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-128f | SW | keygen | 3936 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-128f | SW | sign | 4144 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-128f | SW | verify | 4048 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-192f | SW | keygen | 3936 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-192f | SW | sign | 4144 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-192f | SW | verify | 4048 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-256f | SW | keygen | 3984 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-256f | SW | sign | 4144 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-256f | SW | verify | 4048 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-128s | SW | keygen | 4128 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-128s | SW | sign | 4288 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-128s | SW | verify | 4176 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-192s | SW | keygen | 4640 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-192s | SW | sign | 4864 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-192s | SW | verify | 4700 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-256s | SW | keygen | 4640 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-256s | SW | sign | 4864 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHA2-256s | SW | verify | 4732 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-128s | SW | keygen | 3984 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-128s | SW | sign | 4144 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-128s | SW | verify | 4048 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-192s | SW | keygen | 3984 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-192s | SW | sign | 4144 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-192s | SW | verify | 4080 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-256s | SW | keygen | 3984 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-256s | SW | sign | 4144 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | SLH-DSA-SHAKE-256s | SW | verify | 4080 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | LMS-SHA256-M32-H5-W4 | HW | keygen | 1532 | 3416 | 3416 | 0 | 0 | 0 |
| run_A2_mem | LMS-SHA256-M32-H5-W4 | HW | key-in-ram | 0 | 0 | 0 | 3040 | 3040 | 0 |
| run_A2_mem | LMS-SHA256-M32-H5-W4 | HW | sign | 992 | 24 | 24 | 0 | 0 | 0 |
| run_A2_mem | LMS-SHA256-M32-H5-W4 | HW | verify | 1408 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | LMS-SHA256-M32-H10-W4 | HW | keygen | 1532 | 11736 | 11736 | 0 | 0 | 0 |
| run_A2_mem | LMS-SHA256-M32-H10-W4 | HW | key-in-ram | 0 | 0 | 0 | 9472 | 9472 | 0 |
| run_A2_mem | LMS-SHA256-M32-H10-W4 | HW | sign | 992 | 24 | 24 | 0 | 0 | 0 |
| run_A2_mem | LMS-SHA256-M32-H10-W4 | HW | verify | 1408 | 12 | 12 | 0 | 0 | 0 |
| run_A2_mem | LMS-SHA256-M32-H15-W4 | HW | keygen | 1584 | 52376 | 1668 | 0 | 0 | 0 |
| run_A2_mem | LMS-SHA256-M32-H15-W4 | HW | key-in-ram | 0 | 0 | 0 | 43840 | 1584 | 0 |
| run_A2_mem | LMS-SHA256-M32-H15-W4 | HW | sign | 992 | 24 | 24 | 0 | 0 | 0 |
| run_A2_mem | LMS-SHA256-M32-H15-W4 | HW | verify | 1408 | 12 | 12 | 0 | 0 | 0 |
| run_A2_mem | HSS-SHA256-M32-H10+H10-W4 | HW | keygen | 1756 | 26340 | 26340 | 0 | 0 | 0 |
| run_A2_mem | HSS-SHA256-M32-H10+H10-W4 | HW | key-in-ram | 0 | 0 | 0 | 21276 | 21276 | 0 |
| run_A2_mem | HSS-SHA256-M32-H10+H10-W4 | HW | sign | 992 | 36 | 36 | 0 | 0 | 0 |
| run_A2_mem | HSS-SHA256-M32-H10+H10-W4 | HW | verify | 1472 | 12 | 12 | 0 | 0 | 0 |
| run_A2_mem | XMSS-SHA2_10_256 | HW | keygen | 3500 | 13496 | 13496 | 0 | 0 | 0 |
| run_A2_mem | XMSS-SHA2_10_256 | HW | key-in-ram | 0 | 0 | 0 | 11372 | 11372 | 0 |
| run_A2_mem | XMSS-SHA2_10_256 | HW | sign | 1248 | 32 | 32 | 0 | 0 | 0 |
| run_A2_mem | XMSS-SHA2_10_256 | HW | verify | 3536 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | XMSSMT-SHA2_20/2_256 | HW | keygen | 3612 | 27856 | 27856 | 0 | 0 | 0 |
| run_A2_mem | XMSSMT-SHA2_20/2_256 | HW | key-in-ram | 0 | 0 | 0 | 23040 | 23040 | 0 |
| run_A2_mem | XMSSMT-SHA2_20/2_256 | HW | sign | 1168 | 32 | 32 | 0 | 0 | 0 |
| run_A2_mem | XMSSMT-SHA2_20/2_256 | HW | verify | 3564 | 8 | 8 | 0 | 0 | 0 |
| run_A2_mem | XMSSMT-SHA2_20/4_256 | HW | keygen | 3516 | 25936 | 25936 | 0 | 0 | 0 |
| run_A2_mem | XMSSMT-SHA2_20/4_256 | HW | key-in-ram | 0 | 0 | 0 | 23168 | 23168 | 0 |
| run_A2_mem | XMSSMT-SHA2_20/4_256 | HW | sign | 1340 | 32 | 32 | 0 | 0 | 0 |
| run_A2_mem | XMSSMT-SHA2_20/4_256 | HW | verify | 3580 | 0 | 0 | 0 | 0 | 0 |
| run_A2_mem | XMSSMT-SHA2_60/12_256 | HW | keygen | 3612 | 74200 | 74200 | 0 | 0 | 0 |
| run_A2_mem | XMSSMT-SHA2_60/12_256 | HW | key-in-ram | 0 | 0 | 0 | 71432 | 71432 | 0 |
| run_A2_mem | XMSSMT-SHA2_60/12_256 | HW | sign | 1168 | 32 | 32 | 0 | 0 | 0 |
| run_A2_mem | XMSSMT-SHA2_60/12_256 | HW | verify | 3644 | 0 | 0 | 0 | 0 | 0 |
| run_B3_dsamem | ML-DSA-87 | fast-sign | keygen | 1952 | 75184 | 75184 | 0 | 0 | 0 |
| run_B3_dsamem | ML-DSA-87 | fast-sign | sign | 2016 | 107424 | 107424 | 0 | 0 | 0 |
| run_B3_dsamem | ML-DSA-87 | fast-sign | verify | 2464 | 68944 | 68944 | 0 | 0 | 0 |

## State management (NVS storage, SHA hardware on)

| Scheme | Item | N | Mean | Median | Max |
|---|---|--:|--:|--:|--:|
| NVS | nvs-write-8B | 200 | 4.59 ms | 2.08 ms | 78.3 ms |
| LMS-SHA256-M32-H10-W4 | keygen-nvs | 1 | 5.63 s | 5.63 s | 5.63 s |
| LMS-SHA256-M32-H10-W4 | sign-nvs | 100 | 14.3 ms | 11.9 ms | 101.3 ms |
| LMS-SHA256-M32-H10-W4 | load-nvs | 1 | 12.9 ms | 12.9 ms | 12.9 ms |
| LMS-SHA256-M32-H10-W4 | first-sign-after-load | 1 | 190.3 ms | 190.3 ms | 190.3 ms |
| LMS-SHA256-M32-H10-W4 | second-sign-after-load | 1 | 17.0 ms | 17.0 ms | 17.0 ms |
| XMSSMT-SHA2_20/4_192 | keygen-nvs | 1 | 2.36 s | 2.36 s | 2.36 s |
| XMSSMT-SHA2_20/4_192 | sign-nvs | 100 | 59.8 ms | 26.3 ms | 862.2 ms |
| XMSSMT-SHA2_20/4_192 | load-nvs | 1 | 25.8 ms | 25.8 ms | 25.8 ms |
| XMSSMT-SHA2_20/4_192 | first-sign-after-load | 1 | 287.4 ms | 287.4 ms | 287.4 ms |
| XMSSMT-SHA2_20/4_192 | second-sign-after-load | 1 | 42.6 ms | 42.6 ms | 42.6 ms |
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
| LMS-SHA256-M32-H5-W4 | cache_ram_bytes | | 1024 | | |
| LMS-SHA256-M32-H10-W4 | cache_ram_bytes | | 5120 | | |
| LMS-SHA256-M32-H15-W4 | cache_ram_bytes | | 32768 | | |
| HSS-SHA256-M32-H10+H10-W4 | cache_ram_bytes | | 9216 | | |
| XMSS-SHA2_10_256 | cache_ram_bytes | | 5120 | | |
| XMSSMT-SHA2_20/2_256 | cache_ram_bytes | | 9216 | | |
| XMSSMT-SHA2_20/4_256 | cache_ram_bytes | | 3328 | | |
| XMSSMT-SHA2_60/12_256 | cache_ram_bytes | | 9472 | | |
