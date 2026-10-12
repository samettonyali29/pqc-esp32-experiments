// Copyright (c) 2026 The pqc-esp32-experiments authors
// SPDX-License-Identifier: Apache-2.0
// code-size probe (compile only): see ../codesize.sh
#include <Arduino.h>
#include <XMSS.h>


void setup() {
  Serial.begin(115200);
  XMSSRamStorage st; XMSSKey k; XMSSMTKey t;
  uint8_t pk[68], *sig = (uint8_t *)malloc(28000), m[32] = {1}; size_t pl = 68, sl = 28000;
  int r = k.generate(st, XMSS_SHA2_10_256) | k.publicKey(pk, &pl) | k.sign(m, 32, sig, &sl) | XMSSKey::verify(pk, pl, m, 32, sig, sl);
  r |= t.generate(st, XMSSMT_SHA2_20_4_256) | t.sign(m, 32, sig, &sl) | XMSSMTKey::verify(pk, pl, m, 32, sig, sl) | t.load(st);
  Serial.println(r);
}

void loop() { delay(1000); }
