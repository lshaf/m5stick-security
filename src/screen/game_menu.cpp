#include "screen/game_menu.h"
#include "components/router.h"
#include "screen/game_reflex_test.h"
#include "screen/game_reflex_sec.h"
#include "screen/main_menu.h"

GameMenuScreen::GameMenuScreen() {
    this->title = "Game";
    menuItems = {
        {"Reflex Test", []() { Router::setScreen(new ReflexTestGameScreen()); }},
        {"Reflex Second", []() { Router::setScreen(new ReflexSecondGameScreen()); }},
        {"Back", []() { Router::setScreen(new MainMenuScreen()); }},
    };
}