#!/usr/bin/env python3
# Copyright (c) 2026 The pqc-esp32-experiments authors
# SPDX-License-Identifier: Apache-2.0
"""
Host driver for pqc-benchmark: resets the board, sends the group commands
one after another and logs every line (with a host timestamp) to a file.

Usage: run_bench.py PORT LOGFILE COMMANDS [timeout_per_group_s]
  e.g. run_bench.py /dev/ttyACM0 run.log wekdlnxsocm 21600
"""
import sys, time, serial

port, logfile, cmds = sys.argv[1], sys.argv[2], sys.argv[3]
timeout = float(sys.argv[4]) if len(sys.argv) > 4 else 21600

s = serial.Serial()
s.port, s.baudrate, s.timeout = port, 115200, 0.5
s.dtr = False; s.rts = False
s.open()
s.rts = True; time.sleep(0.1); s.rts = False   # reset

log = open(logfile, "a", buffering=1)
buf = b""

def lines(deadline, until):
    global buf
    while time.time() < deadline:
        d = s.read(4096)
        if not d:
            continue
        buf += d
        while b"\n" in buf:
            ln, buf = buf.split(b"\n", 1)
            t = ln.decode(errors="replace").rstrip("\r")
            log.write(time.strftime("%Y-%m-%d %H:%M:%S ") + t + "\n")
            if until(t):
                return t
            if "Guru Meditation" in t or "abort()" in t or "Backtrace" in t:
                log.write("HOST: device crashed, stopping\n")
                sys.exit(2)
    log.write("HOST: timeout\n")
    sys.exit(3)

lines(time.time() + 30, lambda t: t.startswith("READY"))
for c in cmds:
    log.write(f"HOST: start group {c}\n")
    s.write(c.encode())
    lines(time.time() + timeout, lambda t, c=c: t.startswith(f"DONE,{c}"))
log.write("HOST: ALL GROUPS DONE\n")
