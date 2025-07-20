#pragma once
#include <M5_FPC1020A.h>
#include "utility/numberpad.h"
#include "utility/keypad.h"
#include "utility/encoder_state_manager.h"
#include "utility/config.h"

#define FINGER_PRINT_TX 32
#define FINGER_PRINT_RX 33
#define SELECTED_COLOR TFT_BLUE

extern M5_FPC1020A finger;
extern Config config;
extern String xorKey;
extern Keypad keypad;
extern NumberPad numberPad;
extern EncoderStateManager encoder;
