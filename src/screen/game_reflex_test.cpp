#include <M5StickCPlus2.h>

#include "screen/game_reflex_test.h"
#include "screen/game_menu.h"

#include "components/router.h"
#include "globals.h"

ReflexTestGameScreen::ReflexTestGameScreen() {
  this->state = MAIN_MENU;
  this->waitingForRelease = false;
}

void ReflexTestGameScreen::setBackgroundColor(uint16_t color) {
  StickCP2.Display.fillRect(0, 18, StickCP2.Display.width(), StickCP2.Display.height() - 18, color);
}

void ReflexTestGameScreen::startTest() {
  this->greenTime = millis() + random(1000, 3000);
  this->waitingForRelease = true;
  this->changeScreen(THE_GAME);
}

void ReflexTestGameScreen::updateScreen() {
  if (state == MAIN_MENU) {
    setBackgroundColor(TFT_BLACK);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    StickCP2.Display.setTextSize(2);
    StickCP2.Display.drawCenterString("Reflex", StickCP2.Display.width() / 2, 30);

    StickCP2.Display.setTextColor(selectedMenu == 0 ? SELECTED_COLOR : TFT_WHITE, TFT_BLACK);
    StickCP2.Display.drawCenterString("Play", StickCP2.Display.width() / 2, 70);

    StickCP2.Display.setTextColor(selectedMenu == 1 ? SELECTED_COLOR : TFT_WHITE, TFT_BLACK);
    StickCP2.Display.drawCenterString("Quit", StickCP2.Display.width() / 2, 100);

    StickCP2.Display.setTextSize(1);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    StickCP2.Display.drawCenterString("Wait for green, then", StickCP2.Display.width() / 2, 240 - 21);
    StickCP2.Display.drawCenterString("press encoder!", StickCP2.Display.width() / 2, 240 - 13);
  } else if (state == THE_GAME) {  
    setBackgroundColor(TFT_BLUE);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLUE);
    StickCP2.Display.setTextSize(2);
    StickCP2.Display.drawCenterString("WAIT THE", StickCP2.Display.width() / 2, 30);
    StickCP2.Display.drawCenterString("GREEN", StickCP2.Display.width() / 2, 48);
  } else if (state == TOO_SOON) {
    setBackgroundColor(TFT_RED);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_RED);
    StickCP2.Display.setTextSize(2);
    StickCP2.Display.drawCenterString("Too Soon!", StickCP2.Display.width() / 2, 60);
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.drawCenterString("Press encoder to retry", StickCP2.Display.width() / 2, 110);
  } else if (state == PRESS_TIME) {
    setBackgroundColor(TFT_DARKGREEN);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_DARKGREEN);
    StickCP2.Display.setTextSize(2);
    StickCP2.Display.drawCenterString("PRESS!", StickCP2.Display.width() / 2, StickCP2.Display.height() / 2 - 8);
  } else if (state == SHOW_RESULT) {
    setBackgroundColor(TFT_BLACK);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    StickCP2.Display.setTextSize(2);
    StickCP2.Display.drawCenterString("Your time:", StickCP2.Display.width() / 2, 30);
    StickCP2.Display.setTextSize(3);
    char buf[16];
    sprintf(buf, "%lu ms", pressTime - greenTime);
    StickCP2.Display.drawCenterString(buf, StickCP2.Display.width() / 2, 60);
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.drawCenterString("Press to menu", StickCP2.Display.width() / 2, 110);
  } 
}

void ReflexTestGameScreen::handleInput() {
  if (state == MAIN_MENU) {
    // Use encoder left/right or up/down to change selection
    if (encoder.movedLeft() || encoder.movedRight()) {
      selectedMenu = 1 - selectedMenu; // Toggle between 0 and 1
      this->needsRedraw = true;
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