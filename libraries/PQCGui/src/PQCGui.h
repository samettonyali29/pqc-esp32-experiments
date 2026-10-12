/*
 * PQCGui.h - the screen layout of the PQC demos on the M5StickC PLUS2
 *
 * The same layout as the live demos of fips203-mlkem-esp32, fips204-mldsa-esp32,
 * fips205-slhdsa-esp32, sp800-208-lms-esp32 and sp800-208-xmss-esp32 (240 x 135, landscape):
 *
 *   +--------------------------------------------------+
 *   | ALG  Variant (bold)                    HW   87%  |  teal header
 *   | [ Label 1                     value / running ] |  three cards
 *   | [ Label 2                                     ] |
 *   | [ Label 3                                     ] |
 *   | info                                     status  |
 *   |                                                  |
 *   | footer left                        footer right  |
 *   +--------------------------------------------------+
 *
 * The firmware fills a Screen and calls draw(); a running card is drawn in
 * the active color with its text in orange, as in the demos.
 *
 * Copyright (c) 2026 The pqc-esp32-experiments authors
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef PQCGUI_H
#define PQCGUI_H

#include <M5Unified.h>

namespace pqcgui {

// Colors (RGB565), same in all demos
static const uint16_t COL_BG = 0x0000;
static const uint16_t COL_HEADER = 0x04B4;  // teal
static const uint16_t COL_CARD = 0x2124;
static const uint16_t COL_ACTIVE = 0x4A49;  // card of the running step
static const uint16_t COL_LABEL = 0xBDF7;
static const uint16_t COL_OK = 0x07E0;
static const uint16_t COL_ERR = 0xF800;
static const uint16_t COL_WARN = 0xFD20;
static const uint16_t COL_TEXT = 0xFFFF;

struct Card {
  char label[16];
  char text[40];
  bool active;     // running: active background, orange text
  uint16_t color;  // text color when not active
};

struct Screen {
  char alg[16];      // header: small algorithm name
  char variant[32];  // header: bold parameter set
  bool hw;           // header: "HW" tag (SHA accelerator in use)
  Card cards[3];
  char info[40];  // left of the info line
  char status[32];  // right of the info line
  uint16_t statusColor;
  char footLeft[40];
  char footRight[32];
};

inline void clear(Screen &s) {
  memset(&s, 0, sizeof s);
  for (Card &c : s.cards) {
    strcpy(c.text, "-");
    c.color = COL_TEXT;
  }
  s.statusColor = COL_LABEL;
}

inline void setCard(Screen &s, int i, const char *label, const char *text, bool active = false,
                    uint16_t color = COL_TEXT) {
  Card &c = s.cards[i];
  strlcpy(c.label, label, sizeof c.label);
  strlcpy(c.text, text, sizeof c.text);
  c.active = active;
  c.color = color;
}

// Times: "6.15 ms" below 100 ms, "123.4 ms" below 10 s, "54.27 s" above
// (same format in all demos)
inline void fmtTime(char *buf, size_t n, double ms) {
  if (ms < 100) {
    snprintf(buf, n, "%.2f ms", ms);
  } else if (ms < 10000) {
    snprintf(buf, n, "%.1f ms", ms);
  } else {
    snprintf(buf, n, "%.2f s", ms / 1000.0);
  }
}

// Header: small algorithm name + bold parameter set (same in all demos).
// A parameter set too wide for the bold font is drawn in the small font.
inline void drawTitle(M5Canvas &c, const char *alg, const char *variant, int right) {
  c.setTextColor(TFT_WHITE, COL_HEADER);
  c.setTextDatum(middle_left);
  c.setFont(&fonts::Font2);
  c.drawString(alg, 6, 12);
  const int x = 6 + c.textWidth(alg) + 6;
  c.setFont(&fonts::FreeSansBold9pt7b);
  if (x + c.textWidth(variant) <= right) {
    c.drawString(variant, x, 11);
  } else {
    c.setFont(&fonts::Font2);
    c.drawString(variant, x, 12);
  }
  c.setFont(&fonts::Font2);
}

inline void draw(M5Canvas &c, const Screen &s) {
  const int W = c.width();
  c.fillScreen(COL_BG);

  // Header
  c.fillRect(0, 0, W, 22, COL_HEADER);
  drawTitle(c, s.alg, s.variant, s.hw ? W - 66 : W - 42);
  c.setTextColor(TFT_WHITE, COL_HEADER);
  c.setTextDatum(middle_right);
  c.drawString(String(M5.Power.getBatteryLevel()) + "%", W - 5, 11);
  if (s.hw) c.drawString("HW", W - 46, 11);

  // Cards
  c.setFont(&fonts::Font2);
  for (int i = 0; i < 3; i++) {
    const Card &k = s.cards[i];
    const int y = 26 + i * 21;
    const uint16_t bg = k.active ? COL_ACTIVE : COL_CARD;
    c.fillRoundRect(3, y, W - 6, 19, 4, bg);
    c.setTextDatum(middle_left);
    c.setTextColor(COL_LABEL, bg);
    c.drawString(k.label, 9, y + 10);
    c.setTextColor(k.active ? COL_WARN : k.color, bg);
    c.setTextDatum(middle_right);
    c.drawString(k.text, W - 9, y + 10);
  }

  // Info line and status
  c.setTextDatum(middle_left);
  c.setTextColor(COL_LABEL, COL_BG);
  c.drawString(s.info, 6, 99);
  c.setTextColor(s.statusColor, COL_BG);
  c.setTextDatum(middle_right);
  c.drawString(s.status, W - 6, 99);

  // Footer
  c.setTextColor(COL_LABEL, COL_BG);
  c.setTextDatum(middle_left);
  c.drawString(s.footLeft, 6, 122);
  c.setTextDatum(middle_right);
  c.drawString(s.footRight, W - 6, 122);

  c.pushSprite(0, 0);
}

// Screenshot over the serial port: "SHOT,w,h" followed by w*h RGB565 pixels
// (little-endian), e.g. for documentation
inline void dump(M5Canvas &c, Stream &out) {
  const int w = c.width(), h = c.height();
  out.printf("SHOT,%d,%d\n", w, h);
  uint16_t row[240];
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w && x < 240; x++) row[x] = c.readPixel(x, y);
    out.write((const uint8_t *)row, sizeof(uint16_t) * (w < 240 ? w : 240));
  }
  out.flush();
}

}  // namespace pqcgui

#endif /* PQCGUI_H */
