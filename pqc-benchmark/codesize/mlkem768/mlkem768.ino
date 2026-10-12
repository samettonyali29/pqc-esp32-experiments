// Copyright (c) 2026 The pqc-esp32-experiments authors
// SPDX-License-Identifier: Apache-2.0
// code-size probe (compile only): see ../codesize.sh
#include <Arduino.h>
#include <MLKEM.h>

void setup() {
  Serial.begin(115200);
  uint8_t *pk = (uint8_t *)malloc(MLKEM768::PUBLIC_KEY_SIZE), *sk = (uint8_t *)malloc(MLKEM768::SECRET_KEY_SIZE);
  uint8_t *ct = (uint8_t *)malloc(MLKEM768::CIPHERTEXT_SIZE), ss1[32], ss2[32];
  int r = MLKEM768::generateKeypair(pk, sk) | MLKEM768::encapsulate(ct, ss1, pk) | MLKEM768::decapsulate(ss2, ct, sk);
  Serial.println(r + ss1[0] + ss2[0]);
}

void loop() { delay(1000); }
