#include <M5Unified.h>

#include "screen/game_reflex_test.h"
#include "screen/game_menu.h"

#include "components/router.h"
#include "globals.h"

ReflexTestGameScreen::ReflexTestGameScreen() {
  this->state = MAIN_MENU;
  this->waitingForRelease = false;
}

void ReflexTestGameScreen::setBackgroundColor(uint16_t color) {
  M5.Lcd.fillRect(0, 18, M5.Lcd.width(), M5.Lcd.height() - 18, color);
}

void ReflexTestGameScreen::startTest() {
  this->greenTime = millis() + random(1000, 3000);
  this->waitingForRelease = true;
  this->changeScreen(THE_GAME);
}

void ReflexTestGameScreen::updateScreen() {
  if (state == MAIN_MENU) {
    setBackgroundColor(TFT_BLACK);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawCenterString("Reflex", M5.Lcd.width() / 2, 30);

    M5.Lcd.setTextColor(selectedMenu == 0 ? SELECTED_COLOR : TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString("Play", M5.Lcd.width() / 2, 70);

    M5.Lcd.setTextColor(selectedMenu == 1 ? SELECTED_COLOR : TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString("Quit", M5.Lcd.width() / 2, 100);

    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString("Wait for green, then", M5.Lcd.width() / 2, 240 - 21);
    M5.Lcd.drawCenterString("press encoder!", M5.Lcd.width() / 2, 240 - 13);
  } else if (state == THE_GAME) {  
    setBackgroundColor(TFT_BLUE);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawCenterString("WAIT THE", M5.Lcd.width() / 2, 30);
    M5.Lcd.drawCenterString("GREEN", M5.Lcd.width() / 2, 48);
  } else if (state == TOO_SOON) {
    setBackgroundColor(TFT_RED);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_RED);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawCenterString("Too Soon!", M5.Lcd.width() / 2, 60);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawCenterString("Press encoder to retry", M5.Lcd.width() / 2, 110);
  } else if (state == PRESS_TIME) {
    setBackgroundColor(TFT_DARKGREEN);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_DARKGREEN);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawCenterString("PRESS!", M5.Lcd.width() / 2, M5.Lcd.height() / 2 - 8);
  } else if (state == SHOW_RESULT) {
    setBackgroundColor(TFT_BLACK);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawCenterString("Your time:", M5.Lcd.width() / 2, 30);
    M5.Lcd.setTextSize(3);
    char buf[16];
    sprintf(buf, "%lu ms", pressTime - greenTime);
    M5.Lcd.drawCenterString(buf, M5.Lcd.width() / 2, 60);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawCenterString("Press to menu", M5.Lcd.width() / 2, 110);
  } 
}

void ReflexTestGameScreen::handleInput() {
  if (state == MAIN_MENU) {
    // Use encoder left/right or up/down to change selection
    if (encoder.movedLeft() || encoder.movedRight()) {
      selectedMenu = 1 - selectedMenu; // Toggle between 0 and 1
      this->redrawScreen();
    }
    if (encoder.wasPressed()) {
      if (selectedMenu == 0) {
        startTest();
      } else if (selectedMenu == 1) {
        Router::setScreen(new GameMenuScreen());
      }
    }
    return;
  } else if (state == THE_GAME) {
    if (millis() >= greenTime) {
      this->changeScreen(PRESS_TIME);
    } else {
      if (encoder.wasPressed()) this->changeScreen(TOO_SOON);
    }
  } else if (state == TOO_SOON) {
    if (encoder.wasPressed()) this->changeScreen(MAIN_MENU);
  } else if (state == PRESS_TIME) {
    if (encoder.wasPressed()) {
      pressTime = millis();
      this->changeScreen(SHOW_RESULT);
    }
  } else if (state == SHOW_RESULT) {
    if (encoder.wasPressed()) this->changeScreen(MAIN_MENU);
  }
}

void ReflexTestGameScreen::destroy() {}