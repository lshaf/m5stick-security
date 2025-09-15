#include "screen/battery.h"
#include "components/router.h"
#include "screen/main_menu.h"

#include "globals.h"
#include <M5Unified.h>

BatteryScreen::BatteryScreen() {}

void BatteryScreen::updateScreen() {
  auto power = M5.Power;
  auto d = M5.Lcd;

  d.setTextColor(TFT_WHITE, TFT_BLACK);
  d.drawCenterString("Battery Info", d.width() / 2, 18);

  d.setCursor(5, 28);
  d.print("Level: ");
  d.drawRightString(String(power.getBatteryLevel()) + "%", d.width() - 5, 28);

  d.setCursor(5, 38);
  d.print("Voltage: ");
  d.drawRightString(String(power.getBatteryVoltage()) + "mV", d.width() - 5, 38);

  d.setCursor(5, 48);
  d.print("Current: ");
  d.drawRightString(String(power.getBatteryCurrent()) + "mA", d.width() - 5, 48);

  d.setCursor(5, 58);
  d.print("Ext Out: ");
  d.drawRightString(String(power.getExtOutput()), d.width() - 5, 58);

  d.setCursor(5, 68);
  d.print("VUSB Out: ");
  d.drawRightString(String(power.getVBUSVoltage()) + "mV", d.width() - 5, 68);

  String pmicType;
  switch (power.getType()) {
    case m5::Power_Class::pmic_adc:
      pmicType = "ADC";
      break;
    case m5::Power_Class::pmic_axp192:
      pmicType = "AXP192";
      break;
    case m5::Power_Class::pmic_ip5306:
      pmicType = "IP5306";
      break;
    case m5::Power_Class::pmic_axp2101:
      pmicType = "AXP2101";
      break;
    default:
      pmicType = "Unknown";
  }

  d.setCursor(5, 78);
  d.print("Type: ");
  d.drawRightString(pmicType, d.width() - 5, 78);

  d.setCursor(5, 88);
  d.print("Is Charging: ");
  d.drawRightString(String(power.isCharging() ? "Yes" : " No"), d.width() - 5, 88);

  d.drawCenterString("Hold Encoder to Quit", d.width() / 2, d.height() - 13);
}

void BatteryScreen::handleInput() {
  static unsigned long lastRefresh = 0;
  if (millis() - lastRefresh > 1000) {
    lastRefresh = millis();
    this->redrawScreen(false);
  }

  if (encoder.wasLongPressed()) {
    Router::setScreen(new MainMenuScreen());
  }
}
