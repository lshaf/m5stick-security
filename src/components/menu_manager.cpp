#include "components/menu_manager.h"
#include "globals.h"

MenuManager::MenuManager() {}

void MenuManager::updateScreen() {
  StickCP2.Display.setTextSize(1);

  auto fh = StickCP2.Display.fontHeight();
  auto sw = StickCP2.Display.width();
  for (size_t i = 0; i < menuItems.size(); ++i) {
    StickCP2.Display.setCursor(10, (18 + i * (fh + 4)) + 2);
    if ((int)i == selected) {
      StickCP2.Display.setTextColor(TFT_BLACK, TFT_YELLOW);
      StickCP2.Display.fillRect(5, 18 + i * (fh + 4), sw - 10, fh + 4, TFT_YELLOW);
    } else {
      StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    StickCP2.Display.print(menuItems[i].label);
  }
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
}

void MenuManager::handleInput() {
  if (encoder.movedRight()) selected++;
  if (encoder.movedLeft()) selected--;
  if (selected < 0) selected = menuItems.size() - 1;
  if (selected >= (int) menuItems.size()) selected = 0;

  if (encoder.isMoved()) {
    this->needsRedraw = true;
  }

  if (encoder.wasPressed()) { // Button pressed
    menuItems[selected].onSelect();
  }
}