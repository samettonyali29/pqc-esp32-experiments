// Copyright (c) 2026 The pqc-esp32-experiments authors
// SPDX-License-Identifier: Apache-2.0
// code-size probe (compile only): see ../codesize.sh
#include <Arduino.h>
#include <MLKEM.h>

void setup() {
  Serial.begin(115200);
  uint8_t *pk = (uint8_t *)malloc(1568), *sk = (uint8_t *)malloc(3168), *ct = (uint8_t *)malloc(1568), ss[32];
  int r = 0;
  r |= MLKEM512::generateKeypair(pk, sk) | MLKEM512::encapsulate(ct, ss, pk) | MLKEM512::decapsulate(ss, ct, sk);
  r |= MLKEM768::generateKeypair(pk, sk) | MLKEM768::encapsulate(ct, ss, pk) | MLKEM768::decapsulate(ss, ct, sk);
  r |= MLKEM1024::generateKeypair(pk, sk) | MLKEM1024::encapsulate(ct, ss, pk) | MLKEM1024::decapsulate(ss, ct, sk);
  Serial.println(r + ss[0]);
}

void loop() { delay(1000); }
