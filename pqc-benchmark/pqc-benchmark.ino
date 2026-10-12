/*
 * pqc-benchmark - uniform benchmark of the five PQC libraries on the ESP32
 * (headless experiment firmware; the same benchmark with a live screen is
 * ../pqc-benchmark-gui)
 *
 * Driven over the serial port (115200 baud) by run_bench.py; one character
 * starts a group (see PQCBench.h):
 *   k ML-KEM   d ML-DSA   s SLH-DSA   l LMS/HSS   x XMSS/XMSS^MT
 *   n state management (NVS)   c radio control subset
 *   m memory (all)   q memory ML-KEM + ML-DSA   r memory ML-DSA
 *   w radio on   o radio off   e environment line
 *
 * Conditions: the display is never initialised (backlight off, no SPI
 * traffic); the benchmark task runs on core 1 at 240 MHz, Wi-Fi (if on) on
 * core 0. With the radio on, esp_fill_random() returns true random numbers
 * (ESP-IDF: RF subsystem enabled). The station is started but never
 * connected, so there is no radio traffic.
 *
 * Output: the CSV lines described in PQCBench.cpp (R, M, S, E, DONE).
 *
 * Libraries: fips203-mlkem-esp32, fips204-mldsa-esp32, fips205-slhdsa-esp32, sp800-208-lms-esp32, sp800-208-xmss-esp32
 * and ../libraries/PQCBench. Build with -DLMS_ESP32_HW_SHA
 * -DXMSS_ESP32_HW_SHA (the software rows switch the accelerator off at run
 * time); optional build with -DMLDSA_ESP32_NO_REDUCE_RAM
 * -DMLKEM_ESP32_USE_STACK for the alternative memory modes.
 *
 * Copyright (c) 2026 The pqc-esp32-experiments authors
 * SPDX-License-Identifier: Apache-2.0
 */

#include <PQCBench.h>

class SerialOut : public pqcbench::Observer {
public:
  void line(const char *text) override { Serial.printf("%s\n", text); }
};

static SerialOut out;
static QueueHandle_t cmdQueue;

static void benchTask(void *) {
  for (;;) {
    char cmd;
    if (xQueueReceive(cmdQueue, &cmd, portMAX_DELAY) == pdTRUE) pqcbench::run(cmd);
  }
}

void setup() {
  pinMode(4, OUTPUT);  // M5StickC PLUS2: HOLD pin keeps the board powered on battery
  digitalWrite(4, HIGH);
  Serial.begin(115200);
  delay(500);
  pqcbench::begin(&out);  // SHA accelerator self-tests
  cmdQueue = xQueueCreate(8, 1);
  xTaskCreatePinnedToCore(benchTask, "bench", 32 * 1024, NULL, 1, NULL, 1);
  Serial.println("\nREADY pqc-benchmark");
  pqcbench::printEnv();
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c > ' ') xQueueSend(cmdQueue, &c, 0);
  }
  delay(50);
}
