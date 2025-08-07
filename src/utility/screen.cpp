#include <M5Unified.h>
#include "utility/screen.h"
#include "globals.h"
#include <BleCombo.h>

void Screen::drawHeader()
{
  int internalPercent = 0;
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(5, 5);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.printf("[%3d%%] ", M5.Power.getBatteryLevel());
  bleDevice.setBatteryLevel(M5.Power.getBatteryLevel());

  // Restore text color if you print more after this
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
}

void Screen::drawTimeHeader()
{
  if (M5.Rtc.isEnabled())
  {
    static char lastTimeStr[6] = {0};
    char timeStr[6];
    auto time = M5.Rtc.getTime();
    sprintf(timeStr, "%02d:%02d", time.hours, time.minutes);
    if (strcmp(timeStr, lastTimeStr) == 0) return;

    drawHeader();
    M5.Lcd.setTextSize(1);
    M5.Lcd.fillRect(M5.Lcd.width() - 40, 5, 40, 8, TFT_BLACK);
    M5.Lcd.drawRightString(timeStr, M5.Lcd.width() - 5, 5);
    strcpy(lastTimeStr, timeStr);
  }
}