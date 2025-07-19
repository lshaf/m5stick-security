#pragma once

class MenuScreen {
public:
  bool needsRedraw = true;
  void redrawScreen(bool clear = true);
  virtual void updateScreen() = 0;
  virtual void handleInput() = 0;
  virtual void destroy() = 0;
  virtual ~MenuScreen() {}
};