#include "globals.h"
#include "screen/bt_mouse.h"
#include "components/router.h"
#include "screen/main_menu.h"
#include "utility/helper.h"
#include <NimBLEDevice.h>
#include <M5Unified.h>

BluetoothMouseScreen::BluetoothMouseScreen() {
  this->isConnected = false;
  this->currentState = STATE_MAIN;
  this->bleMouse.begin();
}

void BluetoothMouseScreen::updateScreen() {
  // Update the screen with the current state of the Bluetooth mouse
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(this->isConnected ? TFT_GREEN : TFT_RED, TFT_BLACK);
  M5.Lcd.drawCenterString("BLE", M5.Lcd.width() / 2, 5);

  if (currentState == STATE_MAIN) {
    this->title = "BLE Mouse";
    menuItems = {
      {"Tachiyomi", [this]() {
        this->selected = 0; // Reset selection
        this->currentState = STATE_TACHIYOMI;
        this->redrawScreen();
      }},
      {"PC Mouse", [this]() {
        this->selected = 0; // Reset selection
        this->currentState = STATE_PC;
        this->redrawScreen();
      }},
      {"Back", []() { Router::setScreen(new MainMenuScreen()); }},
    };
  } else if (currentState == STATE_TACHIYOMI) {
    this->title = "Tachiyomi";
    menuItems.clear();
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
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
  } else if (currentState == STATE_PC) {
    this->title = "PC Mouse";
    menuItems.clear();
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString(
      "Hold Encoder to Quit", 
      M5.Lcd.width() / 2, 
      M5.Lcd.height() - 13
    );
  }

  BluetoothMouseScreen::renderMenu();
}

BluetoothMouseScreen::UpdateImuData BluetoothMouseScreen::dotPosition() {
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
  auto moveY = currentImuData.current.y * this->acceleration;
  this->bleMouse.move(moveX, moveY, 0);

  return result;
}

void BluetoothMouseScreen::handleInput() {
  bool bleConnected = bleDevice.isConnected();
  if (this->isConnected != bleConnected) {
    this->redrawScreen(false);
    this->isConnected = bleConnected;
  }

  if (this->currentState == STATE_MAIN) {
    MenuManager::handleInput();
  } else if (this->currentState == STATE_TACHIYOMI) {
    static u_long lastActivityTime = 0;
    if (encoder.wasLongPressed()) {
      this->currentState = STATE_MAIN;
      this->redrawScreen();
    } else if (encoder.movedRight()) {
      this->bleMouse.move(0, 0, -1);
      this->lastActivity = ACT_SCRL_DOWN;
      lastActivityTime = millis();
    } else if (encoder.movedLeft()) {
      this->bleMouse.move(0, 0, 1);
      this->lastActivity = ACT_SCRL_UP;
      lastActivityTime = millis();
    }

    if (encoder.wasMoved()) this->redrawScreen(false);
    if (millis() - lastActivityTime > 500 && this->lastActivity != ACT_NONE) {
      this->lastActivity = ACT_NONE;
      this->redrawScreen(false);
    }
  } else if (this->currentState == STATE_PC) {
    // Handle PC mouse mode input
    if (encoder.wasPressed()) {
      this->bleMouse.click(MOUSE_LEFT);
    } else if (encoder.movedLeft()) {
      this->acceleration -= 2;
    } else if (encoder.movedRight()) {
      this->acceleration += 2;
    }

    if (M5.BtnA.wasPressed()) {
      this->bleMouse.click(MOUSE_RIGHT);
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
  }
}

void BluetoothMouseScreen::destroy() {
  // Clean up resources if necessary
  M5.Lcd.fillRect(M5.Lcd.width() / 2 - 9, 5, 17, 8, TFT_BLACK);
  this->isConnected = false;
  // this->bleMouse.end();
}
