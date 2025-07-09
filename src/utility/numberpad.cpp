#include "utility/numberpad.h"
#include <M5StickCPlus2.h>
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

  StickCP2.Display.fillRect(0, 18, StickCP2.Display.width(), StickCP2.Display.height() - 18, TFT_BLACK);
  updateScreen();
}

void NumberPad::updateScreen() {
  StickCP2.Display.setTextSize(1);
  
  // Draw the number pad UI
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  StickCP2.Display.drawCenterString(this->title, StickCP2.Display.width() / 2, 18);
  
  if (this->currentValue < 100) {
    StickCP2.Display.setTextSize(3);
  } else if (this->currentValue < 1000) {
    StickCP2.Display.setTextSize(2);
  } else {
    StickCP2.Display.setTextSize(1);
  }
  auto fh = StickCP2.Display.fontHeight();
  auto fw = StickCP2.Display.fontWidth() * (strlen(String(this->currentValue).c_str()) + 1);
  StickCP2.Display.fillRect(
    StickCP2.Display.width() / 2 - fw / 2,
    StickCP2.Display.height() / 2 - fh / 2,
    fw, fh, TFT_BLACK
  );
  StickCP2.Display.drawCenterString(
    String(this->currentValue), 
    StickCP2.Display.width() / 2,
    StickCP2.Display.height() / 2 - fh / 2
  );
  
  StickCP2.Display.setTextSize(1);
  StickCP2.Display.drawCenterString("Press to save", StickCP2.Display.width() / 2, StickCP2.Display.height() - 13);
}

void NumberPad::handleInput() {
  if (encoder.movedLeft()) setValue(currentValue - 1);
  if (encoder.movedRight()) setValue(currentValue + 1);
  if (encoder.wasPressed()) {
    if (onOkCallback) {
      onOkCallback(currentValue);
    }
  }

  if (encoder.wasPressed() || encoder.isMoved()) {
    updateScreen();
  }
}