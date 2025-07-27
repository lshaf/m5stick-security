#pragma once

#include "components/menu_manager.h"
#include "BleMouse.h"

class BluetoothMouseScreen : public MenuManager {
public:
  BluetoothMouseScreen();
  void updateScreen() override;
  void handleInput() override;
  void destroy() override;
  virtual bool isScreenAlwaysOn() const { 
    if (this->currentState == this->STATE_PC) {
      return true;
    }

    return false; 
  }
private:
  enum ScreenState {
    STATE_MAIN,
    STATE_TACHIYOMI,
    STATE_PC,
  } currentState = STATE_MAIN;

  struct ImuData {
    float x;
    float y;
    float z;
  };
  struct UpdateImuData {
    ImuData previous;
    ImuData current;
  } currentImuData;
  uint8_t acceleration = 30;
  UpdateImuData dotPosition();

  Mouse_ bleMouse;
  enum {
    ACT_NONE,
    ACT_SCRL_DOWN,
    ACT_SCRL_UP
  } lastActivity = ACT_NONE;
  bool isConnected = false;
};