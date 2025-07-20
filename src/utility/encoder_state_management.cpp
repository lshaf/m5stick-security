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
  unsigned long pressDuration = now - pressStartTime;
  if (!isEncoderPressed && wasPressed && pressDuration < threshold) {
    currentState = EncoderButtonState::PRESSED;
  }

  if (isEncoderPressed && wasPressed && pressDuration >= threshold) {
    currentState = EncoderButtonState::LONG_PRESSED;
  }

  wasPressed = isEncoderPressed;
  return currentState;
}

void EncoderStateManager::breathLight() {
  int default_delta = 4;
  static int brightness = 0;
  static int delta = default_delta; // Smaller value = slower fade
  static bool waiting = false;
  static unsigned long waitStart = 0;

  if (waiting) {
    // Wait 1.5 seconds before breathing in again
    if (millis() - waitStart >= 1500) {
      waiting = false;
      delta = default_delta;
    } else {
      this->offLight();
      return;
    }
  }

  brightness += delta;
  if (brightness >= 100) {
    brightness = 100;
    delta = -1; // Slow fade out (smaller absolute value)
  } else if (brightness <= 0) {
    brightness = 0;
    waiting = true;
    waitStart = millis();
    delta = default_delta; // Reset for next fade in
  }

  uint32_t color = brightness; // Green channel
  mini_encoder->setLEDColor(color);
}

void EncoderStateManager::offLight() {
  mini_encoder->setLEDColor(0x000000);
}