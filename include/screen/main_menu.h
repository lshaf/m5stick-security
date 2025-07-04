#pragma once
#include <M5StickCPlus2.h>
#include "components/menu_manager.h"
#include "components/router.h"
#include "screen/input_name.h"
#include "screen/bt_keyboard.h"
#include "globals.h"

class MainMenuScreen : public MenuManager {
public:
  MainMenuScreen() {
    menuItems = {
      {"Input Name", []() { Router::setScreen(new InputNameScreen()); }},
      {"BLE Keyboard", []() { 
        bleKeyboard.begin();
        Router::setScreen(new BluetoothKeyboardScreen()); 
      }},
      {"Power Off", []() { StickCP2.Power.powerOff(); }},
    };
  }
};