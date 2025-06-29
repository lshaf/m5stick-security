#pragma once

#include <M5StickCPlus2.h>

class Screen {
public:
  void drawHeader(bool isConnected);
  void drawTimeHeader();
private:
  float getInternalBatteryPercent();
};