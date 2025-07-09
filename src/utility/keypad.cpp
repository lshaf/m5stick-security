#include <M5StickCPlus2.h>
#include "utility/keypad.h"
#include "globals.h"
#include "Unit_MiniEncoderC.h"

const int NUM_ROWS = 5; // 4 keypad rows + 1 for top buttons
const int NUM_COLS = 3;

const char* KEYPAD_LAYOUT[4][3] = {
  {".1", "abc2", "def3"},
  {"ghi4", "jkl5", "mno6"},
  {"pqrs7", "tuv8", "wxyz9"},
  {"NUM", " 0", "CAPS"}
};

const char* TOP_BUTTONS[3] = {"OK", "", "DEL"}; // Center is empty

// Add these at the top of your file or as class members if not already present
unsigned long lastTapTime = 0;
int tapIndex = 0;
int lastRow = -1, lastCol = -1;
const unsigned long tapTimeout = 1000; // 1 second

Keypad::Keypad() {}

// Add these as class members or static variables at the top
bool numMode = false;
bool capsMode = false;
void Keypad::setMessage(const String &msg) {
  message = msg;
}

String Keypad::getMessage() const {
  return message;
}

void Keypad::setOnOk(std::function<void(const String &)> cb) {
  onOkCallback = cb;
}

void Keypad::show(String title, int maxLength) {
  this->maxLength = maxLength;
  this->title = title;
  this->printedMessage = "";
  this->selectedCol = 0;
  this->selectedRow = 0;

  StickCP2.Display.fillRect(0, 18, StickCP2.Display.width(), StickCP2.Display.height() - 18, TFT_BLACK);
  updateScreen();
}

void Keypad::updateScreen() {
  StickCP2.Display.setTextSize(1);
  int btnW = (StickCP2.Display.width() - (6 * NUM_COLS)) / NUM_COLS, btnH = 25, x0 = 5, y0 = StickCP2.Display.height() - (NUM_ROWS * (btnH + 5));
  for (int col = 0; col < 3; ++col) {
    if (strlen(TOP_BUTTONS[col]) > 0) {
      uint16_t color = (selectedRow == 0 && selectedCol == col) ? SELECTED_COLOR : TFT_WHITE;
      StickCP2.Display.drawRect(x0 + col * (btnW + 5), y0, btnW, btnH, color);
      StickCP2.Display.setCursor(x0 + col * (btnW + 5) + 6, y0 + 6);
      StickCP2.Display.setTextColor(color, TFT_BLACK);
      StickCP2.Display.print(TOP_BUTTONS[col]);
    }
  }
  // Draw keypad buttons
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 3; ++col) {
      int x = x0 + col * (btnW + 5);
      int y = y0 + (row + 1) * (btnH + 5);

      // Highlight NUM or CAPS if toggled
      bool isNumKey = (row == 3 && col == 0);
      bool isCapsKey = (row == 3 && col == 2);
      bool isActive = (isNumKey && numMode) || (isCapsKey && capsMode);

      uint16_t color = (selectedRow == row + 1 && selectedCol == col) ? SELECTED_COLOR : TFT_WHITE;
      uint16_t fillColor = isActive ? TFT_WHITE : TFT_BLACK;
      uint16_t textColor = isActive ? TFT_BLACK : color;

      StickCP2.Display.fillRect(x, y, btnW, btnH, fillColor);
      StickCP2.Display.drawRect(x, y, btnW, btnH, color);
      StickCP2.Display.setCursor(x + 6, y + 6);
      StickCP2.Display.setTextColor(textColor, fillColor);

      if (isNumKey) {
        StickCP2.Display.print("NUM");
      } else if (isCapsKey) {
        StickCP2.Display.print("CAPS");
      } else {
        String label = KEYPAD_LAYOUT[row][col];
        if (numMode) {
          StickCP2.Display.print(label[label.length() - 1]);
        } else if (capsMode && row < 3) {
          String upLabel = label;
          upLabel.toUpperCase();
          StickCP2.Display.print(upLabel);
        } else {
          StickCP2.Display.print(label);
        }
      }
    }
  }

  if (this->title.length() > 0) {
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.fillRect(5, 16, StickCP2.Display.width() - 10, StickCP2.Display.fontHeight() + 4, TFT_BLACK);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    StickCP2.Display.drawCenterString(this->title, StickCP2.Display.width() / 2, 18);
  }

  // Draw message at the top
  if (printedMessage != message) {
    int yText = 30;
    int xPadding = 5;
    StickCP2.Display.setTextSize(2);
    StickCP2.Display.fillRect(0, yText, StickCP2.Display.width(), StickCP2.Display.fontHeight() * 3, TFT_BLACK);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    // Print message in lines of 10 chars, with left padding
    for (int i = 0; i < message.length(); i += 10) {
      StickCP2.Display.setCursor(xPadding, yText + (i / 10) * StickCP2.Display.fontHeight());
      StickCP2.Display.print(message.substring(i, i + 10));
    }

    StickCP2.Display.setTextSize(1);
    printedMessage = message;
  }
}

void Keypad::handleInput() {
  bool redraw = false;

  // Navigation
  if (encoder.movedLeft()) {
    selectedCol = selectedCol - 1;
    if (selectedRow == 0 && selectedCol == 1) selectedCol = 0;
  }
  if (encoder.movedRight()) {
    selectedCol = selectedCol + 1;
    if (selectedRow == 0 && selectedCol == 1) selectedCol = 2;
  }
  if (encoder.isMoved()) {
    if (selectedCol < 0) {
      selectedRow--;
      selectedCol = NUM_COLS - 1;
    }
    if (selectedCol >= NUM_COLS) {
      selectedRow++;
      selectedCol = 0;
    }
    if (selectedRow < 0) selectedRow = NUM_ROWS - 1;
    if (selectedRow >= NUM_ROWS) selectedRow = 0;
    if (message.length() >= this->maxLength) {
      selectedRow = 0; // Reset to top row if message is too long
      if (selectedCol == 1) selectedCol = 0;
    }
    redraw = true;
  }

  // Multi-tap timeout: finalize character if enough time has passed
  if (selectedRow > 0 && lastRow == selectedRow && lastCol == selectedCol) {
    if (millis() - lastTapTime > tapTimeout && tapIndex >= 0) {
      // Finalize character
      tapIndex = -1;
    }
  }

  // Handle button press (only on press, not hold)
  if (encoder.wasPressed()) { // Only on new press
    unsigned long now = millis();
    if (selectedRow == 0) {
      // Top buttons
      if (selectedCol == 0) {
        if (onOkCallback) onOkCallback(message);
      } else if (selectedCol == 2) {
        if (!message.isEmpty()) message.remove(message.length() - 1);
        redraw = true;
      }
    } else {
      // Keypad area
      if (selectedRow == 4 && selectedCol == 0) {
        // NUM key
        numMode = !numMode;
        capsMode = false;
        redraw = true;
      } else if (selectedRow == 4 && selectedCol == 2) {
        // CAPS key
        capsMode = !capsMode;
        numMode = false;
        redraw = true;
      } else {
        // Normal key input
        tapIndex++;
        const char* chars = KEYPAD_LAYOUT[selectedRow - 1][selectedCol];
        char c;
        if (numMode) {
          c = chars[strlen(chars) - 1]; // Only the number
        } else if (capsMode && selectedRow > 0 && selectedRow < 4) {
          // Only uppercase for letter keys
          String temp(chars);
          c = temp[tapIndex % temp.length()];
          c = toupper(c);
        } else {
          c = chars[tapIndex % strlen(chars)];
        }
        if (lastRow == selectedRow && lastCol == selectedCol && (now - lastTapTime < tapTimeout)) {
          if (!message.isEmpty()) {
            message.setCharAt(message.length() - 1, c);
          } else {
            message += c;
          }
        } else {
          tapIndex = 0;
          message += c;
          if (message.length() >= 30) {
            selectedRow = 0; // Reset to top row if message is too long
            if (selectedCol == 1) selectedCol = 0;
          }
        }
        lastTapTime = now;
        lastRow = selectedRow;
        lastCol = selectedCol;
        redraw = true;
      }
    }
  }

  // Finalize character if timeout passed
  if (selectedRow > 0 && tapIndex >= 0 && (millis() - lastTapTime > tapTimeout)) {
    tapIndex = -1;
  }

  if (redraw) updateScreen();
}