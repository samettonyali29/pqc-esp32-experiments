// Copyright (c) 2026 The pqc-esp32-experiments authors
// SPDX-License-Identifier: Apache-2.0
// code-size probe (compile only): see ../codesize.sh
#include <Arduino.h>
#include <SLHDSA.h>

void setup() {
  Serial.begin(115200);
  uint8_t pk[64], sk[128], *sig = (uint8_t *)malloc(49856), m[32] = {1}; size_t sl; int r = 0;
#define T(D) r |= D::generateKeypair(pk, sk) | D::sign(sig, &sl, m, 32, sk) | D::verify(sig, sl, m, 32, pk);
  T(SLHDSA_SHA2_128s) T(SLHDSA_SHA2_128f) T(SLHDSA_SHA2_192s) T(SLHDSA_SHA2_192f) T(SLHDSA_SHA2_256s) T(SLHDSA_SHA2_256f)
  T(SLHDSA_SHAKE_128s) T(SLHDSA_SHAKE_128f) T(SLHDSA_SHAKE_192s) T(SLHDSA_SHAKE_192f) T(SLHDSA_SHAKE_256s) T(SLHDSA_SHAKE_256f)
  Serial.println(r);
}

void loop() { delay(1000); }
