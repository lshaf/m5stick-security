#pragma once
#include <Arduino.h>
#include <M5Unified.h>

#include "components/menu_screen.h"

class ReflexSecondGameScreen : public MenuScreen {
public:
  struct GameData {
    String printedTime;
    unsigned int elapsed;
  };

  ReflexSecondGameScreen();
  void updateScreen() override;
  void handleInput() override;
  void destroy() override;

private:
  enum State {
    MAIN_MENU,
    THE_GAME,
    SHOW_RESULT
  } state = MAIN_MENU;

  unsigned long gameStartTime = 0;
  unsigned long pressTime = 0;
  uint8_t selectedMenu = 0;
  String resultMessage = "";
  uint16_t resultColor = TFT_WHITE;

  void startGame();
  void calculateResult();
  GameData timeCalculator(unsigned long time);
  String getResultMessage(int differenceMs);
  uint16_t getResultColor(int differenceMs);
  void changeScreen(State newState) {
    this->state = newState;
    this->redrawScreen();
  }
};