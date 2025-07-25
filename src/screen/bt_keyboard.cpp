#include "globals.h"
#include "screen/bt_keyboard.h"
#include "components/router.h"
#include "screen/main_menu.h"
#include "components/menu_manager.h"
#include "utility/helper.h"
#include <LittleFS.h>
#include <NimBLEDevice.h>
#include <BleCombo.h>

BluetoothKeyboardScreen::BluetoothKeyboardScreen() {
  this->isConnected = false;
  this->currentState = STATE_MAIN;
  this->bleKeyboard.begin();
}

void BluetoothKeyboardScreen::loadPasswords() {
  File file = LittleFS.open("/meta_data.ps", "r");
  if (file) {
    while (file.available()) {
      String line = file.readStringUntil('\n');
      line.trim(); // Remove any leading/trailing whitespace
      if (line.length() > 0) {
        String decrpted = Helper::xorEncrypt(line, xorKey);
        String label = decrpted.substring(0, decrpted.indexOf(':'));
        String password = decrpted.substring(decrpted.indexOf(':') + 1);
        passwordItems.push_back({label, password});
      }
    }
    file.close();
  }
}

void BluetoothKeyboardScreen::sendPasswordToDevice(const String& password) {
  // Send the password to the connected Bluetooth device
  if (this->isConnected) {
    this->bleKeyboard.print(password);
    Helper::showAlert("Password sent");
  } else {
    Helper::showAlert("BLE not connected");
  }
}

void BluetoothKeyboardScreen::updateScreen() {
  // Update the screen with the current state of the Bluetooth keyboard
  StickCP2.Display.setTextColor(this->isConnected ? TFT_GREEN : TFT_RED, TFT_BLACK);
  StickCP2.Display.drawCenterString("BLE", StickCP2.Display.width() / 2, 5);

  if (currentState == STATE_MAIN) {
    this->title = "BLE Keyboard";
    menuItems = {
      {"Password", [this]() {
        this->loadPasswords();
        this->selected = 0; // Reset selection
        this->currentState = STATE_PASSWORD;
        this->redrawScreen();
      }},
      {"Tachiyomi", [this]() {
        this->selected = 0; // Reset selection
        this->currentState = STATE_TACHIYOMI;
        this->redrawScreen();
      }},
      {"Back", []() { Router::setScreen(new MainMenuScreen()); }},
    };
  } else if (currentState == STATE_PASSWORD) {
    this->title = "Type Password";
    menuItems.clear();
    for (const auto& item : this->passwordItems) {
      String pw = item.password;
      menuItems.push_back({
        item.label.c_str(),
        [this, pw]() {
          this->sendPasswordToDevice(pw);
        }
      });
    }
    menuItems.push_back({
      "Back",
      [this]() {
        this->selected = 0; // Reset selection
        this->passwordItems.clear();
        this->currentState = STATE_MAIN;
        this->redrawScreen();
      }
    });
  } else if (currentState == STATE_TACHIYOMI) {
    this->title = "Tachiyomi";
    menuItems.clear();
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    StickCP2.Display.drawCenterString(
      "Hold Encoder to Quit", 
      StickCP2.Display.width() / 2, 
      StickCP2.Display.height() - 13
    );

    StickCP2.Display.setTextSize(2);
    StickCP2.Display.setTextColor(this->lastActivity == ACT_SCRL_UP ? SELECTED_COLOR : TFT_WHITE, TFT_BLACK);
    StickCP2.Display.drawCenterString(
      "UP", 
      StickCP2.Display.width() / 2, 
      70
    );
    StickCP2.Display.setTextColor(this->lastActivity == ACT_SCRL_DOWN ? SELECTED_COLOR : TFT_WHITE, TFT_BLACK);
    StickCP2.Display.drawCenterString(
      "DOWN", 
      StickCP2.Display.width() / 2, 
      137
    );
  }

  BluetoothKeyboardScreen::renderMenu();
}

void BluetoothKeyboardScreen::handleInput() {
  bool bleConnected = bleDevice.isConnected();
  if (this->isConnected != bleConnected) {
    this->redrawScreen(false);
    this->isConnected = bleConnected;
  }

  if (this->currentState == STATE_TACHIYOMI) {
    static u_long lastActivityTime = 0;
    if (encoder.wasLongPressed()) {
      this->currentState = STATE_MAIN;
      this->redrawScreen();
    } else if (encoder.movedRight()) {
      this->bleKeyboard.press(KEY_MEDIA_VOLUME_DOWN);
      this->bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);
      this->lastActivity = ACT_SCRL_DOWN;
      lastActivityTime = millis();
    } else if (encoder.movedLeft()) {
      this->bleKeyboard.press(KEY_MEDIA_VOLUME_UP);
      this->bleKeyboard.release(KEY_MEDIA_VOLUME_UP);
      this->lastActivity = ACT_SCRL_UP;
      lastActivityTime = millis();
    }

    if (encoder.isMoved()) this->redrawScreen(false);
    if (millis() - lastActivityTime > 500 && this->lastActivity != ACT_NONE) {
      this->lastActivity = ACT_NONE;
      this->redrawScreen(false);
    }
  } else {
    MenuManager::handleInput();
  }
}

void BluetoothKeyboardScreen::destroy() {
  // Clean up resources if necessary
  StickCP2.Display.fillRect(StickCP2.Display.width() / 2 - 9, 5, 17, 8, TFT_BLACK);
  this->isConnected = false;
  // this->bleKeyboard.end();
}