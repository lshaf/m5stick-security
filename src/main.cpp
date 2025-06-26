#include <Arduino.h>
#include <M5StickCPlus2.h>
#include "screen.h"
#include "keypad.h"

Screen screen;
UNIT_MINIENCODERC encoder;
Keypad keypad(&encoder);

void setup()
{
  Serial.begin(115200);
  screen.init();
  screen.drawHeader(false);
  keypad.show();
}

void loop()
{
  StickCP2.update();
  screen.drawTimeHeader();
  static bool lastConnected = false;
  keypad.handleInput();
}
