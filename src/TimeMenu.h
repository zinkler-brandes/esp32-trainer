#ifndef SRC_TIMEMENU_H
#define SRC_TIMEMENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

class TimeMenu {
  public:
    TimeMenu();
    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurück, 1=1Min, 2=3Min, 3=5Min

  private:
    TFT_eSPI tft;
    Button* menuButtons[3];  // 1 Min, 3 Min, 5 Min
    Button* backButton;
};

#endif
