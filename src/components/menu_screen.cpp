#include "components/menu_screen.h"
#include <M5Unified.h>

bool MenuScreen::needsRedraw() {
  if (this->triggerRedraw) {
    this->triggerRedraw = false;
    return true;
  }

  return false;
}

void MenuScreen::redrawScreen(bool clear) {
  if (clear) {
    M5.Lcd.fillRect(0, 13, M5.Lcd.width(), M5.Lcd.height(), TFT_BLACK);
  }
  
  this->triggerRedraw = true;
}