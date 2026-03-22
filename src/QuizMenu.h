#ifndef SRC_QUIZMENU_H
#define SRC_QUIZMENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

class QuizMenu {
public:
  QuizMenu();
  void init();
  void draw();
  int handleTouch(int16_t x, int16_t y);
  // Returns: 0=Zurueck, 1=Flaggen, 2=Vereine, 3=Stadien, 4=Kennzeichen

private:
  TFT_eSPI tft;
  Button* backButton;
  Button* flaggenButton;
  Button* vereineButton;
  Button* stadienButton;
  Button* kennzeichenButton;
};

#endif
