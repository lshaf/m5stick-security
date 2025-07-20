#pragma once
#include "components/menu_manager.h"

class SettingMenuScreen : public MenuManager {
public:
  SettingMenuScreen();

  void updateScreen() override;
  void handleInput() override;
  void destroy() override;
private:
  enum {
    SET_BRIGHTNESS,
    SET_BLE_NAME,
    SET_FINGER_LOCK,
    MENU
  } state = MENU;

  void handleMainMenu(bool redraw = true);
  void handleBrightnessSetting();
  void handleBLENameSetting();
  void handleFingerLockSetting();
};