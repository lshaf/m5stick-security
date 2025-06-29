#pragma once

#include "components/menu_manager.h"

class InputNameScreen : public MenuScreen {
public:
  void updateScreen() override;
  void handleInput() override;
  void destroy() override;
private:
  String inputName;
  enum {STATE_MAIN, STATE_INPUT, STATE_RESULT} state = STATE_MAIN;
};