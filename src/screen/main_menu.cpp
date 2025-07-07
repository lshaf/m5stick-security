#include "screen/main_menu.h"
#include "screen/input_name.h"
#include "screen/bt_keyboard.h"
#include "screen/game_menu.h"
#include "components/router.h"

MainMenuScreen::MainMenuScreen() {
    this->title = "Main Menu";
    menuItems = {
      {"Input Name", []() { Router::setScreen(new InputNameScreen()); }},
      {"BLE Keyboard", []() { Router::setScreen(new BluetoothKeyboardScreen()); }},
      {"Game", []() { Router::setScreen(new GameMenuScreen()); }},
      {"Power Off", []() { StickCP2.Power.powerOff(); }},
    };
}