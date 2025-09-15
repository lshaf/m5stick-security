#include "globals.h"
#include "utility/helper.h"

#include "screen/bluetooth.h"
#include "screen/main_menu.h"

#include "components/router.h"
#include "components/menu_manager.h"

#include <BleKeyboard.h>
#include <BleMouse.h>
#include <SPIFFS.h>
#include <M5Unified.h>

BluetoothScreen::BluetoothScreen() {
  this->isConnected = false;
  this->currentState = STATE_MAIN;

  Keyboard.begin();
  Mouse.begin();
}

void BluetoothScreen::loadSavedPasswords() {
  File file = SPIFFS.open("/meta_data.ps", "r");
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

BluetoothScreen::UpdateImuData BluetoothScreen::dotPosition() {
  UpdateImuData result;
  currentImuData.previous = currentImuData.current;
  M5.Imu.getAccelData(&currentImuData.current.x, &currentImuData.current.y, &currentImuData.current.z);

  auto boxSize = M5.Lcd.width() - 10;
  auto centerXBox = 5 + boxSize / 2;
  auto centerYBox = 32 + boxSize / 2;

  result.previous.x = centerXBox + ((currentImuData.previous.x * -1) * (boxSize / 2));
  result.previous.y = centerYBox + (currentImuData.previous.y * (boxSize / 2));
  result.current.x = centerXBox + ((currentImuData.current.x * -1) * (boxSize / 2));
  result.current.y = centerYBox + (currentImuData.current.y * (boxSize / 2));

  auto moveX = (currentImuData.current.x * -1) * this->acceleration;
  auto moveY = (currentImuData.current.y * -1) * this->acceleration;
  Mouse.move(moveX, moveY, 0);

  return result;
}

void BluetoothScreen::sendPasswordToDevice(const String& password) {
  if (this->isConnected) {
    Keyboard.print(password);
    Helper::showAlert("Password sent");
  } else {
    Helper::showAlert("BLE not connected");
  }
}

void BluetoothScreen::updateScreen() {
  // Update the screen with the current state of the Bluetooth keyboard
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(this->isConnected ? TFT_GREEN : TFT_RED, TFT_BLACK);
  M5.Lcd.drawCenterString("BLE", M5.Lcd.width() / 2, 5);

  if (currentState == STATE_MAIN) {
    this->title = "Bluetooth";
    menuItems = {
      {"Password", [this]() {
        this->loadSavedPasswords();
        this->selected = 0; // Reset selection
        this->currentState = STATE_PASSWORD;
        this->redrawScreen();
      }},
      {"Tachiyomi", [this]() {
        this->selected = 0; // Reset selection
        this->tachiyomiMode = TACHIYOMI_VOLUME;
        this->currentState = STATE_TACHIYOMI;
        this->redrawScreen();
      }},
      {"Mouse", [this]() {
        this->selected = 0; // Reset selection
        this->currentState = STATE_MOUSE;
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
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.fillRect(0, 28, M5.Lcd.width(), 8, TFT_BLACK);
    M5.Lcd.drawCenterString(
      this->tachiyomiMode == TACHIYOMI_VOLUME ? "Volume Mode" : "Scroll Mode",
      M5.Lcd.width() / 2, 28
    );
    M5.Lcd.drawCenterString(
      "Hold Encoder to Quit", 
      M5.Lcd.width() / 2, 
      M5.Lcd.height() - 13
    );

    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(this->lastActivity == ACT_SCRL_UP ? SELECTED_COLOR : TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString(
      "UP", 
      M5.Lcd.width() / 2, 
      70
    );
    M5.Lcd.setTextColor(this->lastActivity == ACT_SCRL_DOWN ? SELECTED_COLOR : TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString(
      "DOWN", 
      M5.Lcd.width() / 2, 
      137
    );
  } else if (currentState == STATE_MOUSE) {
    this->title = "Mouse";
    menuItems.clear();
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString(
      "Hold Encoder to Quit", 
      M5.Lcd.width() / 2, 
      M5.Lcd.height() - 13
    );
  }

  BluetoothScreen::renderMenu();
}

void BluetoothScreen::handleInput() {
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
      if (this->tachiyomiMode == TACHIYOMI_VOLUME) {
        Keyboard.press(KEY_MEDIA_VOLUME_DOWN);
        Keyboard.release(KEY_MEDIA_VOLUME_DOWN);
      } else {
        Mouse.move(0, 0, -1);
      }
      this->lastActivity = ACT_SCRL_DOWN;
      lastActivityTime = millis();
    } else if (encoder.movedLeft()) {
      if (this->tachiyomiMode == TACHIYOMI_VOLUME) {
        Keyboard.press(KEY_MEDIA_VOLUME_UP);
        Keyboard.release(KEY_MEDIA_VOLUME_UP);
      } else {
        Mouse.move(0, 0, 1);
      }
      this->lastActivity = ACT_SCRL_UP;
      lastActivityTime = millis();
    }

    if (encoder.wasMoved()) this->redrawScreen(false);
    if (M5.BtnA.wasPressed()) {
      this->tachiyomiMode = (this->tachiyomiMode == TACHIYOMI_VOLUME) ? TACHIYOMI_SCROLL : TACHIYOMI_VOLUME;
      this->redrawScreen(false);
    }
    if (millis() - lastActivityTime > 500 && this->lastActivity != ACT_NONE) {
      this->lastActivity = ACT_NONE;
      this->redrawScreen(false);
    }
  } else if (this->currentState == STATE_MOUSE) {
    // Handle PC mouse mode input
    if (encoder.wasPressed()) {
      Mouse.click(MOUSE_LEFT);
    } else if (encoder.movedLeft()) {
      this->acceleration -= 2;
    } else if (encoder.movedRight()) {
      this->acceleration += 2;
    }

    if (M5.BtnA.wasPressed()) {
      Mouse.click(MOUSE_RIGHT);
    }

    if (encoder.wasMoved()) {
      if (this->acceleration <= 2) this->acceleration = 2;
      if (this->acceleration >= 60) this->acceleration = 60;
    }

    auto imu_update = M5.Imu.update();
    if (imu_update) {
      auto dot = this->dotPosition();
      M5.Lcd.fillCircle(dot.previous.x, dot.previous.y, 2, TFT_BLACK);
      M5.Lcd.drawRect(5, 32, M5.Lcd.width() - 10, M5.Lcd.width() - 10, SELECTED_COLOR);
      M5.Lcd.fillCircle(dot.current.x, dot.current.y, 2, SELECTED_COLOR);

      M5.Lcd.setTextSize(2);
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
      String printedAccel = (this->acceleration < 10 ? "0" : "") + String(this->acceleration);
      M5.Lcd.drawCenterString(printedAccel, M5.Lcd.width() / 2, 32 + M5.Lcd.width() + 5);
    }

    if (encoder.wasLongPressed()) {
      this->currentState = STATE_MAIN;
      this->redrawScreen();
    }
  } else {
    MenuManager::handleInput();
  }
}

void BluetoothScreen::destroy() {
  Keyboard.end();
  Mouse.end();
}