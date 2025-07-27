#include "globals.h"
#include "screen/bt_mouse.h"
#include "components/router.h"
#include "screen/main_menu.h"
#include "utility/helper.h"
#include <LittleFS.h>
#include <NimBLEDevice.h>

BluetoothMouseScreen::BluetoothMouseScreen() {
  this->isConnected = false;
  this->currentState = STATE_MAIN;
  this->bleMouse.begin();
}

void BluetoothMouseScreen::updateScreen() {
  // Update the screen with the current state of the Bluetooth mouse
  StickCP2.Display.setTextSize(1);
  StickCP2.Display.setTextColor(this->isConnected ? TFT_GREEN : TFT_RED, TFT_BLACK);
  StickCP2.Display.drawCenterString("BLE", StickCP2.Display.width() / 2, 5);

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
  } else if (currentState == STATE_PC) {
    this->title = "PC Mouse";
    menuItems.clear();
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    StickCP2.Display.drawCenterString(
      "Hold Encoder to Quit", 
      StickCP2.Display.width() / 2, 
      StickCP2.Display.height() - 13
    );
  }

  BluetoothMouseScreen::renderMenu();
}

BluetoothMouseScreen::UpdateImuData BluetoothMouseScreen::dotPosition() {
  UpdateImuData result;
  currentImuData.previous = currentImuData.current;
  StickCP2.Imu.getAccelData(&currentImuData.current.x, &currentImuData.current.y, &currentImuData.current.z);

  auto boxSize = StickCP2.Display.width() - 10;
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

    if (encoder.wasMoved()) {
      if (this->acceleration <= 2) this->acceleration = 2;
      if (this->acceleration >= 60) this->acceleration = 60;
    }

    auto imu_update = StickCP2.Imu.update();
    if (imu_update) {
      auto dot = this->dotPosition();
      StickCP2.Display.fillCircle(dot.previous.x, dot.previous.y, 2, TFT_BLACK);
      StickCP2.Display.drawRect(5, 32, StickCP2.Display.width() - 10, StickCP2.Display.width() - 10, SELECTED_COLOR);
      StickCP2.Display.fillCircle(dot.current.x, dot.current.y, 2, SELECTED_COLOR);

      StickCP2.Display.setTextSize(2);
      StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
      String printedAccel = (this->acceleration < 10 ? "0" : "") + String(this->acceleration);
      StickCP2.Display.drawCenterString(printedAccel, StickCP2.Display.width() / 2, 32 + StickCP2.Display.width() + 5);
    }

    if (encoder.wasLongPressed()) {
      this->currentState = STATE_MAIN;
      this->redrawScreen();
    }
  }
}

void BluetoothMouseScreen::destroy() {
  // Clean up resources if necessary
  StickCP2.Display.fillRect(StickCP2.Display.width() / 2 - 9, 5, 17, 8, TFT_BLACK);
  this->isConnected = false;
  // this->bleMouse.end();
}