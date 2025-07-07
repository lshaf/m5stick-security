#pragma once

#include "components/menu_manager.h"

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
  } currentState = STATE_MAIN;
  struct PasswordItem {
    String label;
    String password;
  };

  std::vector<PasswordItem> passwordItems;
  bool isConnected = false;

  void loadPasswords();
  void sendPasswordToDevice(const String& password);
};