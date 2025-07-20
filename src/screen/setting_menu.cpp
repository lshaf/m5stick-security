#include <BleCombo.h>

#include "utility/helper.h"
#include "screen/main_menu.h"
#include "screen/setting_menu.h"
#include "components/router.h"
#include "globals.h"

SettingMenuScreen::SettingMenuScreen() {
  this->handleMainMenu(false);
  numberPad.setOnOk([this](int value) {
    config.setBrightness(value);
    StickCP2.Display.setBrightness(255 * value / 100);
    this->handleMainMenu();
  });
  keypad.setOnOk([this](const String& name) {
    config.setBleName(name);
    bleDevice.setName(name.c_str());
    this->handleMainMenu();
  });
}

void SettingMenuScreen::handleMainMenu(bool redraw) {
  this->state = MENU;
  this->title = "Settings";
  this->menuItems = {
    {"Brightness", [this]() { this->handleBrightnessSetting(); }},
    {"BLE Name", [this]() { this->handleBLENameSetting(); }},
    {"Finger Lock", [this]() {
      while (finger.getUserCount() == 0xff) {
        Helper::showAlert("Wait for fingerprint");
      }

      this->selected = 0;
      this->state = SET_FINGER_LOCK; 
      this->redrawScreen(); 
    }},
    {"Finger Check", [this]() {
      if (!config.isFingerLockEnabled()) {
        Helper::showAlert("Finger Lock disabled");
        return;
      }

      Helper::showAlert("Put your finger");
      uint8_t res = finger.available();
      if (res == ACK_SUCCESS) {
        Helper::showAlert("Finger recognized");
      } else if (res == ACK_FAIL) {
        Helper::showAlert("Finger not recognized");
      } else {
        Helper::showAlert("Error reading finger");
      }

      this->handleMainMenu();
    }},
    {"Back", []() { Router::setScreen(new MainMenuScreen()); }},
  };

  if (redraw) {
    this->redrawScreen();
  }
}

void SettingMenuScreen::handleBrightnessSetting() {
  this->state = SET_BRIGHTNESS;
  numberPad.setValue(config.getBrightness());
  numberPad.show("Brightness", 1, 100);
}

void SettingMenuScreen::handleBLENameSetting() {
  this->state = SET_BLE_NAME;
  keypad.setMessage(config.getBleName());
  keypad.show("BLE Name", 30);
}

void SettingMenuScreen::handleFingerLockSetting() {
  static int localState = 0;

  if (localState == 0) {
    this->title = "Use Finger Lock?";
    this->menuItems = {
      {"Yes", [this]() {
        localState = 1;
        this->redrawScreen();
      }},
      {"No", [this]() {
        config.setFingerLockEnabled(false);
        finger.delAllFinger();
        // finger.sleep();

        this->handleMainMenu();
      }}
    };

    MenuManager::renderMenu();
  } else if (localState == 1) {
    finger.delAllFinger();
    this->title = "Register Finger Print";
    this->menuItems.clear();
    auto d = StickCP2.Display;

    d.setTextSize(1);
    d.setTextColor(TFT_WHITE, TFT_BLACK);
    for (int i = 0;i < 6; i++) {
      d.fillRect(0, d.height() / 2 - d.fontHeight() - 1, d.width(), d.fontHeight() * 2 + 2, TFT_BLACK);
      if (i == 0) {
        d.drawCenterString("Place your finger", d.width() / 2, d.height() / 2 - d.fontHeight() / 2 - 1);
        d.drawCenterString("on the sensor", d.width() / 2, d.height() / 2 + d.fontHeight() / 2 + 1);
      } else {
        d.drawCenterString("Place same finger", d.width() / 2, d.height() / 2 - d.fontHeight() / 2 - 1);
        d.drawCenterString("for sample " + String(i + 1), d.width() / 2, d.height() / 2 + d.fontHeight() / 2 + 1);
      }
      while (!finger.addFinger(1, 3, i)) {
        delay(1000); // retry if failed
      }
    }

    // finger.sleep();
    d.fillRect(0, d.height() / 2 - d.fontHeight() - 1, d.width(), d.fontHeight() * 2 + 2, TFT_BLACK);
    d.drawCenterString("Success", d.width() / 2, d.height() / 2 - d.fontHeight() / 2);
    delay(2000);

    config.setFingerLockEnabled(true);
    this->state = MENU;
    this->handleMainMenu();
  }
}

void SettingMenuScreen::updateScreen() {
  if (state == MENU) {
    MenuManager::renderMenu();
  } else if (state == SET_FINGER_LOCK) {
    this->handleFingerLockSetting();
  }
}

void SettingMenuScreen::handleInput() {
  if (this->state == MENU || this->state == SET_FINGER_LOCK) {
    MenuManager::handleInput();
  } else if (this->state == SET_BRIGHTNESS) {
    numberPad.handleInput();
  } else if (this->state == SET_BLE_NAME) {
    keypad.handleInput();
  }
}

void SettingMenuScreen::destroy() {
  if (this->state == SET_BRIGHTNESS) {
    numberPad.setOnOk([](int){});
  } else if (this->state == SET_BLE_NAME) {
    keypad.setOnOk([](const String&){});
  }
  
  // finger.sleep();
  this->state = MENU;
  this->redrawScreen();
}