#!/usr/bin/env python3
# Copyright (c) 2026 The pqc-esp32-experiments authors
# SPDX-License-Identifier: Apache-2.0
"""
Object sizes (bytes) of every benchmarked parameter set: public key, secret
key or stored private state, ciphertext / signature, shared secret.

For LMS/HSS and XMSS/XMSS^MT the "secret key" is the state the libraries keep
in storage: the records hdr, ctr, lv<i> (seeds, root, stored signature of
lower trees) and the optional tree caches ax<i> (layouts: lms_key.c,
xmss_key.c), and the tree caches in RAM.

Usage: sizes.py OUTDIR > OUTDIR/sizes.md   (also writes OUTDIR/sizes.csv)
"""
import csv
import os
import sys

rows = []


def add(family, name, pk, sk, ct_or_sig, ss="", state_ram="", note=""):
    rows.append(dict(family=family, name=name, pk=pk, sk=sk, out=ct_or_sig, ss=ss,
                     ram_cache=state_ram, note=note))


for n, pk, sk, ct in [(512, 800, 1632, 768), (768, 1184, 2400, 1088), (1024, 1568, 3168, 1568)]:
    add("ML-KEM", f"ML-KEM-{n}", pk, sk, ct, 32)
for n, pk, sk, sig in [(44, 1312, 2560, 2420), (65, 1952, 4032, 3309), (87, 2592, 4896, 4627)]:
    add("ML-DSA", f"ML-DSA-{n}", pk, sk, sig)
for hashname in ["SHA2", "SHAKE"]:
    for lvl, n, s_sig, f_sig in [(128, 16, 7856, 17088), (192, 24, 16224, 35664),
                                 (256, 32, 29792, 49856)]:
        add("SLH-DSA", f"SLH-DSA-{hashname}-{lvl}s", 2 * n, 4 * n, s_sig)
        add("SLH-DSA", f"SLH-DSA-{hashname}-{lvl}f", 2 * n, 4 * n, f_sig)

# ---- LMS / HSS --------------------------------------------------------------------
P = {(32, 1): 265, (32, 2): 133, (32, 4): 67, (32, 8): 34,
     (24, 1): 200, (24, 2): 101, (24, 4): 51, (24, 8): 26}


def lms_sig(n, h, w):
    return 4 + (4 + n + P[(n, w)] * n) + 4 + h * n


def cache_nodes(h, bottom):
    k = (h + 1) // 2
    nodes = 2 ** (h - k + 1) + 2 ** (k + 1)
    return nodes + (2 ** k if bottom else 0), 2 ** (h - k + 1) - 1


def lms_state(levels, n):
    """levels: list of (h, w); returns (stored bytes incl. caches, RAM cache bytes)"""
    L = len(levels)
    stored = (8 + 8 * L) + 8  # hdr, ctr
    ram = 0
    for i, (h, w) in enumerate(levels):
        parent_sig = lms_sig(n, *levels[i - 1]) if i > 0 else 0
        stored += 4 + 8 + 16 + 2 * n + 4 + parent_sig       # lv<i>
        ram_nodes, ax_nodes = cache_nodes(h, i == L - 1)
        stored += 8 + 16 + ax_nodes * n                      # ax<i>
        ram += ram_nodes * n
    return stored, ram


LMS_SETS = [("LMS-SHA256-M32-H5-W1", 32, [(5, 1)]), ("LMS-SHA256-M32-H5-W2", 32, [(5, 2)]),
            ("LMS-SHA256-M32-H5-W4", 32, [(5, 4)]), ("LMS-SHA256-M32-H5-W8", 32, [(5, 8)]),
            ("LMS-SHA256-M32-H10-W2", 32, [(10, 2)]), ("LMS-SHA256-M32-H10-W4", 32, [(10, 4)]),
            ("LMS-SHA256-M32-H10-W8", 32, [(10, 8)]), ("LMS-SHA256-M24-H10-W4", 24, [(10, 4)]),
            ("LMS-SHA256-M24-H10-W8", 24, [(10, 8)]), ("LMS-SHAKE-M32-H10-W4", 32, [(10, 4)]),
            ("LMS-SHAKE-M24-H10-W4", 24, [(10, 4)]), ("LMS-SHA256-M32-H15-W4", 32, [(15, 4)]),
            ("HSS-SHA256-M32-H10+H10-W4", 32, [(10, 4), (10, 4)]),
            ("HSS-SHA256-M24-H10+H10-W4", 24, [(10, 4), (10, 4)])]
for name, n, levels in LMS_SETS:
    hss = len(levels) > 1
    pk = (4 if hss else 0) + 24 + n
    sig = (4 if hss else 0) + sum(lms_sig(n, h, w) for h, w in levels) + (len(levels) - 1) * (24 + n)
    stored, ram = lms_state(levels, n)
    total = 2 ** sum(h for h, _ in levels)
    add("LMS/HSS", name, pk, stored, sig, "", ram, f"{total} signatures")

# ---- XMSS / XMSS^MT ------------------------------------------------------------------
XMSS_SETS = [("XMSS-SHA2_10_256", 32, 10, 1), ("XMSS-SHA2_10_192", 24, 10, 1),
             ("XMSS-SHAKE256_10_256", 32, 10, 1), ("XMSS-SHAKE256_10_192", 24, 10, 1),
             ("XMSSMT-SHA2_20/2_256", 32, 20, 2), ("XMSSMT-SHA2_20/4_256", 32, 20, 4),
             ("XMSSMT-SHA2_20/4_192", 24, 20, 4), ("XMSSMT-SHA2_40/8_256", 32, 40, 8),
             ("XMSSMT-SHA2_60/12_256", 32, 60, 12), ("XMSSMT-SHAKE256_20/4_256", 32, 20, 4),
             ("XMSSMT-SHAKE256_20/4_192", 24, 20, 4)]
for name, n, h, d in XMSS_SETS:
    ln = 2 * n + 3
    th = h // d
    idx = 4 if d == 1 else (h + 7) // 8
    tree_sig = (ln + th) * n
    sig = idx + n + d * tree_sig
    stored = (12 + n) + 8  # hdr (incl. SEED), ctr
    ram = 0
    for i in range(d):
        stored += 4 + 8 + 3 * n + 4 + (tree_sig if i > 0 else 0)
        ram_nodes, ax_nodes = cache_nodes(th, i == d - 1)
        stored += 8 + ax_nodes * n
        ram += ram_nodes * n
    add("XMSS", name, 4 + 2 * n, stored, sig, "", ram, f"2^{h} signatures")

if len(sys.argv) < 2:
    sys.exit(__doc__)
out = os.path.join(sys.argv[1], "sizes.csv")
with open(out, "w", newline="") as f:
    w = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
    w.writeheader()
    w.writerows(rows)

print("| Family | Parameter set | Public key | Secret key / stored state | Ciphertext / signature "
      "| Shared secret | Tree caches in RAM | Note |")
print("|---|---|--:|--:|--:|--:|--:|---|")
for r in rows:
    print(f"| {r['family']} | {r['name']} | {r['pk']} | {r['sk']} | {r['out']} | {r['ss']} | "
          f"{r['ram_cache']} | {r['note']} |")
