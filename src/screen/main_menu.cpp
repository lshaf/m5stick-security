#include "screen/main_menu.h"
#include "screen/bt_keyboard.h"
#include "screen/bt_mouse.h"
#include "screen/game_menu.h"
#include "screen/setting_menu.h"
#include "components/router.h"

MainMenuScreen::MainMenuScreen() {
    this->title = "Main Menu";
    menuItems = {
      {"BLE Keyboard", []() { Router::setScreen(new BluetoothKeyboardScreen()); }},
      {"BLE Mouse", []() { Router::setScreen(new BluetoothMouseScreen()); }},
      {"Game", []() { Router::setScreen(new GameMenuScreen()); }},
      {"Settings", []() { Router::setScreen(new SettingMenuScreen()); }},
      {"Power Off", []() { StickCP2.Power.powerOff(); }},
    };
}