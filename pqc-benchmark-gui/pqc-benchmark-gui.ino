/*
 * pqc-benchmark-gui - the uniform benchmark of the five PQC libraries with
 * a live screen on the M5StickC PLUS2 (same layout as the library demos)
 *
 * The workloads are those of the headless experiment firmware
 * ../pqc-benchmark (both use ../libraries/PQCBench), and the serial port
 * prints the same CSV lines. The display, its SPI traffic and the UI task
 * on core 0 make the numbers slightly different, so the figures for the
 * paper come from the headless firmware; this one is for demonstrations.
 *
 *   Button B (right side):   select the group (ML-KEM, ML-DSA, SLH-DSA,
 *                            LMS/HSS, XMSS, NVS state, radio control, memory)
 *   Button A (front, "M5"):  run the selected group
 * Serial commands (115200 baud) work as in the headless firmware; '#' prints
 * a screenshot (pqcgui::dump).
 *
 * The radio is switched on at start (Wi-Fi station started, not connected:
 * true random numbers from esp_fill_random()).
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

// ---- state shared between the benchmark task (core 1) and the UI (core 0) --
struct Shared {
  char sel;      // selected group
  char running;  // group being run, 0 if idle
  char alg[16], variant[32];
  bool hw;
  int nops;
  char label[3][16];
  uint32_t planned[3], cnt[3];
  double lastUs[3], sumUs[3];
  int active;  // running operation, -1 none
  uint32_t activeStartMs;
  uint32_t rows, failures;  // CSV result rows and failed operations
  uint32_t setNo;           // parameter sets started in this group
  bool memMode;             // cards show memory rows
  char status[32];
  uint16_t statusColor;
};
static Shared st;
static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
static QueueHandle_t cmdQueue;

// Header names: "ML-KEM-768" -> "ML-KEM" + "768", "LMS-SHA256-M24-H10-W4"
// -> "LMS" + "192 H10 W4", "XMSSMT-SHAKE256_20/4_192" -> "XMSS^MT" +
// "SHAKE 20/4_192" (the full names are in the CSV output)
static void shortName(const char *scheme, char *alg, size_t na, char *var, size_t nv) {
  const char *p;
  if ((p = strstr(scheme, "ML-KEM-")) == scheme || (p = strstr(scheme, "ML-DSA-")) == scheme) {
    snprintf(alg, na, "%.6s", scheme);
    snprintf(var, nv, "%s", scheme + 7);
  } else if (strncmp(scheme, "SLH-DSA-", 8) == 0) {
    snprintf(alg, na, "SLH-DSA");
    snprintf(var, nv, "%s", scheme + 8);
  } else if (strncmp(scheme, "LMS-", 4) == 0 || strncmp(scheme, "HSS-", 4) == 0) {
    snprintf(alg, na, "%.3s", scheme);
    const bool shake = strstr(scheme, "SHAKE") != nullptr;
    const bool m24 = strstr(scheme, "-M24-") != nullptr;
    const char *h = strstr(scheme, "-H");  // "-H10-W4" / "-H10+H10-W4"
    char hw[24];
    snprintf(hw, sizeof hw, "%s", h ? h + 1 : "");
    char *d = strrchr(hw, '-');
    if (d) *d = ' ';
    snprintf(var, nv, "%s%s%s", shake ? "SHAKE " : "", m24 ? "192 " : "", hw);
  } else if (strncmp(scheme, "XMSSMT-", 7) == 0) {
    snprintf(alg, na, "XMSS^MT");
    const bool shake = strncmp(scheme + 7, "SHAKE", 5) == 0;
    const char *u = strchr(scheme, '_');
    snprintf(var, nv, "%s%s", shake ? "SHAKE " : "", u ? u + 1 : scheme + 7);
  } else if (strncmp(scheme, "XMSS-", 5) == 0) {
    snprintf(alg, na, "XMSS");
    snprintf(var, nv, "%s", scheme + 5);
  } else {
    snprintf(alg, na, "%s", scheme);
    var[0] = 0;
  }
}

// Card labels as in the demos
static const char *opLabel(const char *op) {
  static const char *const MAP[][2] = {
      {"keygen", "KeyGen"},     {"encaps", "Encaps"},  {"decaps", "Decaps"},
      {"sign", "Sign"},         {"verify", "Verify"},  {"keygen-nvs", "KeyGen"},
      {"sign-nvs", "Sign"},     {"load-nvs", "Load"},  {"nvs-write-8B", "Write 8 B"},
  };
  for (auto &m : MAP)
    if (strcmp(op, m[0]) == 0) return m[1];
  return op;
}

static void setStatus(const char *s, uint16_t color) {
  portENTER_CRITICAL(&mux);
  strlcpy(st.status, s, sizeof st.status);
  st.statusColor = color;
  portEXIT_CRITICAL(&mux);
}

// ---- observer: CSV to the serial port, progress to the shared state --------
class GuiObserver : public pqcbench::Observer {
public:
  void line(const char *text) override {
    Serial.printf("%s\n", text);
    if (text[0] == 'R' && text[1] == ',') {  // R,...,n,fail,...: count failures
      int field = 0;
      const char *p = text;
      while (*p && field < 7) {
        if (*p++ == ',') field++;
      }
      const unsigned fail = (unsigned)atoi(p);
      portENTER_CRITICAL(&mux);
      st.rows++;
      st.failures += fail;
      portEXIT_CRITICAL(&mux);
    } else if (text[0] == 'M' && text[1] == ',') {
      portENTER_CRITICAL(&mux);
      st.rows++;
      portEXIT_CRITICAL(&mux);
    }
  }
  void setStart(char group, const char *scheme, const char *impl, int nops,
                const char *const ops[3], const uint32_t planned[3]) override {
    char alg[16], var[32];
    shortName(scheme, alg, sizeof alg, var, sizeof var);
    portENTER_CRITICAL(&mux);
    strlcpy(st.alg, alg, sizeof st.alg);
    strlcpy(st.variant, var, sizeof st.variant);
    st.hw = strcmp(impl, "HW") == 0;
    st.nops = nops;
    st.memMode = false;
    for (int i = 0; i < 3; i++) {
      strlcpy(st.label[i], i < nops && ops[i] ? opLabel(ops[i]) : "", sizeof st.label[i]);
      st.planned[i] = i < nops ? planned[i] : 0;
      st.cnt[i] = 0;
      st.lastUs[i] = st.sumUs[i] = 0;
    }
    st.active = -1;
    st.setNo++;
    portEXIT_CRITICAL(&mux);
  }
  void opStart(int op) override {
    portENTER_CRITICAL(&mux);
    st.active = op;
    st.activeStartMs = millis();
    portEXIT_CRITICAL(&mux);
  }
  void opDone(int op, double us, int rc) override {
    portENTER_CRITICAL(&mux);
    st.active = -1;
    st.cnt[op]++;
    st.lastUs[op] = us;
    st.sumUs[op] += us;
    portEXIT_CRITICAL(&mux);
  }
  void memory(const char *scheme, const char *impl, const char *op, long stack, long heap,
              int rc) override {
    char alg[16], var[32], text[40];
    shortName(scheme, alg, sizeof alg, var, sizeof var);
    if (rc != 0) {
      snprintf(text, sizeof text, "FAILED rc %d", rc);
    } else {
      snprintf(text, sizeof text, "stk %.1fK  heap %.1fK", stack / 1024.0, heap / 1024.0);
    }
    portENTER_CRITICAL(&mux);
    if (!st.memMode || strcmp(st.variant, var) != 0 || strcmp(st.alg, alg) != 0 ||
        st.nops >= 3) {  // new scheme: cards restart
      st.memMode = true;
      st.nops = 0;
      st.setNo++;
      strlcpy(st.alg, alg, sizeof st.alg);
      strlcpy(st.variant, var, sizeof st.variant);
    }
    st.hw = strcmp(impl, "HW") == 0;
    strlcpy(st.label[st.nops], opLabel(op), sizeof st.label[0]);
    strlcpy(memText[st.nops], text, sizeof memText[0]);
    memRc[st.nops] = rc;
    st.nops++;
    portEXIT_CRITICAL(&mux);
  }
  void groupDone(char cmd, double seconds) override {
    char buf[32];
    snprintf(buf, sizeof buf, "done %.0f s", seconds);
    if (strchr(pqcbench::GROUPS, cmd)) setStatus(buf, st.failures ? COL_ERR : COL_OK);
  }
  char memText[3][40];
  int memRc[3];
};
static GuiObserver gui;

// ---- benchmark task (core 1, as in the headless firmware) -------------------
static void benchTask(void *) {
  for (;;) {
    char cmd;
    if (xQueueReceive(cmdQueue, &cmd, portMAX_DELAY) != pdTRUE) continue;
    const bool group = strchr(pqcbench::GROUPS, cmd) != nullptr ||
                       cmd == 'q' || cmd == 'r';
    if (group) {
      portENTER_CRITICAL(&mux);
      st.running = cmd;
      st.sel = cmd;
      st.rows = st.failures = st.setNo = 0;
      st.nops = 0;
      st.memMode = false;
      st.active = -1;
      strlcpy(st.alg, pqcbench::groupTitle(cmd), sizeof st.alg);
      st.variant[0] = 0;
      portEXIT_CRITICAL(&mux);
      setStatus("running", COL_WARN);
    }
    pqcbench::run(cmd);
    portENTER_CRITICAL(&mux);
    st.running = 0;
    st.active = -1;
    portEXIT_CRITICAL(&mux);
  }
}

// ---- UI (core 0) -----------------------------------------------------------
static M5Canvas canvas(&M5.Display);  // off-screen buffer: flicker-free

static void buildScreen(Screen &scr) {
  Shared s;
  char memText[3][40];
  int memRc[3];
  portENTER_CRITICAL(&mux);
  s = st;
  memcpy(memText, gui.memText, sizeof memText);
  memcpy(memRc, gui.memRc, sizeof memRc);
  portEXIT_CRITICAL(&mux);
  clear(scr);

  if (!s.running && s.setNo == 0) {  // nothing run yet in this group
    strlcpy(scr.alg, "Bench", sizeof scr.alg);
    strlcpy(scr.variant, pqcbench::groupTitle(s.sel), sizeof scr.variant);
    setCard(scr, 0, "Group", pqcbench::groupTitle(s.sel));
    setCard(scr, 1, "Radio", pqcbench::radioOn() ? "on (true RNG)" : "off");
    setCard(scr, 2, "Output", "CSV on serial");
  } else {
    strlcpy(scr.alg, s.alg, sizeof scr.alg);
    strlcpy(scr.variant, s.variant, sizeof scr.variant);
    scr.hw = s.hw;
    for (int i = 0; i < 3; i++) {
      if (i >= s.nops) {
        setCard(scr, i, "", "");
        continue;
      }
      char buf[40], last[16], avg[16];
      if (s.memMode) {
        setCard(scr, i, s.label[i], memText[i], false, memRc[i] ? COL_ERR : COL_TEXT);
      } else if (s.active == i) {
        fmtTime(last, sizeof last, (double)(millis() - s.activeStartMs));
        snprintf(buf, sizeof buf, "running %s", last);
        setCard(scr, i, s.label[i], buf, true);
      } else if (s.cnt[i] == 0) {
        setCard(scr, i, s.label[i], "-");
      } else {
        fmtTime(last, sizeof last, s.lastUs[i] / 1000.0);
        fmtTime(avg, sizeof avg, s.sumUs[i] / 1000.0 / s.cnt[i]);
        snprintf(buf, sizeof buf, "%s  avg %s", last, avg);
        setCard(scr, i, s.label[i], buf);
      }
    }
  }

  // Info line: progress of the running operation; status
  if (s.running && !s.memMode && s.active >= 0 && s.active < s.nops) {
    snprintf(scr.info, sizeof scr.info, "set %lu  n %lu/%lu", (unsigned long)s.setNo,
             (unsigned long)s.cnt[s.active] + 1, (unsigned long)s.planned[s.active]);
  } else if (s.running) {
    snprintf(scr.info, sizeof scr.info, "set %lu", (unsigned long)s.setNo);
  } else {
    snprintf(scr.info, sizeof scr.info, "radio %s", pqcbench::radioOn() ? "on" : "off");
  }
  strlcpy(scr.status, s.status, sizeof scr.status);
  scr.statusColor = s.statusColor;

  // Footer
  snprintf(scr.footLeft, sizeof scr.footLeft, "#%lu  fail %lu", (unsigned long)s.rows,
           (unsigned long)s.failures);
  strlcpy(scr.footRight, s.running ? "busy" : "A:run B:group", sizeof scr.footRight);
}

static void uiTask(void *) {
  Screen scr;
  for (;;) {
    M5.update();
    if (!st.running) {
      if (M5.BtnB.wasPressed()) {
        const char *g = strchr(pqcbench::GROUPS, st.sel);
        const char next = g && g[1] ? g[1] : pqcbench::GROUPS[0];
        portENTER_CRITICAL(&mux);
        st.sel = next;
        st.setNo = st.rows = st.failures = 0;
        st.nops = 0;
        portEXIT_CRITICAL(&mux);
        setStatus("ready", COL_LABEL);
      }
      if (M5.BtnA.wasPressed()) {
        const char c = st.sel;
        xQueueSend(cmdQueue, &c, 0);
      }
    }
    bool shot = false;
    while (Serial.available()) {
      char c = Serial.read();
      if (c == '#') shot = true;  // screenshot
      else if (c > ' ') xQueueSend(cmdQueue, &c, 0);
    }
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
  st.sel = pqcbench::GROUPS[0];
  st.active = -1;
  setStatus("ready", COL_LABEL);
  pqcbench::begin(&gui);  // SHA accelerator self-tests
  cmdQueue = xQueueCreate(8, 1);
  Serial.println("\nREADY pqc-benchmark-gui");
  pqcbench::run('w');  // radio on: true random numbers
  xTaskCreatePinnedToCore(benchTask, "bench", 32 * 1024, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(uiTask, "ui", 8 * 1024, NULL, 1, NULL, 0);
}

void loop() { vTaskDelete(NULL); }  // the UI and the benchmark run in their own tasks
