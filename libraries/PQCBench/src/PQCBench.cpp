/*
 * PQCBench.cpp - benchmark workloads of the five ESP32 PQC libraries
 * (fips203-mlkem-esp32, fips204-mldsa-esp32, fips205-slhdsa-esp32, sp800-208-lms-esp32, sp800-208-xmss-esp32)
 *
 * Commands (pqcbench::run):
 *   k  ML-KEM-512/768/1024: keygen, encaps, decaps        (N_FAST runs)
 *   d  ML-DSA-44/65/87: keygen, sign, verify               (N_FAST runs)
 *   s  SLH-DSA, 12 sets in software + 6 SHA2 sets with the SHA accelerator
 *   l  LMS / HSS, representative sets, software and SHA accelerator
 *   x  XMSS / XMSS^MT, representative sets, software and SHA accelerator
 *   n  state-management costs of LMS / XMSS with NVS storage
 *   m  peak stack and heap per operation (MIN_RUNS runs each, fresh key per run)
 *   q  the same for ML-KEM and ML-DSA only (alternative memory-mode build)
 *   r  the same for ML-DSA only (use with -DMEM_STACK_KB=8)
 *   c  radio-interference control: all ML-KEM and ML-DSA sets plus a subset of
 *      the hash-based sets (run with the radio off, compare with the main groups)
 *   w / o  radio on (Wi-Fi station started, not connected) / radio off
 *   e  print the environment line
 *
 * Every operation runs at least MIN_RUNS (10) times; every run is recorded.
 *
 * Output lines (Observer::line):
 *   T,group,scheme,impl,op,radio,run,us,cycles   one line per run (printed
 *     after the timed loop, before the R summary line)
 *   R,group,scheme,impl,op,radio,n,fail,mean_us,sd_us,median_us,min_us,max_us,mean_cycles
 *     (sd: sample standard deviation over the n runs)
 *   M,scheme,impl,op,stack_bytes,heap_peak_total,heap_peak_internal,persistent_total,
 *     persistent_internal,rc,run   (one line per run; rc: return code of the operation, 0 = ok;
 *     stack and heap net of the empty measurement task; "key-in-ram" rows:
 *     heap held by a generated key incl. its RAM storage; total = internal
 *     RAM + PSRAM)
 *   S,scheme,impl,item,value      (state-management measurements)
 *   E,...   environment           DONE,cmd,seconds
 * Cycles: CPU cycle counter for operations < 15 s, time x 240 MHz above
 * (the 32-bit counter wraps after 17.9 s).
 *
 * Build: with -DLMS_ESP32_HW_SHA -DXMSS_ESP32_HW_SHA (the software rows
 * switch the accelerator off at run time). Optional build with
 * -DMLDSA_ESP32_NO_REDUCE_RAM -DMLKEM_ESP32_USE_STACK for the alternative
 * ML-KEM / ML-DSA memory modes.
 *
 * Copyright (c) 2026 The pqc-esp32-experiments authors
 * SPDX-License-Identifier: Apache-2.0
 */

#include "PQCBench.h"
#include <WiFi.h>
#include <esp_timer.h>
#include <esp_cpu.h>
#include <esp_heap_caps.h>
#include <esp_idf_version.h>
#include <nvs.h>
#include <stdarg.h>
#include <functional>
#include <MLKEM.h>
#include <MLDSA.h>
#include <SLHDSA.h>
#include <LMS.h>
#include <LMS_NVS.h>
#include <XMSS.h>
#include <XMSS_NVS.h>

namespace pqcbench {

static Observer *obs = nullptr;

// one output line to the observer
static void emit(const char *fmt, ...) {
  char buf[320];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof buf, fmt, ap);
  va_end(ap);
  size_t n = strlen(buf);
  while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r')) buf[--n] = 0;
  if (obs) obs->line(buf);
}

static void setStart(char group, const char *scheme, const char *impl, int nops, const char *o0,
                     const char *o1, const char *o2, uint32_t n0, uint32_t n1, uint32_t n2) {
  const char *ops[3] = {o0, o1, o2};
  const uint32_t planned[3] = {n0, n1, n2};
  if (obs) obs->setStart(group, scheme, impl, nops, ops, planned);
}

#ifndef MIN_RUNS
#define MIN_RUNS 10  // every operation runs at least this often
#endif
#ifndef N_FAST
#define N_FAST 1000  // ML-KEM, ML-DSA
#endif
#ifndef N_SLH_F
#define N_SLH_F 10   // SLH-DSA "f" sets (keygen, sign)
#endif
#ifndef N_SLH_S
#define N_SLH_S MIN_RUNS  // SLH-DSA "s" sets (keygen, sign)
#endif
#ifndef N_SLH_V
#define N_SLH_V 10   // SLH-DSA verify
#endif
#ifndef N_HBS_SIGN
#define N_HBS_SIGN 256  // LMS / XMSS signatures per set (at most 2^h')
#endif
#ifndef N_HBS_KEYGEN
#define N_HBS_KEYGEN MIN_RUNS  // LMS / XMSS key generations per set
#endif
#if N_FAST < MIN_RUNS || N_SLH_F < MIN_RUNS || N_SLH_S < MIN_RUNS || N_SLH_V < MIN_RUNS || \
    N_HBS_SIGN < MIN_RUNS || N_HBS_KEYGEN < MIN_RUNS
#error "every operation must run at least MIN_RUNS times"
#endif

static const char *MLDSA_MODE =
#if defined(MLDSA_ESP32_NO_REDUCE_RAM)
    "fast-sign";
#else
    "reduce-ram";
#endif
static const char *MLKEM_MODE =
#if defined(MLKEM_ESP32_USE_STACK)
    "stack";
#else
    "heap";
#endif

// ---- buffers --------------------------------------------------------------
static uint8_t pk[2592], sk[4896], ct[1568], ss1[32], ss2[32];
static uint8_t sig[49856];
static uint8_t msg[32];
static bool radioOn_ = false;

// ---- statistics -------------------------------------------------------------
struct Acc {
  const char *op;
  int idx;  // operation index for the observer
  uint32_t n, cap, fail;
  double *us;
  double *cyc;  // cycles per run
  double cycSum;
  int64_t t0;
  uint32_t c0;
};

static void accInit(Acc &a, const char *op, uint32_t cap, int idx = 0) {
  a.op = op;
  a.idx = idx;
  a.n = 0;
  a.cap = cap;
  a.fail = 0;
  a.cycSum = 0;
  a.us = (double *)malloc(sizeof(double) * cap);
  a.cyc = (double *)malloc(sizeof(double) * cap);
}

static inline void tic(Acc &a) {
  if (obs) obs->opStart(a.idx);  // before the timed region
  a.t0 = esp_timer_get_time();
  a.c0 = esp_cpu_get_cycle_count();
}

static inline void toc(Acc &a, int rc) {
  const uint32_t c = esp_cpu_get_cycle_count() - a.c0;
  const int64_t us = esp_timer_get_time() - a.t0;
  const double cy = us < 15000000 ? (double)c : (double)us * 240.0;
  if (a.n < a.cap) {
    a.us[a.n] = (double)us;
    a.cyc[a.n] = cy;
  }
  a.cycSum += cy;
  a.n++;
  if (rc != 0) a.fail++;
  if (obs) obs->opDone(a.idx, (double)us, rc);  // after the timed region
}

static int cmpd(const void *x, const void *y) {
  const double a = *(const double *)x, b = *(const double *)y;
  return a < b ? -1 : a > b;
}

static void report(const char *group, const char *scheme, const char *impl, Acc &a) {
  const uint32_t n = a.n < a.cap ? a.n : a.cap;
  const char *radio = radioOn_ ? "on" : "off";
  for (uint32_t i = 0; i < n; i++) {  // every run, in order
    emit("T,%s,%s,%s,%s,%s,%u,%.0f,%.0f", group, scheme, impl, a.op, radio, (unsigned)(i + 1),
         a.us[i], a.cyc[i]);
  }
  double mean = 0, sd = 0, med = 0, mn = 0, mx = 0;
  if (n > 0) {
    for (uint32_t i = 0; i < n; i++) mean += a.us[i];
    mean /= n;
    for (uint32_t i = 0; i < n; i++) sd += (a.us[i] - mean) * (a.us[i] - mean);
    sd = n > 1 ? sqrt(sd / (n - 1)) : 0;
    qsort(a.us, n, sizeof(double), cmpd);
    med = n & 1 ? a.us[n / 2] : (a.us[n / 2 - 1] + a.us[n / 2]) / 2;
    mn = a.us[0];
    mx = a.us[n - 1];
  }
  emit("R,%s,%s,%s,%s,%s,%u,%u,%.1f,%.1f,%.1f,%.1f,%.1f,%.0f\n", group, scheme, impl,
                a.op, radioOn_ ? "on" : "off", (unsigned)n, (unsigned)a.fail, mean, sd, med, mn, mx,
                n ? a.cycSum / a.n : 0.0);
  free(a.us);
  free(a.cyc);
  a.us = a.cyc = nullptr;
}

static void rnd(uint8_t *b, size_t n) { esp_fill_random(b, n); }

// ---- ML-KEM -------------------------------------------------------------------
struct KemImpl {
  const char *name;
  int (*kg)(uint8_t *, uint8_t *);
  int (*enc)(uint8_t *, uint8_t *, const uint8_t *);
  int (*dec)(uint8_t *, const uint8_t *, const uint8_t *);
};
static const KemImpl KEMS[] = {
    {"ML-KEM-512", MLKEM512::generateKeypair, MLKEM512::encapsulate, MLKEM512::decapsulate},
    {"ML-KEM-768", MLKEM768::generateKeypair, MLKEM768::encapsulate, MLKEM768::decapsulate},
    {"ML-KEM-1024", MLKEM1024::generateKeypair, MLKEM1024::encapsulate, MLKEM1024::decapsulate},
};

static void benchKem(const char *group, const KemImpl &k, uint32_t n) {
  setStart(group[0], k.name, MLKEM_MODE, 3, "keygen", "encaps", "decaps", n, n, n);
  Acc a, b, c;
  accInit(a, "keygen", n);
  accInit(b, "encaps", n, 1);
  accInit(c, "decaps", n, 2);
  for (uint32_t i = 0; i < n; i++) {
    tic(a);
    int r = k.kg(pk, sk);
    toc(a, r);
    tic(b);
    r = k.enc(ct, ss1, pk);
    toc(b, r);
    tic(c);
    r = k.dec(ss2, ct, sk);
    toc(c, r != 0 || memcmp(ss1, ss2, 32) != 0);
  }
  report(group, k.name, MLKEM_MODE, a);
  report(group, k.name, MLKEM_MODE, b);
  report(group, k.name, MLKEM_MODE, c);
}

// ---- ML-DSA -------------------------------------------------------------------
struct DsaImpl {
  const char *name;
  int (*kg)(uint8_t *, uint8_t *);
  int (*sign)(uint8_t *, size_t *, const uint8_t *, size_t, const uint8_t *, const uint8_t *,
              size_t);
  int (*verify)(const uint8_t *, size_t, const uint8_t *, size_t, const uint8_t *,
                const uint8_t *, size_t);
};
static const DsaImpl DSAS[] = {
    {"ML-DSA-44", MLDSA44::generateKeypair, MLDSA44::sign, MLDSA44::verify},
    {"ML-DSA-65", MLDSA65::generateKeypair, MLDSA65::sign, MLDSA65::verify},
    {"ML-DSA-87", MLDSA87::generateKeypair, MLDSA87::sign, MLDSA87::verify},
};

static void benchDsa(const char *group, const DsaImpl &d, uint32_t n) {
  setStart(group[0], d.name, MLDSA_MODE, 3, "keygen", "sign", "verify", n, n, n);
  Acc a, b, c;
  accInit(a, "keygen", n);
  accInit(b, "sign", n, 1);
  accInit(c, "verify", n, 2);
  for (uint32_t i = 0; i < n; i++) {
    size_t sl = 0;
    rnd(msg, sizeof msg);
    tic(a);
    int r = d.kg(pk, sk);
    toc(a, r);
    tic(b);
    r = d.sign(sig, &sl, msg, sizeof msg, sk, nullptr, 0);
    toc(b, r);
    tic(c);
    r = d.verify(sig, sl, msg, sizeof msg, pk, nullptr, 0);
    toc(c, r);
  }
  report(group, d.name, MLDSA_MODE, a);
  report(group, d.name, MLDSA_MODE, b);
  report(group, d.name, MLDSA_MODE, c);
}

// ---- SLH-DSA ------------------------------------------------------------------
struct SlhImpl {
  const char *name;
  const slh_param_t *sw, *hw;  // hw == nullptr: SHAKE (no accelerator)
  bool small;
};
static const SlhImpl SLHS[] = {
    {"SLH-DSA-SHA2-128f", &slh_dsa_sha2_128f, &slh_dsa_sha2_128f_hw, false},
    {"SLH-DSA-SHA2-192f", &slh_dsa_sha2_192f, &slh_dsa_sha2_192f_hw, false},
    {"SLH-DSA-SHA2-256f", &slh_dsa_sha2_256f, &slh_dsa_sha2_256f_hw, false},
    {"SLH-DSA-SHAKE-128f", &slh_dsa_shake_128f, nullptr, false},
    {"SLH-DSA-SHAKE-192f", &slh_dsa_shake_192f, nullptr, false},
    {"SLH-DSA-SHAKE-256f", &slh_dsa_shake_256f, nullptr, false},
    {"SLH-DSA-SHA2-128s", &slh_dsa_sha2_128s, &slh_dsa_sha2_128s_hw, true},
    {"SLH-DSA-SHA2-192s", &slh_dsa_sha2_192s, &slh_dsa_sha2_192s_hw, true},
    {"SLH-DSA-SHA2-256s", &slh_dsa_sha2_256s, &slh_dsa_sha2_256s_hw, true},
    {"SLH-DSA-SHAKE-128s", &slh_dsa_shake_128s, nullptr, true},
    {"SLH-DSA-SHAKE-192s", &slh_dsa_shake_192s, nullptr, true},
    {"SLH-DSA-SHAKE-256s", &slh_dsa_shake_256s, nullptr, true},
};

static void benchSlh(const char *group, const char *name, const slh_param_t *prm, bool hw,
                     uint32_t nKs, uint32_t nV) {
  const slh_param_t *p = hw ? prm : slhdsa_esp32_coop(prm);
  const size_t n = slh_pk_sz(prm) / 2;
  setStart(group[0], name, hw ? "HW" : "SW", 3, "keygen", "sign", "verify", nKs, nKs, nV);
  Acc a, b, c;
  accInit(a, "keygen", nKs);
  accInit(b, "sign", nKs, 1);
  accInit(c, "verify", nV, 2);
  size_t sl = 0;
  for (uint32_t i = 0; i < nKs; i++) {
    uint8_t seed[96], addrnd[32];
    rnd(seed, sizeof seed);
    rnd(addrnd, sizeof addrnd);
    rnd(msg, sizeof msg);
    slhdsa_esp32::HwSession hs(hw);
    tic(a);
    slh_keygen_internal(sk, pk, seed, seed + n, seed + 2 * n, p);
    toc(a, 0);
    tic(b);
    sl = slh_sign(sig, msg, sizeof msg, nullptr, 0, sk, addrnd, p);
    toc(b, sl != slh_sig_sz(prm));
  }
  for (uint32_t i = 0; i < nV; i++) {
    slhdsa_esp32::HwSession hs(hw);
    tic(c);
    int ok = slh_verify(msg, sizeof msg, sig, sl, nullptr, 0, pk, p);
    toc(c, ok != 1);
  }
  const char *impl = hw ? "HW" : "SW";
  report(group, name, impl, a);
  report(group, name, impl, b);
  report(group, name, impl, c);
}

// ---- LMS / HSS ----------------------------------------------------------------
struct LmsSet {
  const char *name;
  unsigned levels;
  LMSLevel lv[2];
  bool shake;
  bool swAlso;  // software row too (false: too slow, HW only)
};
static const LmsSet LMSS[] = {
    {"LMS-SHA256-M32-H5-W1", 1, {{LMS_SHA256_M32_H5, LMOTS_SHA256_N32_W1}}, false, true},
    {"LMS-SHA256-M32-H5-W2", 1, {{LMS_SHA256_M32_H5, LMOTS_SHA256_N32_W2}}, false, true},
    {"LMS-SHA256-M32-H5-W4", 1, {{LMS_SHA256_M32_H5, LMOTS_SHA256_N32_W4}}, false, true},
    {"LMS-SHA256-M32-H5-W8", 1, {{LMS_SHA256_M32_H5, LMOTS_SHA256_N32_W8}}, false, true},
    {"LMS-SHA256-M32-H10-W2", 1, {{LMS_SHA256_M32_H10, LMOTS_SHA256_N32_W2}}, false, true},
    {"LMS-SHA256-M32-H10-W4", 1, {{LMS_SHA256_M32_H10, LMOTS_SHA256_N32_W4}}, false, true},
    {"LMS-SHA256-M32-H10-W8", 1, {{LMS_SHA256_M32_H10, LMOTS_SHA256_N32_W8}}, false, true},
    {"LMS-SHA256-M24-H10-W4", 1, {{LMS_SHA256_M24_H10, LMOTS_SHA256_N24_W4}}, false, true},
    {"LMS-SHA256-M24-H10-W8", 1, {{LMS_SHA256_M24_H10, LMOTS_SHA256_N24_W8}}, false, true},
    {"LMS-SHAKE-M32-H10-W4", 1, {{LMS_SHAKE_M32_H10, LMOTS_SHAKE_N32_W4}}, true, true},
    {"LMS-SHAKE-M24-H10-W4", 1, {{LMS_SHAKE_M24_H10, LMOTS_SHAKE_N24_W4}}, true, true},
    {"LMS-SHA256-M32-H15-W4", 1, {{LMS_SHA256_M32_H15, LMOTS_SHA256_N32_W4}}, false, false},
    {"HSS-SHA256-M32-H10+H10-W4", 2,
     {{LMS_SHA256_M32_H10, LMOTS_SHA256_N32_W4}, {LMS_SHA256_M32_H10, LMOTS_SHA256_N32_W4}}, false,
     true},
    {"HSS-SHA256-M24-H10+H10-W4", 2,
     {{LMS_SHA256_M24_H10, LMOTS_SHA256_N24_W4}, {LMS_SHA256_M24_H10, LMOTS_SHA256_N24_W4}}, false,
     true},
};

static LMSRamStorage lmsRam;
static LMSKey lmsKey;
static HSSKey hssKey;

static void benchLms(const char *group, const LmsSet &s, bool hw) {
  lms_hw_sha_set_enabled(hw);
  LMSKeyBase &key = s.levels > 1 ? (LMSKeyBase &)hssKey : (LMSKeyBase &)lmsKey;
  const uint32_t nkg = N_HBS_KEYGEN;
  {
    const uint32_t ns = s.levels > 1 || s.lv[0].lms != LMS_SHA256_M32_H5 ? N_HBS_SIGN : 32;
    setStart(group[0], s.name, hw ? "HW" : "SW", 3, "keygen", "sign", "verify", nkg, ns, ns);
  }
  Acc a, b, c;
  accInit(a, "keygen", nkg);
  for (uint32_t i = 0; i < nkg; i++) {
    tic(a);
    int r = s.levels > 1 ? hssKey.generate(lmsRam, s.levels, s.lv)
                         : lmsKey.generate(lmsRam, s.lv[0].lms, s.lv[0].ots);
    toc(a, r);
  }
  uint8_t kpk[64];
  size_t kpkLen = sizeof kpk;
  key.publicKey(kpk, &kpkLen);
  uint32_t n = key.totalSignatures() < N_HBS_SIGN ? (uint32_t)key.totalSignatures() : N_HBS_SIGN;
  accInit(b, "sign", n, 1);
  accInit(c, "verify", n, 2);
  for (uint32_t i = 0; i < n; i++) {
    size_t sl = sizeof sig;
    rnd(msg, sizeof msg);
    tic(b);
    int r = key.sign(msg, sizeof msg, sig, &sl);
    toc(b, r);
    tic(c);
    r = s.levels > 1 ? HSSKey::verify(kpk, kpkLen, msg, sizeof msg, sig, sl)
                     : LMSKey::verify(kpk, kpkLen, msg, sizeof msg, sig, sl);
    toc(c, r);
  }
  const char *impl = hw ? "HW" : "SW";
  report(group, s.name, impl, a);
  report(group, s.name, impl, b);
  report(group, s.name, impl, c);
  key.end();
  lmsRam.clear();
  lms_hw_sha_set_enabled(1);
}

// ---- XMSS / XMSS^MT -------------------------------------------------------------
struct XmssSet {
  const char *name;
  bool mt;
  uint32_t oid;
  bool shake;
};
static const XmssSet XMSSS[] = {
    {"XMSS-SHA2_10_256", false, XMSS_SHA2_10_256, false},
    {"XMSS-SHA2_10_192", false, XMSS_SHA2_10_192, false},
    {"XMSS-SHAKE256_10_256", false, XMSS_SHAKE256_10_256, true},
    {"XMSS-SHAKE256_10_192", false, XMSS_SHAKE256_10_192, true},
    {"XMSSMT-SHA2_20/2_256", true, XMSSMT_SHA2_20_2_256, false},
    {"XMSSMT-SHA2_20/4_256", true, XMSSMT_SHA2_20_4_256, false},
    {"XMSSMT-SHA2_20/4_192", true, XMSSMT_SHA2_20_4_192, false},
    {"XMSSMT-SHA2_40/8_256", true, XMSSMT_SHA2_40_8_256, false},
    {"XMSSMT-SHA2_60/12_256", true, XMSSMT_SHA2_60_12_256, false},
    {"XMSSMT-SHAKE256_20/4_256", true, XMSSMT_SHAKE256_20_4_256, true},
    {"XMSSMT-SHAKE256_20/4_192", true, XMSSMT_SHAKE256_20_4_192, true},
};

static XMSSRamStorage xmssRam;
static XMSSKey xmssKey;
static XMSSMTKey xmssmtKey;

static void benchXmss(const char *group, const XmssSet &s, bool hw) {
  xmss_hw_sha_set_enabled(hw);  // trees take the setting when they are created
  XMSSKeyBase &key = s.mt ? (XMSSKeyBase &)xmssmtKey : (XMSSKeyBase &)xmssKey;
  const xmss_param *p = s.mt ? xmssmt_param_get(s.oid) : xmss_param_get(s.oid);
  const uint32_t nkg = N_HBS_KEYGEN;
  {
    const uint32_t ns = (1u << p->th) < N_HBS_SIGN ? (1u << p->th) : N_HBS_SIGN;
    setStart(group[0], s.name, hw ? "HW" : "SW", 3, "keygen", "sign", "verify", nkg, ns, ns);
  }
  Acc a, b, c;
  accInit(a, "keygen", nkg);
  for (uint32_t i = 0; i < nkg; i++) {
    tic(a);
    int r = s.mt ? xmssmtKey.generate(xmssRam, s.oid) : xmssKey.generate(xmssRam, s.oid);
    toc(a, r);
  }
  uint8_t kpk[68];
  size_t kpkLen = sizeof kpk;
  key.publicKey(kpk, &kpkLen);
  const uint32_t per = 1u << p->th;
  const uint32_t n = per < N_HBS_SIGN ? per : N_HBS_SIGN;
  accInit(b, "sign", n, 1);
  accInit(c, "verify", n, 2);
  for (uint32_t i = 0; i < n; i++) {
    size_t sl = sizeof sig;
    rnd(msg, sizeof msg);
    tic(b);
    int r = key.sign(msg, sizeof msg, sig, &sl);
    toc(b, r);
    tic(c);
    r = s.mt ? XMSSMTKey::verify(kpk, kpkLen, msg, sizeof msg, sig, sl)
             : XMSSKey::verify(kpk, kpkLen, msg, sizeof msg, sig, sl);
    toc(c, r);
  }
  const char *impl = hw ? "HW" : "SW";
  report(group, s.name, impl, a);
  report(group, s.name, impl, b);
  report(group, s.name, impl, c);
  key.end();
  xmssRam.clear();
  xmss_hw_sha_set_enabled(1);
}

// ---- state management with NVS ----------------------------------------------------
static void stateLine(const char *scheme, const char *item, double value) {
  emit("S,%s,HW,%s,%.1f\n", scheme, item, value);
}

static void benchState() {
  // raw NVS update of an 8-byte counter (what every signature pays)
  {
    Preferences p;
    p.begin("pqbench", false);
    setStart('n', "NVS", "-", 1, "nvs-write-8B", nullptr, nullptr, 200, 0, 0);
    Acc a;
    accInit(a, "nvs-write-8B", 200);
    uint64_t v = 0;
    for (int i = 0; i < 200; i++) {
      v++;
      tic(a);
      size_t w = p.putBytes("ctr", &v, 8);
      toc(a, w != 8);
    }
    report("n", "NVS", "-", a);
    p.clear();
    p.end();
  }
  // LMS H10 W4 and XMSS^MT 20/4_192 with NVS storage (accelerator on)
  for (int which = 0; which < 2; which++) {
    LMSNvsStorage lstore;
    XMSSNvsStorage xstore;
    const char *name = which == 0 ? "LMS-SHA256-M32-H10-W4" : "XMSSMT-SHA2_20/4_192";
    if (which == 0) lstore.begin("pqbench"); else xstore.begin("pqbench");
    auto sign = [&](size_t *sl) {
      return which == 0 ? lmsKey.sign(msg, sizeof msg, sig, sl) : xmssmtKey.sign(msg, sizeof msg, sig, sl);
    };
    auto drop = [&] {  // free the RAM copy (the state stays in NVS)
      if (which == 0) lmsKey.end(); else xmssmtKey.end();
    };
    auto erase = [&] {
      drop();
      if (which == 0) LMSKeyBase::erase(lstore); else XMSSKeyBase::erase(xstore);
    };
    Acc g, s, l, f, s2;
    setStart('n', name, "HW", 3, "keygen-nvs", "sign-nvs", "load-nvs", MIN_RUNS,
             100 + 2 * MIN_RUNS, MIN_RUNS);
    // key generation including the initial NVS records (fresh key each run)
    accInit(g, "keygen-nvs", MIN_RUNS);
    for (int i = 0; i < MIN_RUNS; i++) {
      erase();
      tic(g);
      int r = which == 0 ? lmsKey.generate(lstore, LMS_SHA256_M32_H10, LMOTS_SHA256_N32_W4)
                         : xmssmtKey.generate(xstore, XMSSMT_SHA2_20_4_192);
      toc(g, r);
    }
    report("n", name, "HW", g);
    // signing with the write-ahead state update in NVS
    accInit(s, "sign-nvs", 100, 1);
    for (int i = 0; i < 100; i++) {
      size_t sl = sizeof sig;
      rnd(msg, sizeof msg);
      tic(s);
      int r = sign(&sl);
      toc(s, r);
    }
    report("n", name, "HW", s);
    // reload: free the RAM copy, load from NVS, then sign twice (the first
    // signature after a load rebuilds the tree caches)
    accInit(l, "load-nvs", MIN_RUNS, 2);
    accInit(f, "first-sign-after-load", MIN_RUNS, 1);
    accInit(s2, "second-sign-after-load", MIN_RUNS, 1);
    for (int i = 0; i < MIN_RUNS; i++) {
      drop();
      tic(l);
      int r = which == 0 ? lmsKey.load(lstore) : xmssmtKey.load(xstore);
      toc(l, r);
      size_t sl = sizeof sig;
      tic(f);
      r = sign(&sl);
      toc(f, r);
      sl = sizeof sig;
      tic(s2);
      r = sign(&sl);
      toc(s2, r);
    }
    report("n", name, "HW", l);
    report("n", name, "HW", f);
    report("n", name, "HW", s2);
    stateLine(name, "cache_ram_bytes",
              which == 0 ? (double)lmsKey.cacheSize() : (double)xmssmtKey.cacheSize());
    erase();
  }
  Preferences p;
  p.begin("pqbench", false);
  p.clear();
  p.end();
}

// ---- memory (one run per operation in a fresh task) -----------------------------------
#ifndef MEM_STACK_KB
#define MEM_STACK_KB 32  // max. need: ML-KEM stack mode ~22 KB
#endif
static const uint32_t MEM_STACK = MEM_STACK_KB * 1024;

struct MemJob {
  std::function<int()> fn;  // returns 0 on success
  int rc;
  size_t stack;
  long heapPeak, heapPeakInt;
  SemaphoreHandle_t done;
};

static void memTask(void *arg) {
  MemJob *j = (MemJob *)arg;
  const size_t free0 = heap_caps_get_free_size(MALLOC_CAP_8BIT);
  const size_t free0i = heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  heap_caps_monitor_local_minimum_free_size_start();
  j->rc = j->fn();
  const size_t minFree = heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT);
  const size_t minFreeI = heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  heap_caps_monitor_local_minimum_free_size_stop();
  j->heapPeak = (long)free0 - (long)minFree;
  j->heapPeakInt = (long)free0i - (long)minFreeI;
  j->stack = MEM_STACK - uxTaskGetStackHighWaterMark(NULL);
  xSemaphoreGive(j->done);
  vTaskDelete(NULL);
}

static size_t memBaseline = 0;
static int memRunNo = 1;  // run index printed with every M line
static long memBaseHeap = 0, memBaseHeapInt = 0;

static void memRun(const char *scheme, const char *impl, const char *op, std::function<int()> fn) {
  MemJob j;
  j.fn = fn;
  j.done = xSemaphoreCreateBinary();
  xTaskCreatePinnedToCore(memTask, "mem", MEM_STACK, &j, 1, NULL, 1);
  xSemaphoreTake(j.done, portMAX_DELAY);
  vSemaphoreDelete(j.done);
  vTaskDelay(pdMS_TO_TICKS(20));  // let the idle task free the task memory
  if (scheme == nullptr) {
    memBaseline = j.stack;
    memBaseHeap = j.heapPeak;
    memBaseHeapInt = j.heapPeakInt;
    emit("M,baseline,-,empty-task,%u,%ld,%ld,0,0,0,1", (unsigned)j.stack, j.heapPeak,
         j.heapPeakInt);
    return;
  }
  emit("M,%s,%s,%s,%u,%ld,%ld,0,0,%d,%d", scheme, impl, op, (unsigned)(j.stack - memBaseline),
       j.heapPeak - memBaseHeap, j.heapPeakInt - memBaseHeapInt, j.rc, memRunNo);
  if (obs) obs->memory(scheme, impl, op, (long)(j.stack - memBaseline), j.heapPeak - memBaseHeap, j.rc);
}

static void benchMemoryDsa();

static void benchMemoryLattice() {
  for (const KemImpl &k : KEMS) {
    for (memRunNo = 1; memRunNo <= MIN_RUNS; memRunNo++) {
      memRun(k.name, MLKEM_MODE, "keygen", [&] { return k.kg(pk, sk); });
      memRun(k.name, MLKEM_MODE, "encaps", [&] { return k.enc(ct, ss1, pk); });
      memRun(k.name, MLKEM_MODE, "decaps", [&] { return k.dec(ss2, ct, sk); });
    }
  }
  benchMemoryDsa();
}

// ML-DSA only: with a small measurement task (-DMEM_STACK_KB=8) the
// fast-signing mode's ~105 KB internal-heap peak still fits
static void benchMemoryDsa() {
  for (const DsaImpl &d : DSAS) {
    static size_t sl;
    for (memRunNo = 1; memRunNo <= MIN_RUNS; memRunNo++) {
      rnd(msg, sizeof msg);
      memRun(d.name, MLDSA_MODE, "keygen", [&] { return d.kg(pk, sk); });
      memRun(d.name, MLDSA_MODE, "sign", [&] { return d.sign(sig, &sl, msg, sizeof msg, sk, nullptr, 0); });
      memRun(d.name, MLDSA_MODE, "verify", [&] { return d.verify(sig, sl, msg, sizeof msg, pk, nullptr, 0); });
    }
  }
}

static void benchMemory() {
  memRunNo = 1;
  memRun(nullptr, "", "", [] { return 0; });  // baseline: the measurement task itself
  benchMemoryLattice();
  for (const SlhImpl &s : SLHS) {
#if defined(MEM_QUICK)
    if (s.small) continue;  // smoke test only: skip the slow "s" sets
#endif
    for (int hw = 0; hw < 2; hw++) {
      if (hw && s.hw == nullptr) continue;
      if (hw && s.small) continue;  // stack of the HW path: "f" sets suffice
      const slh_param_t *p = hw ? s.hw : slhdsa_esp32_coop(s.sw);
      const size_t n = slh_pk_sz(s.sw) / 2;
      static size_t sl;
      const char *impl = hw ? "HW" : "SW";
      for (memRunNo = 1; memRunNo <= MIN_RUNS; memRunNo++) {  // fresh key per run
        rnd(msg, sizeof msg);
        memRun(s.name, impl, "keygen", [&] {
          uint8_t seed[96];
          rnd(seed, sizeof seed);
          slhdsa_esp32::HwSession hs(hw);
          slh_keygen_internal(sk, pk, seed, seed + n, seed + 2 * n, p);
          return 0;
        });
        memRun(s.name, impl, "sign", [&] {
          slhdsa_esp32::HwSession hs(hw);
          sl = slh_sign(sig, msg, sizeof msg, nullptr, 0, sk, nullptr, p);
          return sl == slh_sig_sz(s.sw) ? 0 : -1;
        });
        memRun(s.name, impl, "verify", [&] {
          slhdsa_esp32::HwSession hs(hw);
          return slh_verify(msg, sizeof msg, sig, sl, nullptr, 0, pk, p) == 1 ? 0 : -1;
        });
      }
    }
  }
  // LMS / XMSS: keygen allocates the persistent tree caches
#if defined(MEM_QUICK)
  static const int LMS_MEM[] = {2, 5};
#else
  static const int LMS_MEM[] = {2, 5, 11, 12};  // H5 W4, H10 W4, H15 W4, HSS H10+H10
#endif
  for (int idx : LMS_MEM) {
    const LmsSet &s = LMSS[idx];
    LMSKeyBase &key = s.levels > 1 ? (LMSKeyBase &)hssKey : (LMSKeyBase &)lmsKey;
    static size_t sl;
    static uint8_t kpk[64];
    static size_t kpkLen;
    for (memRunNo = 1; memRunNo <= MIN_RUNS; memRunNo++) {  // fresh key per run
      rnd(msg, sizeof msg);
      const size_t free0 = heap_caps_get_free_size(MALLOC_CAP_8BIT);
      const size_t free0i = heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
      memRun(s.name, "HW", "keygen", [&] {
        return s.levels > 1 ? hssKey.generate(lmsRam, s.levels, s.lv)
                            : lmsKey.generate(lmsRam, s.lv[0].lms, s.lv[0].ots);
      });
      emit("M,%s,HW,key-in-ram,0,0,0,%ld,%ld,0,%d", s.name,
                    (long)free0 - (long)heap_caps_get_free_size(MALLOC_CAP_8BIT),
                    (long)free0i - (long)heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT),
                    memRunNo);
      if (memRunNo == 1) emit("S,%s,HW,cache_ram_bytes,%u", s.name, (unsigned)key.cacheSize());
      kpkLen = sizeof kpk;
      key.publicKey(kpk, &kpkLen);
      memRun(s.name, "HW", "sign", [&] {
        sl = sizeof sig;
        return key.sign(msg, sizeof msg, sig, &sl);
      });
      memRun(s.name, "HW", "verify", [&] {
        return s.levels > 1 ? HSSKey::verify(kpk, kpkLen, msg, sizeof msg, sig, sl)
                            : LMSKey::verify(kpk, kpkLen, msg, sizeof msg, sig, sl);
      });
      key.end();
      lmsRam.clear();
    }
  }
#if defined(MEM_QUICK)
  static const int XMSS_MEM[] = {5};
#else
  static const int XMSS_MEM[] = {0, 4, 5, 8};  // 10_256, 20/2_256, 20/4_256, 60/12_256
#endif
  for (int idx : XMSS_MEM) {
    const XmssSet &s = XMSSS[idx];
    XMSSKeyBase &key = s.mt ? (XMSSKeyBase &)xmssmtKey : (XMSSKeyBase &)xmssKey;
    static size_t sl;
    static uint8_t kpk[68];
    static size_t kpkLen;
    for (memRunNo = 1; memRunNo <= MIN_RUNS; memRunNo++) {  // fresh key per run
      rnd(msg, sizeof msg);
      const size_t free0 = heap_caps_get_free_size(MALLOC_CAP_8BIT);
      const size_t free0i = heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
      memRun(s.name, "HW", "keygen", [&] {
        return s.mt ? xmssmtKey.generate(xmssRam, s.oid) : xmssKey.generate(xmssRam, s.oid);
      });
      emit("M,%s,HW,key-in-ram,0,0,0,%ld,%ld,0,%d", s.name,
                    (long)free0 - (long)heap_caps_get_free_size(MALLOC_CAP_8BIT),
                    (long)free0i - (long)heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT),
                    memRunNo);
      if (memRunNo == 1) emit("S,%s,HW,cache_ram_bytes,%u", s.name, (unsigned)key.cacheSize());
      kpkLen = sizeof kpk;
      key.publicKey(kpk, &kpkLen);
      memRun(s.name, "HW", "sign", [&] {
        sl = sizeof sig;
        return key.sign(msg, sizeof msg, sig, &sl);
      });
      memRun(s.name, "HW", "verify", [&] {
        return s.mt ? XMSSMTKey::verify(kpk, kpkLen, msg, sizeof msg, sig, sl)
                    : XMSSKey::verify(kpk, kpkLen, msg, sizeof msg, sig, sl);
      });
      key.end();
      xmssRam.clear();
    }
  }
}

// ---- groups ---------------------------------------------------------------------------
static void groupKem() {
  for (const KemImpl &k : KEMS) benchKem("k", k, N_FAST);
}
static void groupDsa() {
  for (const DsaImpl &d : DSAS) benchDsa("d", d, N_FAST);
}
static void groupSlh() {
  for (const SlhImpl &s : SLHS) {
    const uint32_t n = s.small ? N_SLH_S : N_SLH_F;
    benchSlh("s", s.name, s.sw, false, n, N_SLH_V);
    if (s.hw != nullptr) benchSlh("s", s.name, s.hw, true, n, N_SLH_V);
  }
}
static void groupLms() {
  for (const LmsSet &s : LMSS) {
    if (s.swAlso) benchLms("l", s, false);
    if (!s.shake) benchLms("l", s, true);
  }
}
static void groupXmss() {
  for (const XmssSet &s : XMSSS) {
    benchXmss("x", s, false);
    if (!s.shake) benchXmss("x", s, true);
  }
}
static void groupControl() {
  for (const KemImpl &k : KEMS) benchKem("c", k, N_FAST);
  for (const DsaImpl &d : DSAS) benchDsa("c", d, N_FAST);
  benchSlh("c", SLHS[0].name, SLHS[0].sw, false, N_SLH_F, N_SLH_V);
  benchSlh("c", SLHS[0].name, SLHS[0].hw, true, N_SLH_F, N_SLH_V);
  benchLms("c", LMSS[5], true);
  benchXmss("c", XMSSS[5], true);
}

static void setRadio(bool on) {
  if (on) {
    WiFi.mode(WIFI_STA);  // station started, never connected: RF on, no traffic
  } else {
    WiFi.mode(WIFI_OFF);
  }
  radioOn_ = on;
  delay(500);
}

void printEnv() {
  // NVS fill level: a nearly full partition slows every write (garbage collection)
  nvs_stats_t nv = {};
  nvs_get_stats(NULL, &nv);
  emit("E,cpu_mhz=%u,idf=%s,arduino=%d.%d.%d,radio=%s,mlkem=%s,mldsa=%s,"
                "slh_hw=%d,lms_hw=%d,xmss_hw=%d,free_heap=%u,free_internal=%u,n_fast=%d,"
                "n_slh_f=%d,n_slh_s=%d,n_hbs_sign=%d,min_runs=%d,nvs_used=%u,nvs_free=%u,"
                "nvs_total=%u",
                (unsigned)getCpuFrequencyMhz(), esp_get_idf_version(), ESP_ARDUINO_VERSION_MAJOR,
                ESP_ARDUINO_VERSION_MINOR, ESP_ARDUINO_VERSION_PATCH, radioOn_ ? "on" : "off",
                MLKEM_MODE, MLDSA_MODE, slhdsa_esp32_hw_sha_active(), lms_hw_sha_active(),
                xmss_hw_sha_active(), (unsigned)heap_caps_get_free_size(MALLOC_CAP_8BIT),
                (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT), N_FAST,
                N_SLH_F, N_SLH_S, N_HBS_SIGN, MIN_RUNS, (unsigned)nv.used_entries,
                (unsigned)nv.free_entries, (unsigned)nv.total_entries);
}


bool radioOn() { return pqcbench::radioOn_; }

const char GROUPS[] = "kdslxncm";

const char *groupTitle(char cmd) {
  switch (cmd) {
    case 'k': return "ML-KEM";
    case 'd': return "ML-DSA";
    case 's': return "SLH-DSA";
    case 'l': return "LMS/HSS";
    case 'x': return "XMSS";
    case 'n': return "NVS state";
    case 'c': return "Radio ctrl";
    case 'm': return "Memory";
    case 'q': return "Memory KEM/DSA";
    case 'r': return "Memory DSA";
    default: return "?";
  }
}

void begin(Observer *o) {
  obs = o;
  // self-tests of the SHA accelerator backends before any measurement
  slhdsa_esp32_hw_sha_active();
  lms_hw_sha_active();
  xmss_hw_sha_active();
}

bool run(char cmd) {
  const int64_t t0 = esp_timer_get_time();
  switch (cmd) {
    case 'k': groupKem(); break;
    case 'd': groupDsa(); break;
    case 's': groupSlh(); break;
    case 'l': groupLms(); break;
    case 'x': groupXmss(); break;
    case 'n': benchState(); break;
    case 'm': benchMemory(); break;
    case 'q':
      memRun(nullptr, "", "", [] { return 0; });
      benchMemoryLattice();
      break;
    case 'r':
      memRun(nullptr, "", "", [] { return 0; });
      benchMemoryDsa();
      break;
    case 'c': groupControl(); break;
    case 'w': setRadio(true); break;
    case 'o': setRadio(false); break;
    case 'e': break;
    default: return false;
  }
  const double secs = (esp_timer_get_time() - t0) / 1e6;
  printEnv();
  emit("DONE,%c,%.1f s", cmd, secs);
  if (obs) obs->groupDone(cmd, secs);
  return true;
}

}  // namespace pqcbench
