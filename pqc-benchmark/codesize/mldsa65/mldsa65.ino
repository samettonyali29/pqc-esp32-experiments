// Copyright (c) 2026 The pqc-esp32-experiments authors
// SPDX-License-Identifier: Apache-2.0
// code-size probe (compile only): see ../codesize.sh
#include <Arduino.h>
#include <MLDSA.h>

void setup() {
  Serial.begin(115200);
  uint8_t *pk = (uint8_t *)malloc(MLDSA65::PUBLIC_KEY_SIZE), *sk = (uint8_t *)malloc(MLDSA65::SECRET_KEY_SIZE);
  uint8_t *sig = (uint8_t *)malloc(MLDSA65::SIGNATURE_SIZE), m[32] = {1}; size_t sl;
  int r = MLDSA65::generateKeypair(pk, sk) | MLDSA65::sign(sig, &sl, m, 32, sk) | MLDSA65::verify(sig, sl, m, 32, pk);
  Serial.println(r);
}

void loop() { delay(1000); }
