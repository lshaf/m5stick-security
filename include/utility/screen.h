#pragma once

#include <M5StickCPlus2.h>

class Screen {
public:
  void drawHeader();
  void drawTimeHeader();
  float getInternalBatteryPercent();
};