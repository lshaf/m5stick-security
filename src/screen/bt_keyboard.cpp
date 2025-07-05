#include "globals.h"
#include "screen/bt_keyboard.h"
#include "components/router.h"
#include "screen/main_menu.h"
#include "components/menu_manager.h"
#include "utility/helper.h"
#include "LittleFS.h"

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
    bleKeyboard.print(password);
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
    menuItems = {
      {"Password", [this]() {
        this->loadPasswords();
        this->selected = 0; // Reset selection
        this->currentState = STATE_PASSWORD;
        this->needsRedraw = true;
      }},
      {"Back", []() { Router::setScreen(new MainMenuScreen()); }},
    };
  } else if (currentState == STATE_PASSWORD) {
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
        this->needsRedraw = true;
      }
    });
  }

  BluetoothKeyboardScreen::renderMenu();
}

void BluetoothKeyboardScreen::handleInput() {
  bool bleConnected = bleKeyboard.isConnected();
  if (this->isConnected != bleConnected) {
    this->needsRedraw = true;
    this->isConnected = bleConnected;
  }

  MenuManager::handleInput();
}

void BluetoothKeyboardScreen::destroy() {
  // Clean up resources if necessary
  StickCP2.Display.fillRect(StickCP2.Display.width() / 2 - 9, 5, 17, 8, TFT_BLACK);
  this->isConnected = false;
  bleKeyboard.end();
}