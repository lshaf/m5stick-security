#pragma once
#include "components/menu_screen.h"

class LockScreen : public MenuScreen {
public:
  LockScreen();
  bool isScreenAlwaysOn() const override { return true; }
  void updateScreen() override;
  void handleInput() override;
  void destroy() override;
};