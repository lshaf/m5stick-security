#pragma once

#include "components/menu_manager.h"

class BluetoothKeyboardScreen : public MenuManager {
public:
  void updateScreen() override;
  void handleInput() override;
  void destroy() override;
private:
  bool isConnected = false;
};