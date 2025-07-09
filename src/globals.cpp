#include "globals.h"
#include "M5StickCPlus2.h"

Config config;
Keypad keypad;
NumberPad numberPad;
String xorKey = "m5stick-security";
BleKeyboard bleKeyboard("M5Security", "Espressif", 100);
int SELECTED_COLOR = TFT_BLUE;
