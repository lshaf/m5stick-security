#include <M5Unified.h>

namespace Helper
{
  void showAlert(const String &message)
  {
    // Save a 50x20 pixel region at (x, y)
    int w = M5.Lcd.width(), h = 20;
    int x = 0, y = M5.Lcd.height() - h;
    uint16_t* buffer = new uint16_t[w * h];

    // Save region
    M5.Lcd.readRect(x, y, w, h, buffer);

    // Draw popup (for example)
    M5.Lcd.fillRect(x, y, w, h, TFT_BLUE);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
    M5.Lcd.drawCentreString(message, w/2, y + h/2 - 4);

    // Wait a few seconds
    delay(1500);

    // Restore region
    M5.Lcd.pushImage(x, y, w, h, buffer);

    delete[] buffer;
  }

  String xorEncrypt(const String &data, const String &key)
  {
    String result = "";
    for (size_t i = 0; i < data.length(); ++i)
    {
      char c = data[i] ^ key[i % key.length()];
      result += c;
    }
    return result;
  }
} // namespace Helper
