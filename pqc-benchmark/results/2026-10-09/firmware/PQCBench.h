/*
 * PQCBench.h - shared workloads of the PQC experiments
 *
 * The benchmark (pqcbench) and the energy phases (pqcenergy) of the five
 * ESP32 PQC libraries. The code here never touches the display: results go
 * to an Observer, so the same measurements run in the headless experiment
 * firmwares (Observer prints CSV) and in the GUI firmwares (Observer also
 * updates the screen).
 *
 * All Observer callbacks run in the caller's task and outside the timed
 * regions.
 *
 * Copyright (c) 2026 The pqc-esp32-experiments authors
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef PQCBENCH_H
#define PQCBENCH_H

#include <Arduino.h>

namespace pqcbench {

class Observer {
public:
  virtual ~Observer() {}
  // one complete output line (CSV etc.), without the newline
  virtual void line(const char *text) = 0;
  // a parameter set starts: up to 3 operations (CSV names, e.g. "keygen"),
  // planned runs of each
  virtual void setStart(char group, const char *scheme, const char *impl, int nops,
                        const char *const ops[3], const uint32_t planned[3]) {}
  virtual void opStart(int op) {}
  virtual void opDone(int op, double us, int rc) {}
  // one memory measurement (stack and heap net of the measurement task)
  virtual void memory(const char *scheme, const char *impl, const char *op, long stack,
                      long heap, int rc) {}
  virtual void groupDone(char cmd, double seconds) {}
};

// Runs the SHA hardware self-tests; obs receives all output.
void begin(Observer *obs);

// Runs one command (blocking; see the list in PQCBench.cpp). Every operation
// runs at least 10 times (MIN_RUNS) and every run is printed ("T" lines).
// Returns false for an unknown command.
//   k ML-KEM   d ML-DSA   s SLH-DSA   l LMS/HSS   x XMSS/XMSS^MT
//   n state management (NVS)   c radio control subset
//   m memory (all)   q memory ML-KEM + ML-DSA   r memory ML-DSA
//   w radio on   o radio off   e environment line
bool run(char cmd);

// The groups in display order, and their titles
extern const char GROUPS[];
const char *groupTitle(char cmd);

bool radioOn();
void printEnv();

}  // namespace pqcbench

namespace pqcenergy {

class Observer {
public:
  virtual ~Observer() {}
  virtual void line(const char *text) = 0;
  // phase k (0-based) of n prepared; op names of its workload
  virtual void phaseStart(unsigned k, unsigned n, const char *name, int nops,
                          const char *const ops[3]) {}
  virtual void opDone(int op, double us) {}
  virtual void phaseDone(unsigned k, double seconds) {}
};

void begin(Observer *obs);
unsigned count();
const char *name(unsigned k);
// Prepares phase k (untimed: key generation etc.), prints "PHASE,k,name,ready"
void prepare(unsigned k);
// Runs the workload of the prepared phase for at least minSeconds and until
// every operation has run at least 10 times (MIN_RUNS); prints
// "PHASE,k,name,measuring" before, one "T" line per run and the "RESULT"
// line (mean and standard deviation per operation) after the window
void measure(unsigned k, uint32_t minSeconds);

}  // namespace pqcenergy

#endif /* PQCBENCH_H */
