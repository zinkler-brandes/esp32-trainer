#ifndef SRC_MENU_H
#define SRC_MENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

class Menu {
  public:
    Menu();
    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // Gibt gewählte Option zurück (1-3, 0=nichts)

  private:
    TFT_eSPI tft;
    Button* menuButtons[3];  // 3 Menü-Optionen
};

#endif
