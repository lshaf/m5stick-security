#include "components/menu_manager.h"
#include "globals.h"

MenuManager::MenuManager() {}

void MenuManager::updateScreen() {
  renderMenu();
}

void MenuManager::renderMenu() {
  StickCP2.Display.setTextSize(1);

  auto marginTop = this->menuY;
  if (!this->title.isEmpty()) {
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    StickCP2.Display.drawCenterString(this->title, StickCP2.Display.width() / 2, marginTop);
    marginTop += StickCP2.Display.fontHeight() + 5; // Add some space after the title
  }
  auto fh = StickCP2.Display.fontHeight();
  auto sw = StickCP2.Display.width();
  auto blockHeight = fh + 6;
  for (size_t i = 0; i < menuItems.size(); ++i) {
    StickCP2.Display.setCursor(10, (marginTop + i * blockHeight) + 3);
    if ((int)i == selected) {
      StickCP2.Display.setTextColor(TFT_BLACK, SELECTED_COLOR);
      StickCP2.Display.fillRect(5, marginTop + i * blockHeight, sw - 10, blockHeight, SELECTED_COLOR);
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