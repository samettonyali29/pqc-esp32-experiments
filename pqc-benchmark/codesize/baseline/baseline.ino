// Copyright (c) 2026 The pqc-esp32-experiments authors
// SPDX-License-Identifier: Apache-2.0
// code-size probe (compile only): see ../codesize.sh
#include <Arduino.h>


void setup() {
  Serial.begin(115200);
  uint8_t *b = (uint8_t *)malloc(64); esp_fill_random(b, 64); Serial.println(b[0]); free(b);
}

void loop() { delay(1000); }
