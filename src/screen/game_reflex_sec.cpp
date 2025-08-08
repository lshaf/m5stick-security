#include "screen/game_reflex_sec.h"
#include "screen/game_menu.h"

#include "components/router.h"
#include "globals.h"

ReflexSecondGameScreen::ReflexSecondGameScreen() {
  this->state = MAIN_MENU;
  M5.Lcd.fillRect(0, 18, M5.Lcd.width(), M5.Lcd.height() - 18, TFT_BLACK);
}

void ReflexSecondGameScreen::startGame() {
  this->gameStartTime = millis();
  this->changeScreen(THE_GAME);
}

ReflexSecondGameScreen::GameData ReflexSecondGameScreen::timeCalculator(unsigned long time) {
  unsigned long elapsed = time - gameStartTime;
  unsigned long seconds = elapsed / 1000;
  unsigned long milliseconds = elapsed % 1000;

  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%lu.%04lu", seconds, milliseconds);

  GameData data;
  data.printedTime = String(buffer);
  data.elapsed = elapsed;
  return data;
}

void ReflexSecondGameScreen::updateScreen() {
  if (state == MAIN_MENU) {
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawCenterString("Reflex Sec", M5.Lcd.width() / 2, 30);

    M5.Lcd.setTextColor(selectedMenu == 0 ? SELECTED_COLOR : TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString("Play", M5.Lcd.width() / 2, 70);

    M5.Lcd.setTextColor(selectedMenu == 1 ? SELECTED_COLOR : TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString("Quit", M5.Lcd.width() / 2, 100);

    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString("Press when timer hits", M5.Lcd.width() / 2, 240 - 21);
    M5.Lcd.drawCenterString("exactly 5.000 seconds!", M5.Lcd.width() / 2, 240 - 13);
  } else if (state == THE_GAME) {
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawCenterString("Timer", M5.Lcd.width() / 2, 30);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawCenterString("Press at 5.000!", M5.Lcd.width() / 2, M5.Lcd.height() - 13);
  } else if (state == SHOW_RESULT) {
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawCenterString("Result", M5.Lcd.width() / 2, 30);
    
    M5.Lcd.setTextSize(3);
    auto calculator = timeCalculator(pressTime);
    M5.Lcd.setTextColor(resultColor, TFT_BLACK);
    M5.Lcd.drawCenterString(calculator.printedTime, M5.Lcd.width() / 2, M5.Lcd.height() / 2 - M5.Lcd.fontHeight() / 2);
    
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawCenterString(resultMessage, M5.Lcd.width() / 2, M5.Lcd.height() / 4 * 3 - M5.Lcd.fontHeight() / 2);

    char differenceStr[16];
    int difference = calculator.elapsed - 5000;
    sprintf(differenceStr, "Diff: %d ms", difference);
    M5.Lcd.drawCenterString(differenceStr, M5.Lcd.width() / 2, M5.Lcd.height() / 4 * 3 - M5.Lcd.fontHeight() / 2 - 9);

    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString("Press to menu", M5.Lcd.width() / 2, M5.Lcd.height() - 13);
  }
}

void ReflexSecondGameScreen::handleInput() {
  if (state == MAIN_MENU) {
    if (encoder.movedLeft() || encoder.movedRight()) {
      selectedMenu = 1 - selectedMenu; // Toggle between 0 and 1
      this->redrawScreen(false);
    }
    if (encoder.wasPressed()) {
      if (selectedMenu == 0) {
        startGame();
      } else if (selectedMenu == 1) {
        Router::setScreen(new GameMenuScreen());
      }
    }
    return;
  } else if (state == THE_GAME) {
    M5.Lcd.setTextSize(3);
    auto calculator = timeCalculator(millis());
    M5.Lcd.drawCenterString(calculator.printedTime, M5.Lcd.width() / 2, M5.Lcd.height() / 2 - M5.Lcd.fontHeight() / 2);

    if (encoder.wasPressed() || calculator.elapsed >= 7000) {
      this->pressTime = millis();
      calculateResult();
      this->changeScreen(SHOW_RESULT);
    }
  } else if (state == SHOW_RESULT) {
    if (encoder.wasPressed()) {
      this->changeScreen(MAIN_MENU);
    }
  }
}

void ReflexSecondGameScreen::calculateResult() {
  int difference = this->pressTime - this->gameStartTime - 5000;
  resultMessage = getResultMessage(difference);
  resultColor = getResultColor(difference);
}

String ReflexSecondGameScreen::getResultMessage(int differenceMs) {
  if (differenceMs < 0) {
    return "Too early!";
  } else if (differenceMs <= 100) {
    return "Are you superhero?";
  } else if (differenceMs <= 300) {
    return "Fast enough";
  } else if (differenceMs <= 700) {
    return "Average Person";
  } else if (differenceMs <= 1000) {
    return "I think you're too old for this";
  } else {
    return "Ngelamun";
  }
}

uint16_t ReflexSecondGameScreen::getResultColor(int differenceMs) {
  if (differenceMs < 0) {
    return TFT_RED;
  } else if (differenceMs <= 100) {
    return TFT_GREEN;
  } else if (differenceMs <= 300) {
    return TFT_CYAN;
  } else if (differenceMs <= 700) {
    return TFT_YELLOW;
  } else if (differenceMs <= 1000) {
    return TFT_ORANGE;
  } else {
    return TFT_RED;
  }
}

void ReflexSecondGameScreen::destroy() {}