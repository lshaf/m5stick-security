#include "screen/game_menu.h"
#include "components/router.h"
#include "screen/game_reflex_test.h"
#include "screen/main_menu.h"

GameMenuScreen::GameMenuScreen() {
    this->title = "Game";
    menuItems = {
        {"Reflex Test", []() { Router::setScreen(new ReflexTestGameScreen()); }},
        {"Back", []() { Router::setScreen(new MainMenuScreen()); }},
    };
}