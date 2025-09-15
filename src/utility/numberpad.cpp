#include "utility/numberpad.h"
#include <M5Unified.h>
#include "globals.h"

NumberPad::NumberPad() {}

void NumberPad::setValue(int value) {
  if (value < minValue || value > maxValue) {
    return; // Value out of bounds
  }
  currentValue = value;
}

int NumberPad::getValue() const {
  return currentValue;
}

void NumberPad::setOnOk(std::function<void(int)> cb) {
  onOkCallback = cb;
}

void NumberPad::show(String title, int min, int max) {
  this->title = title;
  this->minValue = min;
  this->maxValue = max;

  if (currentValue < minValue || currentValue > maxValue) {
    currentValue = minValue; // Reset to min if out of bounds
  }

  M5.Lcd.fillRect(0, 18, M5.Lcd.width(), M5.Lcd.height() - 18, TFT_BLACK);
  updateScreen();
}

void NumberPad::updateScreen() {
  M5.Lcd.setTextSize(1);
  
  // Draw the number pad UI
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.drawCenterString(this->title, M5.Lcd.width() / 2, 18);
  
  if (this->currentValue < 100) {
    M5.Lcd.setTextSize(3);
  } else if (this->currentValue < 1000) {
    M5.Lcd.setTextSize(2);
  } else {
    M5.Lcd.setTextSize(1);
  }
  auto fh = M5.Lcd.fontHeight();
  auto fw = M5.Lcd.fontWidth() * (strlen(String(this->currentValue).c_str()) + 1);
  M5.Lcd.fillRect(
    M5.Lcd.width() / 2 - fw / 2,
    M5.Lcd.height() / 2 - fh / 2,
    fw, fh, TFT_BLACK
  );
  M5.Lcd.drawCenterString(
    String(this->currentValue), 
    M5.Lcd.width() / 2,
    M5.Lcd.height() / 2 - fh / 2
  );
  
  M5.Lcd.setTextSize(1);
  M5.Lcd.drawCenterString("Press to save", M5.Lcd.width() / 2, M5.Lcd.height() - 13);
}

void NumberPad::handleInput() {
  if (encoder.movedLeft()) setValue(currentValue - 1);
  if (encoder.movedRight()) setValue(currentValue + 1);
  if (encoder.wasMoved()) updateScreen();
  if (encoder.wasPressed()) {
    if (onOkCallback) {
      M5.Lcd.fillRect(0, 18, M5.Lcd.width(), M5.Lcd.height() - 18, TFT_BLACK);
      onOkCallback(currentValue);
    }
  }
}
