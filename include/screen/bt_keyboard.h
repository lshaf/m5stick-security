#pragma once

#include "components/menu_manager.h"
#include "BleKeyboard.h"

class BluetoothKeyboardScreen : public MenuManager {
public:
  BluetoothKeyboardScreen();
  void updateScreen() override;
  void handleInput() override;
  void destroy() override;
private:
  enum ScreenState {
    STATE_MAIN,
    STATE_PASSWORD,
    STATE_TACHIYOMI,
  } currentState = STATE_MAIN;
  enum {
    ACT_NONE,
    ACT_SCRL_DOWN,
    ACT_SCRL_UP
  } lastActivity = ACT_NONE;
  struct PasswordItem {
    String label;
    String password;
  };

  Keyboard_ bleKeyboard;
  std::vector<PasswordItem> passwordItems;
  bool isConnected = false;

  void loadPasswords();
  void sendPasswordToDevice(const String& password);
};