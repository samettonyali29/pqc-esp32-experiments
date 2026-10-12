/*
 * PQCEnergy.cpp - steady-state power phases for board-level energy
 * measurement with an inline USB power meter (e.g. FNIRSI FNB-C2)
 *
 * Method: every phase runs one workload in a loop for at least the given
 * time and until every operation of the workload has run at least MIN_RUNS
 * (10) times; the power read from the meter during the phase times the
 * measured time per operation gives the energy per operation:
 *   E_total = P_phase * t_op,  E_incremental = (P_phase - P_idle) * t_op
 *
 * Output lines (Observer::line):
 *   PHASE,k,name,ready        prepared (key generation etc. done)
 *   PHASE,k,name,measuring    the measuring window starts
 *   T,k,op,run,us             one line per run (after the window; at most
 *                             MAX_STORED runs per operation are stored)
 *   RESULT,k,name,secs s,op,count ops,avg ms avg,sd ms sd[,op,...]
 *                             (sd: sample standard deviation over the runs)
 *
 * Copyright (c) 2026 The pqc-esp32-experiments authors
 * SPDX-License-Identifier: Apache-2.0
 */

#include "PQCBench.h"
#include <Preferences.h>
#include <esp_timer.h>
#include <stdarg.h>
#include <MLKEM.h>
#include <MLDSA.h>
#include <SLHDSA.h>
#include <LMS.h>
#include <XMSS.h>

namespace pqcenergy {

static Observer *obs = nullptr;

static void emit(const char *fmt, ...) {
  char buf[320];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof buf, fmt, ap);
  va_end(ap);
  if (obs) obs->line(buf);
}

// ---- buffers -----------------------------------------------------------------
static uint8_t kemPk[MLKEM768::PUBLIC_KEY_SIZE], kemSk[MLKEM768::SECRET_KEY_SIZE];
static uint8_t kemCt[MLKEM768::CIPHERTEXT_SIZE], ss1[32], ss2[32];
static uint8_t dsaPk[MLDSA65::PUBLIC_KEY_SIZE], dsaSk[MLDSA65::SECRET_KEY_SIZE];
static uint8_t dsaSig[MLDSA65::SIGNATURE_SIZE];
static uint8_t slhPk[64], slhSk[128], slhSig[17088];
static uint8_t hbsPk[68], hbsSig[10 * 1024];
static uint8_t msg[32];

#ifndef MIN_RUNS
#define MIN_RUNS 10  // every operation runs at least this often per phase
#endif
#ifndef MAX_STORED
#define MAX_STORED 50000  // runs stored per operation (PSRAM)
#endif

// ---- per-phase statistics ------------------------------------------------------
struct OpStat {
  const char *name;
  uint32_t count;
  int64_t sumUs;
  uint32_t *runs;  // time of every run (us), the first MAX_STORED
};
static OpStat ops[3];
static unsigned nops;

static void opsReset(const char *a, const char *b = nullptr, const char *c = nullptr) {
  for (OpStat &o : ops) free(o.runs);
  memset(ops, 0, sizeof ops);
  ops[0].name = a;
  ops[1].name = b;
  ops[2].name = c;
  nops = c ? 3 : b ? 2 : 1;
  for (unsigned i = 0; i < nops; i++) {  // >= 16 KB: allocated in PSRAM
    ops[i].runs = (uint32_t *)malloc(sizeof(uint32_t) * MAX_STORED);
  }
}

static inline void opAdd(int i, int64_t us) {
  if (ops[i].runs && ops[i].count < MAX_STORED) ops[i].runs[ops[i].count] = (uint32_t)us;
  ops[i].sumUs += us;
  ops[i].count++;
  if (obs) obs->opDone(i, (double)us);
}

#define TIMED(i, expr)                              \
  do {                                              \
    const int64_t t0_ = esp_timer_get_time();       \
    (void)(expr);                                   \
    opAdd(i, esp_timer_get_time() - t0_);           \
  } while (0)

// ---- phases --------------------------------------------------------------------
struct Phase {
  const char *name;
  void (*prep)();  // untimed set-up
  void (*work)();  // one iteration of the workload
};

static XMSSRamStorage xmssStore;
static XMSSMTKey xmssKey;
static LMSRamStorage lmsStore;
static HSSKey hssKey;
static Preferences nvs;
static const slh_param_t *slhSet;
static bool slhHw;

static void workIdle() {
  const int64_t t0 = esp_timer_get_time();
  vTaskDelay(pdMS_TO_TICKS(100));
  opAdd(0, esp_timer_get_time() - t0);
}
static void workBusy() {
  volatile uint32_t x = 0;
  const int64_t t0 = esp_timer_get_time();
  for (int i = 0; i < 1000000; i++) x += i;
  opAdd(0, esp_timer_get_time() - t0);
}

static void prepKem() { opsReset("keygen", "encaps", "decaps"); }
static void workKem() {
  TIMED(0, MLKEM768::generateKeypair(kemPk, kemSk));
  TIMED(1, MLKEM768::encapsulate(kemCt, ss1, kemPk));
  TIMED(2, MLKEM768::decapsulate(ss2, kemCt, kemSk));
}

static void prepDsa() {
  MLDSA65::generateKeypair(dsaPk, dsaSk);
  opsReset("sign", "verify");
}
static void workDsa() {
  size_t sl;
  esp_fill_random(msg, sizeof msg);
  TIMED(0, MLDSA65::sign(dsaSig, &sl, msg, sizeof msg, dsaSk));
  TIMED(1, MLDSA65::verify(dsaSig, sl, msg, sizeof msg, dsaPk));
}

static void prepSlh(const slh_param_t *sw, const slh_param_t *hw, bool useHw) {
  uint8_t seed[48];
  slhSet = useHw ? hw : slhdsa_esp32_coop(sw);
  slhHw = useHw;
  esp_fill_random(seed, sizeof seed);
  slhdsa_esp32::HwSession s(slhHw);
  slh_keygen_internal(slhSk, slhPk, seed, seed + 16, seed + 32, slhSet);
  opsReset("sign", "verify");
}
static void prepSlhSw() { prepSlh(&slh_dsa_sha2_128f, &slh_dsa_sha2_128f_hw, false); }
static void prepSlhHw() { prepSlh(&slh_dsa_sha2_128f, &slh_dsa_sha2_128f_hw, true); }
static void prepSlhShake() { prepSlh(&slh_dsa_shake_128f, &slh_dsa_shake_128f, false); }
static void workSlh() {
  size_t sl = 0;
  esp_fill_random(msg, sizeof msg);
  slhdsa_esp32::HwSession s(slhHw);
  TIMED(0, sl = slh_sign(slhSig, msg, sizeof msg, nullptr, 0, slhSk, nullptr, slhSet));
  TIMED(1, slh_verify(msg, sizeof msg, slhSig, sl, nullptr, 0, slhPk, slhSet));
}

static void prepHss(bool hw) {
  static const LMSLevel lv[2] = {{LMS_SHA256_M32_H10, LMOTS_SHA256_N32_W4},
                                 {LMS_SHA256_M32_H10, LMOTS_SHA256_N32_W4}};
  lms_hw_sha_set_enabled(1);  // key generation in hardware (faster set-up)
  hssKey.end();
  lmsStore.clear();
  hssKey.generate(lmsStore, 2, lv);
  lms_hw_sha_set_enabled(hw);
  size_t l = sizeof hbsPk;
  hssKey.publicKey(hbsPk, &l);
  opsReset("sign", "verify");
}
static void prepHssSw() { prepHss(false); }
static void prepHssHw() { prepHss(true); }
static void workHss() {
  size_t sl = sizeof hbsSig;
  esp_fill_random(msg, sizeof msg);
  TIMED(0, hssKey.sign(msg, sizeof msg, hbsSig, &sl));
  TIMED(1, HSSKey::verify(hbsPk, 60, msg, sizeof msg, hbsSig, sl));
}

static void prepXmss(bool hw) {
  xmss_hw_sha_set_enabled(hw);  // trees take the setting at creation
  xmssKey.end();
  xmssStore.clear();
  xmssKey.generate(xmssStore, XMSSMT_SHA2_20_4_256);
  size_t l = sizeof hbsPk;
  xmssKey.publicKey(hbsPk, &l);
  opsReset("sign", "verify");
}
static void prepXmssSw() { prepXmss(false); }
static void prepXmssHw() { prepXmss(true); }
static void workXmss() {
  size_t sl = sizeof hbsSig;
  esp_fill_random(msg, sizeof msg);
  TIMED(0, xmssKey.sign(msg, sizeof msg, hbsSig, &sl));
  TIMED(1, XMSSMTKey::verify(hbsPk, 68, msg, sizeof msg, hbsSig, sl));
}

static void prepNvs() {
  nvs.begin("pwrtest", false);
  opsReset("nvs-write-8B");
}
static void workNvs() {
  static uint64_t ctr;
  ctr++;
  TIMED(0, nvs.putBytes("ctr", &ctr, 8));
}

static void prepIdle() { opsReset("idle-100ms"); }
static void prepBusy() { opsReset("busy-loop-1M"); }

static const Phase PHASES[] = {
    {"Idle", prepIdle, workIdle},
    {"Busy loop 1 core", prepBusy, workBusy},
    {"ML-KEM-768 kg+enc+dec", prepKem, workKem},
    {"ML-DSA-65 sign+verify", prepDsa, workDsa},
    {"SLH-DSA-SHA2-128f SW", prepSlhSw, workSlh},
    {"SLH-DSA-SHA2-128f HW", prepSlhHw, workSlh},
    {"SLH-DSA-SHAKE-128f", prepSlhShake, workSlh},
    {"HSS H10+H10 W4 SW", prepHssSw, workHss},
    {"HSS H10+H10 W4 HW", prepHssHw, workHss},
    {"XMSSMT 20/4_256 SW", prepXmssSw, workXmss},
    {"XMSSMT 20/4_256 HW", prepXmssHw, workXmss},
    {"NVS write loop", prepNvs, workNvs},
    {"Idle again (drift)", prepIdle, workIdle},
};
static const unsigned NPHASES = sizeof PHASES / sizeof PHASES[0];
static const unsigned NVS_PHASE = 11;

void begin(Observer *o) {
  obs = o;
  slhdsa_esp32_hw_sha_active();
  lms_hw_sha_active();
  xmss_hw_sha_active();
}

unsigned count() { return NPHASES; }

const char *name(unsigned k) { return k < NPHASES ? PHASES[k].name : "?"; }

void prepare(unsigned k) {
  if (k >= NPHASES) return;
  const Phase &p = PHASES[k];
  p.prep();
  const char *names[3] = {ops[0].name, ops[1].name, ops[2].name};
  if (obs) obs->phaseStart(k, NPHASES, p.name, (int)nops, names);
  emit("PHASE,%u,%s,ready", k + 1, p.name);
}

static bool enoughRuns() {
  for (unsigned i = 0; i < nops; i++)
    if (ops[i].count < MIN_RUNS) return false;
  return true;
}

void measure(unsigned k, uint32_t minSeconds) {
  if (k >= NPHASES) return;
  const Phase &p = PHASES[k];
  emit("PHASE,%u,%s,measuring", k + 1, p.name);
  const int64_t t0 = esp_timer_get_time();
  while (esp_timer_get_time() - t0 < (int64_t)minSeconds * 1000000 || !enoughRuns()) p.work();
  const double secs = (esp_timer_get_time() - t0) / 1e6;
  // every run, then the summary (after the window: printing does not count)
  double sd[3] = {0, 0, 0};
  for (unsigned i = 0; i < nops; i++) {
    const uint32_t n = ops[i].count < MAX_STORED ? ops[i].count : MAX_STORED;
    if (!ops[i].runs || n == 0) continue;
    double mean = 0, var = 0;
    for (uint32_t r = 0; r < n; r++) {
      emit("T,%u,%s,%lu,%lu", k + 1, ops[i].name, (unsigned long)(r + 1),
           (unsigned long)ops[i].runs[r]);
      mean += ops[i].runs[r];
    }
    mean /= n;
    for (uint32_t r = 0; r < n; r++) var += (ops[i].runs[r] - mean) * (ops[i].runs[r] - mean);
    sd[i] = n > 1 ? sqrt(var / (n - 1)) : 0;
  }
  char buf[400];
  int n = snprintf(buf, sizeof buf, "RESULT,%u,%s,%.1f s", k + 1, p.name, secs);
  for (unsigned i = 0; i < nops && n < (int)sizeof buf; i++) {
    n += snprintf(buf + n, sizeof buf - n, ",%s,%lu ops,%.3f ms avg,%.3f ms sd", ops[i].name,
                  (unsigned long)ops[i].count,
                  ops[i].count ? ops[i].sumUs / 1000.0 / ops[i].count : 0.0, sd[i] / 1000.0);
  }
  if (obs) obs->line(buf);
  if (k == NVS_PHASE) {  // remove the test record
    nvs.clear();
    nvs.end();
  }
  if (obs) obs->phaseDone(k, secs);
}

}  // namespace pqcenergy
