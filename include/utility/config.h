#pragma once

#include <Arduino.h>

class Config {
public:
  Config();
  void load();
  void save();

  void setBrightness(uint8_t value);
  uint8_t getBrightness() const;

  void setBleName(const String& name);
  String getBleName() const;

private:
  uint8_t brightness = 90; // in percentage
  String bleName = "M5Security";
};