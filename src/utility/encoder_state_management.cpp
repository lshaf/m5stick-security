#include "Arduino.h"
#include "utility/encoder_state_manager.h"
#include "Unit_MiniEncoderC.h"

void EncoderStateManager::updateEncoderState() {
  static EncoderButtonState lastButtonState = IDLE;

  _isRotatedLeft = mini_encoder->getEncoderValue() <= -2;
  _isRotatedRight = mini_encoder->getEncoderValue() >= 2;
  
  if (_isRotatedLeft || _isRotatedRight) mini_encoder->resetCounter();

  _encoderButtonState = getLatestButtonState();
}

EncoderStateManager::EncoderButtonState EncoderStateManager::getLatestButtonState() {
  static unsigned long lastPressTime = 0;
  static unsigned long pressStartTime = 0;
  static bool wasPressed = false;
  static bool waitingForSecond = false;
  static int pressCount = 0;
  int threshold = 250;

  bool isEncoderPressed = !mini_encoder->getButtonStatus();
  unsigned long now = millis();

  if (isEncoderPressed && !wasPressed) {
    pressStartTime = now;
  }

  if (!isEncoderPressed && wasPressed) {
    unsigned long pressDuration = now - pressStartTime;
    if (pressDuration >= threshold) {
      pressCount = 0;
      waitingForSecond = false;
      wasPressed = isEncoderPressed;
      return EncoderButtonState::LONG_PRESSED;
    } else {
      pressCount++;
      if (pressCount == 1) {
        lastPressTime = now;
        waitingForSecond = true;
      } else if (pressCount == 2 && (now - lastPressTime) < threshold) {
        pressCount = 0;
        waitingForSecond = false;
        return EncoderButtonState::DOUBLE_PRESSED;
      }
    }
  }

  if (waitingForSecond && (now - lastPressTime) >= threshold) {
    waitingForSecond = false;
    pressCount = 0;
    return EncoderButtonState::PRESSED;
  }

  wasPressed = isEncoderPressed;
  return IDLE;
}