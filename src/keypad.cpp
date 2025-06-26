#include <M5StickCPlus2.h>
#include "keypad.h"
#include "Unit_MiniEncoderC.h"

const char* KEYPAD_LAYOUT[4][3] = {
  {"1", "2abc", "3def"},
  {"4ghi", "5jkl", "6mno"},
  {"7pqrs", "8tuv", "9wxyz"},
  {"*", "0", "#"}
};

Keypad::Keypad(UNIT_MINIENCODERC* encoder):encoder(encoder){}

void Keypad::show() {
  int btnW = 35, btnH = 30, x0 = 5, y0 = 95;
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 3; ++col) {
      int x = x0 + col * (btnW + 5);
      int y = y0 + row * (btnH + 5);
      uint16_t color = (row == selectedRow && col == selectedCol) ? TFT_YELLOW : TFT_WHITE;
      StickCP2.Display.drawRect(x, y, btnW, btnH, color);
      StickCP2.Display.setCursor(x + 6, y + 6);
      StickCP2.Display.setTextColor(color, TFT_BLACK);
      StickCP2.Display.print(KEYPAD_LAYOUT[row][col]);
    }
  }
  // Draw message at the top
  StickCP2.Display.setCursor(5, 5);
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  StickCP2.Display.fillRect(0, 0, 160, 20, TFT_BLACK);
  StickCP2.Display.print(message);
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK); // restore
}

void Keypad::handleInput() {
  static int lastRow = -1, lastCol = -1;
  bool redraw = false;

  bool isEncoderPressed = !encoder->getButtonStatus();
  bool isRotatedLeft = encoder->getEncoderValue() < -1;
  bool isRotatedRight = encoder->getEncoderValue() > 1;
  if (isRotatedLeft || isRotatedRight) encoder->resetCounter();

  // Navigation: BtnB = right, BtnA long = down (customize as needed)
  if (isRotatedLeft) {
    selectedCol = (selectedCol + 1) % 3;
    redraw = true;
  }
  if (isEncoderPressed) {
    selectedRow = (selectedRow + 1) % 4;
    redraw = true;
  }

  // Multi-tap input: BtnA short press
  if (isRotatedRight) {
    unsigned long now = millis();
    if (lastRow == selectedRow && lastCol == selectedCol && (now - lastTapTime < tapTimeout)) {
      tapIndex++;
    } else {
      tapIndex = 0;
    }
    lastTapTime = now;
    lastRow = selectedRow;
    lastCol = selectedCol;

    const char* chars = KEYPAD_LAYOUT[selectedRow][selectedCol];
    int len = strlen(chars);
    char c = chars[tapIndex % len];
    // Replace last char if multi-tapping, else append
    if (!message.isEmpty() && (now - lastTapTime < tapTimeout) && (lastRow == selectedRow && lastCol == selectedCol)) {
      message.setCharAt(message.length() - 1, c);
    } else {
      message += c;
    }
    redraw = true;
  }

  if (redraw) show();
}