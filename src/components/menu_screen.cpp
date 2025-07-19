#include "components/menu_screen.h"
#include "M5StickCPlus2.h"

bool MenuScreen::needsRedraw() {
  if (this->triggerRedraw) {
    this->triggerRedraw = false;
    return true;
  }

  return false;
}

void MenuScreen::redrawScreen(bool clear) {
  if (clear) {
    StickCP2.Display.fillRect(0, 13, StickCP2.Display.width(), StickCP2.Display.height(), TFT_BLACK);
  }
  
  this->triggerRedraw = true;
}