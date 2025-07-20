#include "screen/lock.h"
#include "screen/main_menu.h"

#include "globals.h"
#include "utility/helper.h"
#include "components/router.h"
#include <M5StickCPlus2.h>

LockScreen::LockScreen() {}

void LockScreen::updateScreen() {
  auto& d = StickCP2.Display;

  // Draw lock body (rectangle)
  int centerX = d.width() / 2;
  int centerY = d.height() / 2 + 10;
  int bodyWidth = 53;
  int bodyHeight = 40;
  int bodyX = centerX - bodyWidth / 2;
  int bodyY = centerY - bodyHeight / 2;

  d.fillRoundRect(bodyX, bodyY, bodyWidth, bodyHeight, 4, SELECTED_COLOR);

  // Draw shackle (arc)
  int shackleRadius = 24;
  int shackleThickness = 6;
  int shackleY = bodyY;
  d.fillArc(centerX, shackleY, shackleRadius, shackleRadius - shackleThickness, 180, 360, SELECTED_COLOR);

  // Draw keyhole
  int keyholeX = centerX;
  int keyholeY = centerY;
  d.fillCircle(keyholeX, keyholeY, 3, TFT_BLACK);
  d.fillRect(keyholeX - 1, keyholeY, 3, 8, TFT_BLACK);
}

void LockScreen::handleInput() {
  bool res = finger.available();
  if (res == ACK_SUCCESS) {
    Helper::showAlert("Finger recognized");
    Router::setScreen(new MainMenuScreen());
  } else if (res == ACK_NOUSER) {
    Helper::showAlert("No user found");
    Router::setScreen(new MainMenuScreen());
  } else if (finger.getFingerPermission() == 0x05) {
    Helper::showAlert("Finger not recognized");
  }

  delay(1000);
}

void LockScreen::destroy() {
  // finger.sleep();
}