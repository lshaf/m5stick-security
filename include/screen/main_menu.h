#pragma once
#include <M5StickCPlus2.h>
#include "components/menu_manager.h"
#include "components/router.h"
#include "screen/input_name.h"

class MainMenuScreen : public MenuManager {
public:
  MainMenuScreen() {
    menuItems = {
      {"Input Name", []() { Router::setScreen(new InputNameScreen()); }},
      {"Show Info", []() { /* show info screen */ }},
      {"Power Off", []() { StickCP2.Power.powerOff(); }},
    };
  }
};