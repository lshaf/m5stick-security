#pragma once
#include <Arduino.h>
#include "components/menu_screen.h"

class ReflexTestGameScreen : public MenuScreen {
public:
  ReflexTestGameScreen();
  void updateScreen() override;
  void handleInput() override;
  void destroy() override;

private:
  enum State {
    MAIN_MENU,
    TOO_SOON,
    THE_GAME,
    PRESS_TIME,
    SHOW_RESULT
  } state = MAIN_MENU;

  unsigned long greenTime = 0;
  unsigned long pressTime = 0;
  bool waitingForRelease = false;

  uint8_t selectedMenu = 0;

  void startTest();
  void setBackgroundColor(uint16_t color);
  void changeScreen(State newState) {
    this->state = newState;
    this->needsRedraw = true;
  }
};