#include <BleCombo.h>

#include "screen/main_menu.h"
#include "screen/setting_menu.h"
#include "components/router.h"
#include "globals.h"

SettingMenuScreen::SettingMenuScreen() {
  this->title = "Setting";
  this->state = MENU;
  menuItems = {
    {"Brightness", [this]() { this->handleBrightnessSetting(); }},
    {"BLE Name", [this]() { this->handleBLENameSetting(); }},
    {"Back", []() { Router::setScreen(new MainMenuScreen()); }},
  };
}

void SettingMenuScreen::handleBrightnessSetting() {
  this->state = SET_BRIGHTNESS;
  numberPad.setValue(config.getBrightness());
  numberPad.setOnOk([this](int value) {
    config.setBrightness(value);
    StickCP2.Display.setBrightness(255 * value / 100);
    this->redrawScreen();
    this->state = MENU;
    numberPad.setOnOk([](int){});
  });
  numberPad.show("Brightness", 5, 100);
}

void SettingMenuScreen::handleBLENameSetting() {
  this->state = SET_BLE_NAME;
  keypad.setMessage(config.getBleName());
  keypad.setOnOk([this](const String& name) {
    config.setBleName(name);
    bleDevice.setName(name.c_str());
    this->redrawScreen();
    this->state = MENU;
    keypad.setOnOk([](const String&){});
  });
  keypad.show("BLE Name", 30);
}

void SettingMenuScreen::updateScreen() {
  if (state == MENU) {
    MenuManager::renderMenu();
  }
}

void SettingMenuScreen::handleInput() {
  if (this->state == MENU) {
    MenuManager::handleInput();
  } else if (this->state == SET_BRIGHTNESS) {
    numberPad.handleInput();
  } else if (this->state == SET_BLE_NAME) {
    keypad.handleInput();
  }
}
