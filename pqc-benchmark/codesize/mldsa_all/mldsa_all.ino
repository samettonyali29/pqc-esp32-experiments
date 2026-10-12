// Copyright (c) 2026 The pqc-esp32-experiments authors
// SPDX-License-Identifier: Apache-2.0
// code-size probe (compile only): see ../codesize.sh
#include <Arduino.h>
#include <MLDSA.h>

void setup() {
  Serial.begin(115200);
  uint8_t *pk = (uint8_t *)malloc(2592), *sk = (uint8_t *)malloc(4896), *sig = (uint8_t *)malloc(4627), m[32] = {1}; size_t sl; int r = 0;
  r |= MLDSA44::generateKeypair(pk, sk) | MLDSA44::sign(sig, &sl, m, 32, sk) | MLDSA44::verify(sig, sl, m, 32, pk);
  r |= MLDSA65::generateKeypair(pk, sk) | MLDSA65::sign(sig, &sl, m, 32, sk) | MLDSA65::verify(sig, sl, m, 32, pk);
  r |= MLDSA87::generateKeypair(pk, sk) | MLDSA87::sign(sig, &sl, m, 32, sk) | MLDSA87::verify(sig, sl, m, 32, pk);
  Serial.println(r);
}

void loop() { delay(1000); }
