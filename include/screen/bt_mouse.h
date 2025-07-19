#pragma once

#include "components/menu_manager.h"
#include <BleMouse.h>

class BluetoothMouseScreen : public MenuManager {
public:
  BluetoothMouseScreen();
  void updateScreen() override;
  void handleInput() override;
  void destroy() override;
private:
  enum ScreenState {
    STATE_MAIN,
    STATE_TACHIYOMI,
  } currentState = STATE_MAIN;

  BleMouse bleMouse;
  enum {
    ACT_NONE,
    ACT_SCRL_DOWN,
    ACT_SCRL_UP
  } lastActivity = ACT_NONE;
  bool isConnected = false;
};