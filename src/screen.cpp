#include "M5StickCPlus2.h"
#include "screen.h"

void Screen::init()
{
  StickCP2.begin();
  StickCP2.Display.setTextSize(1);
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  StickCP2.Display.fillScreen(TFT_BLACK);
}

float Screen::getInternalBatteryPercent()
{
  float vbat = StickCP2.Power.getBatteryVoltage();
  float percent = (vbat - 3.2) / (4.2 - 3.2) * 100.0;
  if (percent > 100)
    percent = 100;
  if (percent < 0)
    percent = 0;
  return percent;
}

void Screen::drawHeader(bool isConnected)
{
  int internalPercent = (int)getInternalBatteryPercent();
  StickCP2.Display.setCursor(5, 5);
  StickCP2.Display.printf("[%3d%%] ", internalPercent);

  // Set color for connection status
  if (isConnected) {
    StickCP2.Display.setTextColor(TFT_GREEN, TFT_BLACK);
    StickCP2.Display.print("CN");
  } else {
    StickCP2.Display.setTextColor(TFT_RED, TFT_BLACK);
    StickCP2.Display.print("DC");
  }

  // Restore text color if you print more after this
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
}

void Screen::drawTimeHeader()
{
  if (StickCP2.Rtc.isEnabled())
  {
    static char lastTimeStr[9] = {0};
    auto time = StickCP2.Rtc.getTime();
    char timeStr[9];
    sprintf(timeStr, "%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
    if (strcmp(timeStr, lastTimeStr) == 0) return;

    StickCP2.Display.fillRect(StickCP2.Display.width() - 50, 5, 45, 8, TFT_BLACK);
    StickCP2.Display.drawRightString(timeStr, StickCP2.Display.width() - 5, 5);
    strcpy(lastTimeStr, timeStr);
  }
}