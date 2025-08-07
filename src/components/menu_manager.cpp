#include "components/menu_manager.h"
#include "globals.h"
#include <M5Unified.h>

MenuManager::MenuManager() {}

void MenuManager::updateScreen() {
  renderMenu();
}

void MenuManager::renderMenu() {
  M5.Lcd.setTextSize(1);

  auto marginTop = this->menuY;
  if (!this->title.isEmpty()) {
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawCenterString(this->title, M5.Lcd.width() / 2, marginTop);
    marginTop += M5.Lcd.fontHeight() + 5; // Add some space after the title
  }
  auto fh = M5.Lcd.fontHeight();
  auto sw = M5.Lcd.width();
  auto blockHeight = fh + 6;
  for (size_t i = 0; i < menuItems.size(); ++i) {
    M5.Lcd.setCursor(10, (marginTop + i * blockHeight) + 3);
    if ((int)i == selected) {
      M5.Lcd.setTextColor(TFT_BLACK, SELECTED_COLOR);
      M5.Lcd.fillRect(5, marginTop + i * blockHeight, sw - 10, blockHeight, SELECTED_COLOR);
    } else {
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    M5.Lcd.print(menuItems[i].label);
  }
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
}

void MenuManager::handleInput() {
  if (encoder.movedRight()) selected++;
  if (encoder.movedLeft()) selected--;
  if (selected < 0) selected = menuItems.size() - 1;
  if (selected >= (int) menuItems.size()) selected = 0;

  if (encoder.wasMoved()) this->redrawScreen();
  if (encoder.wasPressed()) menuItems[selected].onSelect();
}