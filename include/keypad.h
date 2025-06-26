#include <Arduino.h>
#include "Unit_MiniEncoderC.h"

class Keypad
{
public:
  Keypad(UNIT_MINIENCODERC* encoder = nullptr);
  void show();
  void handleInput();

private:
  UNIT_MINIENCODERC* encoder = nullptr;
  int selectedRow = 0;
  int selectedCol = 0;
  int tapIndex = 0;
  unsigned long lastTapTime = 0;
  const unsigned long tapTimeout = 800; // ms
  String message = "";
};