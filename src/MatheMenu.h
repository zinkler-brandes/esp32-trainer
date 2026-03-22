#ifndef SRC_MATHEMENU_H
#define SRC_MATHEMENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

class MatheMenu {
  public:
    MatheMenu();
    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurück, 1=+/-, 2=1x1, 3=Pokal

  private:
    TFT_eSPI tft;
    Button* menuButtons[3];  // +/-, 1x1, Pokal
    Button* backButton;
};

#endif
