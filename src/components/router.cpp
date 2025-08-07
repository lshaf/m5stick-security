#include "components/router.h"

MenuScreen* Router::currentScreen = nullptr;

void Router::setScreen(MenuScreen* screen) {
  if (currentScreen) currentScreen->destroy();
  currentScreen = screen;
  currentScreen->redrawScreen();
  Router::refreshScreen();
}

void Router::refreshScreen() {
  if (currentScreen && currentScreen->needsRedraw()) {
    currentScreen->updateScreen();
  }
}

void Router::handleMenu() {
  if (currentScreen) {
    currentScreen->handleInput();
    Router::refreshScreen();
  }
}

void Router::destroyScreen() {
  if (currentScreen) {
    currentScreen->destroy();
    currentScreen = nullptr;
  }
}