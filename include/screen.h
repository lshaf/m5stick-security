#pragma once
#include <M5StickCPlus2.h>

class Screen
{
public:
  void init();
  float getInternalBatteryPercent();
  void drawHeader(bool isConnected);
  void drawTimeHeader();
};