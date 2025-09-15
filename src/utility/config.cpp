#include "utility/config.h"
#include <SPIFFS.h>

#define CONFIG_FILE "/config.ini"

Config::Config() {
  load();
}

void Config::load() {
  if (!SPIFFS.begin()) return;
  File file = SPIFFS.open(CONFIG_FILE, "r");
  if (!file) return;

  String line;
  while (file.available()) {
    line = file.readStringUntil('\n');
    int sep = line.indexOf('=');
    if (sep == -1) continue;
    String key = line.substring(0, sep);
    String value = line.substring(sep + 1);
    value.trim();
    if (key == "brightness") {
      brightness = value.toInt();
    } else if (key == "bleName") {
      bleName = value;
    } else if (key == "fingerLockEnabled") {
      fingerLockEnabled = (value == "true");
    }
  }
  file.close();
}

void Config::save() {
  if (!SPIFFS.begin()) return;
  File file = SPIFFS.open(CONFIG_FILE, "w");
  if (!file) return;
  file.printf("brightness=%u\n", brightness);
  file.printf("bleName=%s\n", bleName.c_str());
  file.printf("fingerLockEnabled=%s\n", fingerLockEnabled ? "true" : "false");
  file.close();
}

void Config::setBrightness(uint8_t value) {
  brightness = value;
  if (brightness > 100) {
    brightness = 100; // Cap brightness at 100%
  } else if (brightness < 0) {
    brightness = 0; // Ensure brightness is not negative
  }

  save();
}

uint8_t Config::getBrightness() const {
  return brightness;
}

void Config::setBleName(const String& name) {
  bleName = name;
  if (bleName.length() > 14) {
    bleName = bleName.substring(0, 14);
  }
  
  save();
}

String Config::getBleName() const {
  return bleName;
}

void Config::setFingerLockEnabled(bool enabled) {
  fingerLockEnabled = enabled;
  save();
}

bool Config::isFingerLockEnabled() const {
  return fingerLockEnabled;
}