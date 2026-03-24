#ifndef SRC_REITABZEICHEN_MENU_H
#define SRC_REITABZEICHEN_MENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

// Schwierigkeitsauswahl fuer Springreiten (Bronze, Silber, Gold)
class ReitabzeichenMenu {
  public:
    ReitabzeichenMenu();
    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurueck, 1=Bronze, 2=Silber, 3=Gold

  private:
    TFT_eSPI tft;
    Button* menuButtons[3];  // Bronze, Silber, Gold
    Button* backButton;

    void drawMedal(int x, int y, uint16_t color, const char* text);
};

#endif
