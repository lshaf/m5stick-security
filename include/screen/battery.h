#pragma once
#include "components/menu_screen.h"

class BatteryScreen : public MenuScreen {
public:
  BatteryScreen();
  void updateScreen() override;
  void handleInput() override;
  void destroy() override {};
};