#include <Arduino.h>
#include <LittleFS.h>
#include <M5Unified.h>
#include <BleCombo.h>

#include "globals.h"
#include "utility/screen.h"
#include "utility/encoder_state_manager.h"
#include "utility/serial_command.h"

#include "screen/lock.h"
#include "screen/main_menu.h"
#include "components/router.h"

Screen screen;

UNIT_MINIENCODERC mini_encoder;
EncoderStateManager encoder(mini_encoder);

void setup()
{
  auto cf = M5.config();
  M5.begin(cf);
  LittleFS.begin();
  Serial.begin(115200);
  
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(SELECTED_COLOR, TFT_BLACK);
  M5.Lcd.drawCenterString("Booting", 
    M5.Lcd.width() / 2, 
    M5.Lcd.height() / 2
  );

  // finger.begin(&Serial2, FINGER_PRINT_RX, FINGER_PRINT_TX, 19200);
  mini_encoder.begin(&Wire, MINIENCODERC_ADDR, 0, 26, 100000UL);

  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.fillScreen(TFT_BLACK);

  M5.Lcd.setBrightness(255 * config.getBrightness() / 100);
  bleDevice.setName(config.getBleName().c_str());

  if (config.isFingerLockEnabled()) {
    Router::setScreen(new LockScreen());
  } else {
    Router::setScreen(new MainMenuScreen());
  }
}

void loop()
{
  static bool isScreenOff = false;;
  static unsigned long lastUpdate = 0;
  M5.update();
  encoder.updateEncoderState();

  MenuScreen* current = Router::getCurrentScreen();
  bool isAlwaysOn = current && current->isScreenAlwaysOn();

  if (encoder.wasMoved() || encoder.wasPressed() || isAlwaysOn) {
    lastUpdate = millis();
    M5.Lcd.setBrightness(255 * config.getBrightness() / 100);
    isScreenOff = false;
    encoder.offLight();
  }

  if (millis() - lastUpdate > 10000 && !isScreenOff) {
    M5.Lcd.setBrightness(0);
    isScreenOff = true;
  }

  if (!isScreenOff) {
    SerialCommand::listen();
    screen.drawTimeHeader();
    Router::handleMenu();
  } else {
    encoder.breathLight();
  }
}
