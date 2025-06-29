#pragma once
#include "menu_screen.h"

class Router {
public:
    static void setScreen(MenuScreen* screen);
    static void handleMenu();
    static void destroyScreen();

private:
    static MenuScreen* currentScreen;
    static void refreshScreen();
};