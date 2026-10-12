/*
 * pqc-energy-gui - the steady-state power phases of pqc-energy-profile with
 * a live screen on the M5StickC PLUS2 (same layout as the library demos)
 *
 * The workloads are those of the headless experiment firmware
 * ../pqc-energy-profile (both use ../libraries/PQCBench), and the serial
 * port prints the same PHASE / RESULT lines. Here the display stays on and
 * adds its own power to every phase, so energy figures come from the
 * headless firmware; this one is for demonstrations.
 *
 *   Button B (right side):   select the first phase
 *   Button A (front, "M5"):  run the phases from the selected one to the end
 * Serial: 'g' works like button A, '#' prints a screenshot (pqcgui::dump).
 *
 * Libraries: fips203-mlkem-esp32, fips204-mldsa-esp32, fips205-slhdsa-esp32, sp800-208-lms-esp32, sp800-208-xmss-esp32,
 * ../libraries/PQCBench, ../libraries/PQCGui, M5Unified. Build with
 * -DLMS_ESP32_HW_SHA -DXMSS_ESP32_HW_SHA.
 * Board: "M5StickCPlus2" (esp32:esp32:m5stack_stickc_plus2).
 *
 * Copyright (c) 2026 The pqc-esp32-experiments authors
 * SPDX-License-Identifier: Apache-2.0
 */

#include <M5Unified.h>
#include <PQCBench.h>
#include <PQCGui.h>

using namespace pqcgui;

#ifndef PHASE_S
#define PHASE_S 45  // minimum time per phase (s); also >= 10 runs per operation
#endif

// Header of each phase (same order as the phases in PQCEnergy.cpp)
struct PhaseHeader {
  const char *alg, *variant;
  bool hw;
};
static const PhaseHeader HEADERS[] = {
    {"Energy", "Idle", false},           {"Energy", "Busy loop", false},
    {"ML-KEM", "768", false},            {"ML-DSA", "65", false},
    {"SLH-DSA", "SHA2-128f", false},     {"SLH-DSA", "SHA2-128f", true},
    {"SLH-DSA", "SHAKE-128f", false},    {"HSS", "H10+H10 W4", false},
    {"HSS", "H10+H10 W4", true},         {"XMSS^MT", "20/4_256", false},
    {"XMSS^MT", "20/4_256", true},       {"NVS", "write 8 B", false},
    {"Energy", "Idle again", false},
};
static const unsigned NHEADERS = sizeof HEADERS / sizeof HEADERS[0];

static const char *opLabel(const char *op) {
  static const char *const MAP[][2] = {
      {"keygen", "KeyGen"}, {"encaps", "Encaps"},          {"decaps", "Decaps"},
      {"sign", "Sign"},     {"verify", "Verify"},          {"nvs-write-8B", "Write 8 B"},
      {"idle-100ms", "Idle 100 ms"}, {"busy-loop-1M", "Busy 1M"},
  };
  for (auto &m : MAP)
    if (strcmp(op, m[0]) == 0) return m[1];
  return op;
}

// ---- state shared between the workload task (core 1) and the UI (core 0) ---
struct Shared {
  unsigned sel;    // selected first phase
  int phase;       // current phase, -1 idle
  bool measuring;  // false: preparing
  uint32_t measureStartMs;
  int nops;
  char label[3][16];
  uint32_t cnt[3];
  double lastUs[3], sumUs[3];
  char status[32];
  uint16_t statusColor;
};
static Shared st;
static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
static volatile bool startRequested = false;

static void setStatus(const char *s, uint16_t color) {
  portENTER_CRITICAL(&mux);
  strlcpy(st.status, s, sizeof st.status);
  st.statusColor = color;
  portEXIT_CRITICAL(&mux);
}

class GuiObserver : public pqcenergy::Observer {
public:
  void line(const char *text) override {
    Serial.printf("%s\n", text);
    if (strncmp(text, "PHASE,", 6) == 0 && strstr(text, ",measuring")) {
      portENTER_CRITICAL(&mux);
      st.measuring = true;
      st.measureStartMs = millis();
      portEXIT_CRITICAL(&mux);
      setStatus("measuring", COL_WARN);
    }
  }
  void phaseStart(unsigned k, unsigned n, const char *name, int nops,
                  const char *const ops[3]) override {
    portENTER_CRITICAL(&mux);
    st.phase = (int)k;
    st.measuring = false;
    st.nops = nops;
    for (int i = 0; i < 3; i++) {
      strlcpy(st.label[i], i < nops && ops[i] ? opLabel(ops[i]) : "", sizeof st.label[i]);
      st.cnt[i] = 0;
      st.lastUs[i] = st.sumUs[i] = 0;
    }
    portEXIT_CRITICAL(&mux);
  }
  void opDone(int op, double us) override {
    portENTER_CRITICAL(&mux);
    st.cnt[op]++;
    st.lastUs[op] = us;
    st.sumUs[op] += us;
    portEXIT_CRITICAL(&mux);
  }
};
static GuiObserver gui;

// ---- workload task (core 1, as in the headless firmware) --------------------
static void energyTask(void *) {
  for (;;) {
    if (!startRequested) {
      vTaskDelay(pdMS_TO_TICKS(50));
      continue;
    }
    for (unsigned k = st.sel; k < pqcenergy::count(); k++) {
      portENTER_CRITICAL(&mux);
      st.phase = (int)k;
      st.measuring = false;
      st.nops = 0;
      portEXIT_CRITICAL(&mux);
      setStatus("preparing", COL_LABEL);
      pqcenergy::prepare(k);
      pqcenergy::measure(k, PHASE_S);
      setStatus("phase done", COL_OK);
      vTaskDelay(pdMS_TO_TICKS(1500));  // show the phase result
    }
    Serial.println("PROFILE,complete");
    portENTER_CRITICAL(&mux);
    st.phase = -1;
    portEXIT_CRITICAL(&mux);
    setStatus("all phases done", COL_OK);
    startRequested = false;
  }
}

// ---- UI (core 0) -----------------------------------------------------------
static M5Canvas canvas(&M5.Display);  // off-screen buffer: flicker-free

static void buildScreen(Screen &scr) {
  Shared s;
  portENTER_CRITICAL(&mux);
  s = st;
  portEXIT_CRITICAL(&mux);
  clear(scr);
  const unsigned k = s.phase >= 0 ? (unsigned)s.phase : s.sel;
  const PhaseHeader &h = HEADERS[k < NHEADERS ? k : 0];
  strlcpy(scr.alg, h.alg, sizeof scr.alg);
  strlcpy(scr.variant, h.variant, sizeof scr.variant);
  scr.hw = h.hw;

  uint32_t total = 0;
  if (s.phase < 0) {  // idle: what button A will start
    char buf[40];
    snprintf(buf, sizeof buf, "%u..%u of %u", k + 1, pqcenergy::count(), pqcenergy::count());
    setCard(scr, 0, "Phases", buf);
    snprintf(buf, sizeof buf, ">= %d s, >= 10 runs", PHASE_S);
    setCard(scr, 1, "Duration", buf);
    setCard(scr, 2, "Results", "RESULT lines on serial");
  } else {
    for (int i = 0; i < 3; i++) {
      if (i >= s.nops) {
        setCard(scr, i, "", "");
        continue;
      }
      total += s.cnt[i];
      char buf[40], last[16], avg[16];
      if (s.cnt[i] == 0) {
        setCard(scr, i, s.label[i], "-");
      } else {
        fmtTime(last, sizeof last, s.lastUs[i] / 1000.0);
        fmtTime(avg, sizeof avg, s.sumUs[i] / 1000.0 / s.cnt[i]);
        snprintf(buf, sizeof buf, "%s  avg %s", last, avg);
        setCard(scr, i, s.label[i], buf);
      }
    }
  }

  // Info line: phase and time in the measuring window; status
  if (s.phase >= 0 && s.measuring) {
    const uint32_t t = (millis() - s.measureStartMs) / 1000;
    snprintf(scr.info, sizeof scr.info, "phase %u/%u  %lu s", k + 1, pqcenergy::count(),
             (unsigned long)t);
  } else {
    snprintf(scr.info, sizeof scr.info, "phase %u/%u", k + 1, pqcenergy::count());
  }
  strlcpy(scr.status, s.status, sizeof scr.status);
  scr.statusColor = s.statusColor;

  // Footer
  snprintf(scr.footLeft, sizeof scr.footLeft, "#%lu  fail 0", (unsigned long)total);
  strlcpy(scr.footRight, s.phase >= 0 ? "busy" : "A:start B:phase", sizeof scr.footRight);
}

static void uiTask(void *) {
  Screen scr;
  for (;;) {
    M5.update();
    const bool idle = !startRequested;
    if (idle && M5.BtnB.wasPressed()) {
      portENTER_CRITICAL(&mux);
      st.sel = (st.sel + 1) % pqcenergy::count();
      portEXIT_CRITICAL(&mux);
      setStatus("ready", COL_LABEL);
    }
    bool go = idle && M5.BtnA.wasPressed();
    bool shot = false;
    while (Serial.available()) {
      const char c = Serial.read();
      if (c == 'g' && idle) go = true;
      if (c == '#') shot = true;  // screenshot
    }
    if (go) startRequested = true;
    buildScreen(scr);
    draw(canvas, scr);
    if (shot) dump(canvas, Serial);
    vTaskDelay(pdMS_TO_TICKS(200));  // 5 Hz
  }
}

// ---- Arduino ---------------------------------------------------------------
void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);
  M5.Display.setRotation(1);  // landscape, 240 x 135
  M5.Display.setBrightness(96);
  canvas.createSprite(M5.Display.width(), M5.Display.height());
  memset(&st, 0, sizeof st);
  st.phase = -1;
  setStatus("ready", COL_LABEL);
  pqcenergy::begin(&gui);  // SHA accelerator self-tests
  Serial.printf("\nREADY pqc-energy-gui,phases=%u,phase_s=%d,cpu_mhz=%u\n",
                pqcenergy::count(), PHASE_S, (unsigned)getCpuFrequencyMhz());
  xTaskCreatePinnedToCore(energyTask, "energy", 24 * 1024, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(uiTask, "ui", 8 * 1024, NULL, 1, NULL, 0);
}

void loop() { vTaskDelete(NULL); }  // the UI and the workloads run in their own tasks
