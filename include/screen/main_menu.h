#pragma once
#include "components/menu_manager.h"
#include "components/router.h"
#include "screen/input_name.h"

class MainMenuScreen : public MenuManager {
public:
  MainMenuScreen() {
    menuItems = {
      {"Input Name", []() { Router::setScreen(new InputNameScreen()); }},
      {"Show Info", []() { /* show info screen */ }},
      {"Exit", []() { /* exit or reset */ }}
    };
  }
};