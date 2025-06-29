#include "Arduino.h"
#include <M5StickCPlus2.h>
#include "globals.h"

#include "screen/input_name.h"
#include "screen/main_menu.h"
#include "components/menu_screen.h"
#include "components/router.h"
#include "components/menu_manager.h"

void InputNameScreen::updateScreen() {
  StickCP2.Display.setTextSize(1);
  if (state == STATE_MAIN) {
    StickCP2.Display.drawCenterString("Input your name", StickCP2.Display.width() / 2, 18);
  } else if (state == STATE_RESULT) {
    StickCP2.Display.drawCenterString("Your name:", StickCP2.Display.width() / 2, 18);
    StickCP2.Display.setTextSize(2);
    StickCP2.Display.drawCenterString(this->inputName, StickCP2.Display.width() / 2, StickCP2.Display.height() / 2);
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.drawString("Press encoder to go", 10, StickCP2.Display.height() - 21);
    StickCP2.Display.drawString("back", 10, StickCP2.Display.height() - 13);
  }
}
void InputNameScreen::handleInput() {
  if (this->state == STATE_INPUT) {
    keypad.handleInput();
  } else {
    if (encoder.wasPressed()) {
      if (this->state == STATE_MAIN) {
        this->state = STATE_INPUT;
        keypad.setMessage("");
        keypad.setOnOk([this](const String& name) {
          this->inputName = name;
          this->needsRedraw = true;
          this->state = STATE_RESULT;
        });
        keypad.show("Input your name");
      } else if (this->state == STATE_RESULT) {
        Router::setScreen(new MainMenuScreen()); // Go back to main menu
      }
    }
  }
}
void InputNameScreen::destroy(){}