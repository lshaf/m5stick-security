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
  this->bleMouse = BleMouse(config.getBleName().c_str(), "Espressif", 100);
  this->bleMouse.begin();
}

void BluetoothMouseScreen::updateScreen() {
  // Update the screen with the current state of the Bluetooth mouse
  StickCP2.Display.setTextColor(this->isConnected ? TFT_GREEN : TFT_RED, TFT_BLACK);
  StickCP2.Display.drawCenterString("BLE", StickCP2.Display.width() / 2, 5);

  if (currentState == STATE_MAIN) {
    this->title = "BLE Mouse";
    menuItems = {
      {"Tachiyomi", [this]() {
        this->selected = 0; // Reset selection
        this->currentState = STATE_TACHIYOMI;
        this->needsRedraw = true;
      }},
      {"Back", []() { Router::setScreen(new MainMenuScreen()); }},
    };
  } else if (currentState == STATE_TACHIYOMI) {
    this->title = "Tachiyomi";
    menuItems.clear();
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.drawCenterString(
      "Hold Encoder to Quit", 
      StickCP2.Display.width() / 2, 
      StickCP2.Display.height() - 8
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

  BluetoothMouseScreen::renderMenu();
}

void BluetoothMouseScreen::handleInput() {
  bool bleConnected = this->bleMouse.isConnected();
  if (this->isConnected != bleConnected) {
    this->needsRedraw = true;
    this->isConnected = bleConnected;
  }

  if (this->currentState == STATE_MAIN) {
    MenuManager::handleInput();
  } else if (this->currentState == STATE_TACHIYOMI) {
    static u_long lastActivityTime = 0;
    if (encoder.wasLongPressed()) {
      this->currentState = STATE_MAIN;
      this->needsRedraw = true;
    } else if (encoder.movedRight()) {
      this->bleMouse.move(0, 0, -1);
      this->lastActivity = ACT_SCRL_DOWN;
      lastActivityTime = millis();
    } else if (encoder.movedLeft()) {
      this->bleMouse.move(0, 0, 1);
      this->lastActivity = ACT_SCRL_UP;
      lastActivityTime = millis();
    }

    if (encoder.isMoved()) this->needsRedraw = true;
    if (millis() - lastActivityTime > 500 && this->lastActivity != ACT_NONE) {
      this->lastActivity = ACT_NONE;
      this->needsRedraw = true;
    }
  }
}

void BluetoothMouseScreen::destroy() {
  // Clean up resources if necessary
  StickCP2.Display.fillRect(StickCP2.Display.width() / 2 - 9, 5, 17, 8, TFT_BLACK);
  this->isConnected = false;
  this->bleMouse.end();
}