#pragma once

#include <Arduino.h>

class NumberPad
{
public:
  NumberPad();
  void show(String title = "", int min = 0, int max = 999);
  void updateScreen();
  void handleInput();
  void setValue(int value);
  int getValue() const;
  void setOnOk(std::function<void(int)> cb);

private:
  int currentValue = 0;
  int minValue = 0;
  int maxValue = 999;
  String title = "";
  std::function<void(int)> onOkCallback;
};
