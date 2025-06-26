#include <Arduino.h>
#include <M5StickCPlus2.h>
#include "Unit_MiniEncoderC.h"
#include "screen.h"
#include "keypad.h"

Screen screen;
UNIT_MINIENCODERC encoder;
Keypad keypad(&encoder);

void setup()
{
  Serial.begin(115200);
  StickCP2.begin();
  encoder.begin(&Wire, MINIENCODERC_ADDR, 0, 26, 100000UL);
  screen.init();
  keypad.show();
  screen.drawHeader(false);
}

void loop()
{
  StickCP2.update();
  screen.drawTimeHeader();
  static bool lastConnected = false;
  keypad.handleInput();
}
