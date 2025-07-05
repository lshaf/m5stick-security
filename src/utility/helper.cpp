#include <M5StickCPlus2.h>

namespace Helper
{
  void showAlert(const String &message)
  {
    // Save a 50x20 pixel region at (x, y)
    int w = StickCP2.Display.width(), h = 20;
    int x = 0, y = StickCP2.Display.height() - h;
    uint16_t* buffer = new uint16_t[w * h];

    // Save region
    StickCP2.Display.readRect(x, y, w, h, buffer);

    // Draw popup (for example)
    StickCP2.Display.fillRect(x, y, w, h, TFT_BLUE);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLUE);
    StickCP2.Display.drawCentreString(message, w/2, y + h/2 - 4);

    // Wait a few seconds
    delay(1500);

    // Restore region
    StickCP2.Display.pushImage(x, y, w, h, buffer);

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
