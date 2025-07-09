#pragma once

#include <Arduino.h>

class Keypad
{
public:
  Keypad();
  void show(String title = "", int maxLength = 30);
  void handleInput();
  void updateScreen();
  void setMessage(const String& msg);
  String getMessage() const;
  void setOnOk(std::function<void(const String&)> cb);

private:
  int selectedRow = 0;
  int selectedCol = 0;
  int tapIndex = 0;
  int maxLength = 30;
  unsigned long lastTapTime = 0;
  const unsigned long tapTimeout = 800; // ms
  String printedMessage = "";
  String message = "";
  String title = "";
  std::function<void(const String&)> onOkCallback;
};