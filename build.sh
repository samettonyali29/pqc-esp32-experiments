#!/bin/bash
# Copyright (c) 2026 The pqc-esp32-experiments authors
# SPDX-License-Identifier: Apache-2.0
# Compiles (and optionally uploads) one experiment firmware.
#
# Usage: bash build.sh SKETCH [PORT] [EXTRA_FLAGS]
#   bash build.sh pqc-benchmark                      compile only
#   bash build.sh pqc-benchmark /dev/ttyACM0         compile and upload
#   bash build.sh pqc-benchmark /dev/ttyACM0 "-DMLDSA_ESP32_NO_REDUCE_RAM -DMLKEM_ESP32_USE_STACK"
#
# The five PQC libraries are expected next to this repository (see README):
#   <parent>/fips203-mlkem-esp32  fips204-mldsa-esp32  fips205-slhdsa-esp32
#            sp800-208-lms-esp32  sp800-208-xmss-esp32  pqc-esp32-experiments
# or in the directory given by LIBS_DIR. The GUI firmwares also need the
# M5Unified library (Arduino Library Manager).
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
LIBS_DIR="${LIBS_DIR:-$HERE/..}"
FQBN="${FQBN:-esp32:esp32:m5stack_stickc_plus2}"
SKETCH="${1:?usage: bash build.sh SKETCH [PORT] [EXTRA_FLAGS]}"
SKETCH="${SKETCH%/}"
PORT="$2"
FLAGS="-DLMS_ESP32_HW_SHA -DXMSS_ESP32_HW_SHA $3"
BUILD="${BUILD:-$HERE/build/$(basename "$SKETCH")}"

L=""
for lib in fips203-mlkem-esp32 fips204-mldsa-esp32 fips205-slhdsa-esp32 sp800-208-lms-esp32 sp800-208-xmss-esp32; do
  if [ ! -f "$LIBS_DIR/$lib/library.properties" ]; then
    echo "error: $LIBS_DIR/$lib not found (clone it next to this repository or set LIBS_DIR)" >&2
    exit 1
  fi
  L="$L --library $LIBS_DIR/$lib"
done
L="$L --library $HERE/libraries/PQCBench --library $HERE/libraries/PQCGui"

arduino-cli compile -b "$FQBN" $L --build-path "$BUILD" \
  --build-property "compiler.c.extra_flags=$FLAGS" \
  --build-property "compiler.cpp.extra_flags=$FLAGS" "$HERE/$SKETCH"
if [ -n "$PORT" ]; then
  arduino-cli upload -b "$FQBN" -p "$PORT" --input-dir "$BUILD" "$HERE/$SKETCH"
fi
