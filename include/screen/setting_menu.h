#pragma once
#include "components/menu_manager.h"

class SettingMenuScreen : public MenuManager {
public:
  SettingMenuScreen();

  void updateScreen() override;
  void handleInput() override;
private:
  enum {
    SET_BRIGHTNESS,
    SET_BLE_NAME,
    MENU
  } state = MENU;

  void handleBrightnessSetting();
  void handleBLENameSetting();
};