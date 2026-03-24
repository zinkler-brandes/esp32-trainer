#ifndef SRC_KITA_MENU_H
#define SRC_KITA_MENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

class KiTaMenu {
  public:
    KiTaMenu();
    void init();
    void draw();
    void setPlayerName(const String& name);
    int handleTouch(int16_t x, int16_t y);  // 0=Zurueck, 1=Schreiben

  private:
    TFT_eSPI tft;
    Button* backButton;
    Button* schreibenButton;
    String _playerName;

    void drawPencilIcon(int x, int y);
};

#endif
