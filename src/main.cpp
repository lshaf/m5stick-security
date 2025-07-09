#include <Arduino.h>
#include <LittleFS.h>
#include <M5StickCPlus2.h>

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

  Router::setScreen(new MainMenuScreen());
}

void loop()
{
  StickCP2.update();
  encoder.updateEncoderState();
  
  SerialCommand::listen();
  screen.drawTimeHeader();
  Router::handleMenu();
}
