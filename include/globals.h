#pragma once
#include "utility/numberpad.h"
#include "utility/keypad.h"
#include "BleKeyboard.h"
#include "utility/encoder_state_manager.h"
#include "utility/config.h"

extern Config config;
extern String xorKey;
extern Keypad keypad;
extern NumberPad numberPad;
extern EncoderStateManager encoder;
extern BleKeyboard bleKeyboard;
extern int SELECTED_COLOR;
