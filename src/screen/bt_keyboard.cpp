#include "globals.h"
#include "screen/bt_keyboard.h"
#include "components/router.h"
#include "screen/main_menu.h"
#include "components/menu_manager.h"
#include "utility/helper.h"

void BluetoothKeyboardScreen::updateScreen() {
    // Update the screen with the current state of the Bluetooth keyboard
    StickCP2.Display.setCursor(5, 18);
    StickCP2.Display.setTextColor(TFT_WHITE);
    String statusText = this->isConnected ? "Connected" : "Disconnected";
    int statusColor = this->isConnected ? TFT_DARKGREEN : TFT_RED;

    StickCP2.Display.fillRect(5, 18, StickCP2.Display.width() - 10, 20, statusColor);
    StickCP2.Display.drawCenterString("BLE: " + statusText, StickCP2.Display.width() / 2, 24);

    menuItems = {
        {"BitWarden", [this]() { 
            if (this->isConnected) {
                bleKeyboard.print("password");
                Helper::showAlert("DONE");
            } else {
                Helper::showAlert("BLE not connected");
            }
        }},
        {"Back", []() { Router::setScreen(new MainMenuScreen()); }},
    };

    this->menuY = 43;
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
    this->isConnected = false;
    bleKeyboard.end();
}