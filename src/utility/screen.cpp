#include "M5StickCPlus2.h"
#include "utility/screen.h"
#include "globals.h"
#include <BleCombo.h>

void Screen::drawHeader()
{
  int internalPercent = 0;
  StickCP2.Display.setTextSize(1);
  StickCP2.Display.setCursor(5, 5);
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  StickCP2.Display.printf("[%3d%%] ", StickCP2.Power.getBatteryLevel());
  bleDevice.setBatteryLevel(StickCP2.Power.getBatteryLevel());

  // Restore text color if you print more after this
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
}

void Screen::drawTimeHeader()
{
  if (StickCP2.Rtc.isEnabled())
  {
    static char lastTimeStr[6] = {0};
    char timeStr[6];
    auto time = StickCP2.Rtc.getTime();
    sprintf(timeStr, "%02d:%02d", time.hours, time.minutes);
    if (strcmp(timeStr, lastTimeStr) == 0) return;

    drawHeader();
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.fillRect(StickCP2.Display.width() - 40, 5, 40, 8, TFT_BLACK);
    StickCP2.Display.drawRightString(timeStr, StickCP2.Display.width() - 5, 5);
    strcpy(lastTimeStr, timeStr);
  }
}