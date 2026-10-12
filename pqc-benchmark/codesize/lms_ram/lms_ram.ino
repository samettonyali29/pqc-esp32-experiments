// Copyright (c) 2026 The pqc-esp32-experiments authors
// SPDX-License-Identifier: Apache-2.0
// code-size probe (compile only): see ../codesize.sh
#include <Arduino.h>
#include <LMS.h>


void setup() {
  Serial.begin(115200);
  LMSRamStorage st; LMSKey k; HSSKey h;
  uint8_t pk[64], *sig = (uint8_t *)malloc(9000), m[32] = {1}; size_t pl = 64, sl = 9000;
  int r = k.generate(st, LMS_SHA256_M32_H10, LMOTS_SHA256_N32_W4) | k.publicKey(pk, &pl) | k.sign(m, 32, sig, &sl) | LMSKey::verify(pk, pl, m, 32, sig, sl);
  const LMSLevel lv[2] = {{LMS_SHA256_M32_H5, LMOTS_SHA256_N32_W4}, {LMS_SHA256_M32_H5, LMOTS_SHA256_N32_W4}};
  r |= h.generate(st, lv) | h.sign(m, 32, sig, &sl) | HSSKey::verify(pk, pl, m, 32, sig, sl) | h.load(st);
  Serial.println(r);
}

void loop() { delay(1000); }
