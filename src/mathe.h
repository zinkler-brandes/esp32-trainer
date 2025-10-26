#ifndef SRC_MATHE_H
#define SRC_MATHE_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

class Mathe {
  public:
    Mathe();
    void init();
    void drawButtons();
    void handleButtonPress(int16_t x, int16_t y);  // Touch-Koordinaten verarbeiten

  private:
    TFT_eSPI tft;

    // Test-Buttons für Numpad
    Button* buttons[12];  // 0-9, C, OK
};

#endif
