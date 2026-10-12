// Copyright (c) 2026 The pqc-esp32-experiments authors
// SPDX-License-Identifier: Apache-2.0
// code-size probe (compile only): see ../codesize.sh
#include <Arduino.h>
#include <SLHDSA.h>

void setup() {
  Serial.begin(115200);
  typedef SLHDSA_SHA2_128f D;
  uint8_t pk[D::PUBLIC_KEY_SIZE], sk[D::SECRET_KEY_SIZE], *sig = (uint8_t *)malloc(D::SIGNATURE_SIZE), m[32] = {1}; size_t sl;
  int r = D::generateKeypair(pk, sk) | D::sign(sig, &sl, m, 32, sk) | D::verify(sig, sl, m, 32, pk);
  Serial.println(r);
}

void loop() { delay(1000); }
