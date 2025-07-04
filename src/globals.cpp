#include "globals.h"
#include "M5StickCPlus2.h"
#include "BleKeyboard.h"

Keypad keypad;
BleKeyboard bleKeyboard("M5Security", "Espressif", 100);
int SELECTED_COLOR = TFT_BLUE;
