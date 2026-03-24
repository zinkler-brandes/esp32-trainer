#ifndef SRC_PARCOURS_MENU_H
#define SRC_PARCOURS_MENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

// Rechenart-Auswahl fuer Parcours
class ParcoursMenu {
  public:
    ParcoursMenu();
    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurueck, 1=+/-, 2=1x1, 3=1:1, 4=Alle

  private:
    TFT_eSPI tft;
    Button* menuButtons[4];  // +/-, 1x1, 1:1, Alle (2x2 Grid)
    Button* backButton;
};

#endif
