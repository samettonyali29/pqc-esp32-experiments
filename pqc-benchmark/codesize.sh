#!/bin/bash
# Copyright (c) 2026 The pqc-esp32-experiments authors
# SPDX-License-Identifier: Apache-2.0
# Flash and static RAM of each library: compiles the probes in codesize/ and
# prints "probe,config,flash_bytes,static_ram_bytes" (subtract the baseline).
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"; ROOT="${LIBS_DIR:-$HERE/../..}"  # the five libraries (see ../README.md)
BUILD="${BUILD:-/tmp/pqc-codesize}"
L="--library $ROOT/fips203-mlkem-esp32 --library $ROOT/fips204-mldsa-esp32 --library $ROOT/fips205-slhdsa-esp32 --library $ROOT/sp800-208-lms-esp32 --library $ROOT/sp800-208-xmss-esp32"
FQBN="${FQBN:-esp32:esp32:m5stack_stickc_plus2}"
run() { # probe config flags
  local b="$BUILD/$1_$2"; rm -rf "$b"
  local out; out=$(arduino-cli compile -b "$FQBN" $L --build-path "$b" \
      --build-property "compiler.c.extra_flags=$3" --build-property "compiler.cpp.extra_flags=$3" "$HERE/codesize/$1" 2>&1) \
    || { echo "$1,$2,FAILED"; echo "$out" | grep -m3 error >&2; return; }
  local flash ram
  flash=$(echo "$out" | sed -n 's/Sketch uses \([0-9]*\) bytes.*/\1/p')
  ram=$(echo "$out" | sed -n 's/Global variables use \([0-9]*\) bytes.*/\1/p')
  echo "$1,$2,$flash,$ram"; rm -rf "$b"
}
run baseline default ""
for p in mlkem768 mlkem_all mldsa65 mldsa_all slhdsa128f slhdsa_all lms_ram xmss_ram lms xmss; do run $p default ""; done
run mlkem_all stack "-DMLKEM_ESP32_USE_STACK"
run mldsa_all fast "-DMLDSA_ESP32_NO_REDUCE_RAM"
run slhdsa128f hwsha "-DSLHDSA_ESP32_HW_SHA"
run slhdsa_all hwsha "-DSLHDSA_ESP32_HW_SHA"
run lms_ram hwsha "-DLMS_ESP32_HW_SHA"
run xmss_ram hwsha "-DXMSS_ESP32_HW_SHA"
