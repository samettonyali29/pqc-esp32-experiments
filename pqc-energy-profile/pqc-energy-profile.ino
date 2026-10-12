/*
 * pqc-energy-profile - steady-state power phases for board-level energy
 * measurement with an inline USB power meter (e.g. FNIRSI FNB-C2)
 * (headless experiment firmware; the same phases with a live screen are
 * ../pqc-energy-gui)
 *
 * Method: every phase runs one workload in a loop for at least PHASE_S
 * seconds and until every operation has run at least 10 times (a phase can
 * therefore take longer than PHASE_S); the power read from the meter during
 * the phase times the measured time per operation gives the energy per
 * operation:
 *   E_total = P_phase * t_op,  E_incremental = (P_phase - P_idle) * t_op
 *
 * The display is never initialised (backlight off, no SPI traffic), so it
 * adds nothing to the measured power. Procedure: run_energy.py drives the
 * phases over the serial port (115200 baud) and asks for the meter reading
 * after each phase:
 *   g  prepare and measure the next phase
 *   0  restart at the first phase
 *
 * Output lines: PHASE,k,name,ready / PHASE,k,name,measuring / T,... (every
 * run) / RESULT,... (mean and standard deviation; see PQCEnergy.cpp),
 * PROFILE,complete after the last phase.
 *
 * Conditions: battery fully charged (no charging current), Wi-Fi/BT off,
 * CPU 240 MHz, workload on core 1, core 0 idle.
 *
 * Libraries: fips203-mlkem-esp32, fips204-mldsa-esp32, fips205-slhdsa-esp32, sp800-208-lms-esp32,
 * sp800-208-xmss-esp32, ../libraries/PQCBench. Build with -DLMS_ESP32_HW_SHA
 * -DXMSS_ESP32_HW_SHA (the software phases switch the hardware off at run
 * time).
 *
 * Copyright (c) 2026 The pqc-esp32-experiments authors
 * SPDX-License-Identifier: Apache-2.0
 */

#include <PQCBench.h>

#ifndef PHASE_S
#define PHASE_S 45  // minimum time per phase (s)
#endif

class SerialOut : public pqcenergy::Observer {
public:
  void line(const char *text) override { Serial.printf("%s\n", text); }
};

static SerialOut out;
static QueueHandle_t cmdQueue;

static void energyTask(void *) {
  unsigned k = 0;
  for (;;) {
    char cmd;
    if (xQueueReceive(cmdQueue, &cmd, portMAX_DELAY) != pdTRUE) continue;
    if (cmd == '0') {
      k = 0;
      Serial.println("PROFILE,restart");
    } else if (cmd == 'g' && k < pqcenergy::count()) {
      pqcenergy::prepare(k);
      pqcenergy::measure(k, PHASE_S);
      if (++k == pqcenergy::count()) Serial.println("PROFILE,complete");
    }
  }
}

void setup() {
  pinMode(4, OUTPUT);  // M5StickC PLUS2: HOLD pin keeps the board powered on battery
  digitalWrite(4, HIGH);
  Serial.begin(115200);
  delay(500);
  pqcenergy::begin(&out);  // SHA accelerator self-tests
  cmdQueue = xQueueCreate(8, 1);
  xTaskCreatePinnedToCore(energyTask, "energy", 24 * 1024, NULL, 1, NULL, 1);
  Serial.printf("\nREADY pqc-energy-profile,phases=%u,phase_s=%d,min_runs=10,cpu_mhz=%u\n",
                pqcenergy::count(), PHASE_S, (unsigned)getCpuFrequencyMhz());
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c > ' ') xQueueSend(cmdQueue, &c, 0);
  }
  delay(50);
}
