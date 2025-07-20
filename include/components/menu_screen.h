#pragma once

class MenuScreen {
public:
  void redrawScreen(bool clear = true);
  bool needsRedraw();
  virtual bool isScreenAlwaysOn() const { return false; }
  virtual void updateScreen() = 0;
  virtual void handleInput() = 0;
  virtual void destroy() = 0;
  virtual ~MenuScreen() {};
private:
  bool triggerRedraw = true;
};