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
  static unsigned long pressStartTime = 0;
  static bool wasPressed = false;
  int threshold = 500;

  bool isEncoderPressed = !mini_encoder->getButtonStatus();
  unsigned long now = millis();

  if (isEncoderPressed && !wasPressed) {
    pressStartTime = now;
  }

  EncoderButtonState currentState = IDLE;
  if (!isEncoderPressed && wasPressed) {
    unsigned long pressDuration = now - pressStartTime;
    if (pressDuration >= threshold) {
      currentState = EncoderButtonState::LONG_PRESSED;
    } else {
      currentState = EncoderButtonState::PRESSED;
    }
  }

  wasPressed = isEncoderPressed;
  return currentState;
}