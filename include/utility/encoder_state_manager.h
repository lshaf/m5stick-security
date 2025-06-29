#pragma once
#include "Unit_MiniEncoderC.h"

class EncoderStateManager {
public:
  EncoderStateManager(UNIT_MINIENCODERC& p_encoder) : mini_encoder(&p_encoder) {}
  void updateEncoderState();
  bool movedLeft() const { return _isRotatedLeft; }
  bool movedRight() const { return _isRotatedRight; }
  bool isMoved() const { return _isRotatedLeft || _isRotatedRight; }
  bool wasPressed() const { return _encoderButtonState == PRESSED; }
  bool wasDoublePressed() const { return _encoderButtonState == DOUBLE_PRESSED; }
  bool wasLongPressed() const { return _encoderButtonState == LONG_PRESSED; }

private:
  UNIT_MINIENCODERC* mini_encoder;
  bool _isRotatedLeft = false;
  bool _isRotatedRight = false;
  enum EncoderButtonState {
    IDLE,
    PRESSED,
    DOUBLE_PRESSED,
    LONG_PRESSED,
  } _encoderButtonState = IDLE;

  EncoderButtonState getLatestButtonState();
};