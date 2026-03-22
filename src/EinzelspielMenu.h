#ifndef SRC_EINZELSPIELMENU_H
#define SRC_EINZELSPIELMENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

class EinzelspielMenu {
  public:
    EinzelspielMenu();
    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurück, 1=+/-, 2=1x1, 3=1:1, 4=Alle

  private:
    TFT_eSPI tft;
    Button* menuButtons[4];  // +/-, 1x1, 1:1, Alle (2x2 Grid)
    Button* backButton;
};

#endif
