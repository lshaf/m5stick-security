#pragma once
#include <Arduino.h>

namespace Helper {
  void showAlert(const String &message);
  String xorEncrypt(const String &data, const String &key);
}
