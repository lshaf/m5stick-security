#include <Arduino.h>
#include <LittleFS.h>
#include <M5StickCPlus2.h>
#include <BleCombo.h>

#include "globals.h"
#include "utility/screen.h"
#include "utility/encoder_state_manager.h"
#include "utility/serial_command.h"

#include "screen/main_menu.h"
#include "components/router.h"

Screen screen;
MenuManager* mainMenu = nullptr;

UNIT_MINIENCODERC mini_encoder;
EncoderStateManager encoder(mini_encoder);

void setup()
{
  StickCP2.begin();
  LittleFS.begin();
  Serial.begin(115200);
  mini_encoder.begin(&Wire, MINIENCODERC_ADDR, 0, 26, 100000UL);

  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  StickCP2.Display.fillScreen(TFT_BLACK);

  StickCP2.Display.setBrightness(255 * config.getBrightness() / 100);
  bleDevice.setName(config.getBleName().c_str());

  Router::setScreen(new MainMenuScreen());
}

void loop()
{
  static bool isScreenOff = false;;
  static unsigned long lastUpdate = 0;
  StickCP2.update();
  encoder.updateEncoderState();
  
  if (encoder.isMoved() || encoder.wasPressed()) {
    lastUpdate = millis();
    StickCP2.Display.setBrightness(255 * config.getBrightness() / 100);
    isScreenOff = false;
    encoder.offLight();
  }

  if (millis() - lastUpdate > 10000 && !isScreenOff) {
    StickCP2.Display.setBrightness(0);
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
