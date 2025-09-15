#pragma once

#include "components/menu_manager.h"

class BluetoothScreen : public MenuManager {
public:
  BluetoothScreen();
  void updateScreen() override;
  void handleInput() override;
  void destroy() override;
  virtual bool isScreenAlwaysOn() const { 
    if (this->currentState == this->STATE_MOUSE) {
      return true;
    }

    return false; 
  }
private:
  enum ScreenState {
    STATE_MAIN,
    STATE_DEVICE_LIST,
    STATE_PASSWORD,
    STATE_TACHIYOMI,
    STATE_MOUSE,
  } currentState = STATE_MAIN;
  enum {
    ACT_NONE,
    ACT_SCRL_DOWN,
    ACT_SCRL_UP
  } lastActivity = ACT_NONE;
  enum {
    TACHIYOMI_VOLUME,
    TACHIYOMI_SCROLL
  } tachiyomiMode = TACHIYOMI_VOLUME;

  struct PasswordItem {
    String label;
    String password;
  };

  struct ImuData {
    float x;
    float y;
    float z;
  };
  struct UpdateImuData {
    ImuData previous;
    ImuData current;
  } currentImuData;

  int selectedDeviceIndex = 0;
  int selectedPasswordIndex = 0;
  String passwordInput = "";
  bool isConnected = false;
  
  std::vector<PasswordItem> passwordItems;

  uint8_t acceleration = 30;
  UpdateImuData dotPosition();

  void loadSavedPasswords();
  void sendPasswordToDevice(const String& password);
};