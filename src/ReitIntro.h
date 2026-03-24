#ifndef SRC_REIT_INTRO_H
#define SRC_REIT_INTRO_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "Touch.h"
#include "SpringreitenTrainer.h"

class ReitIntro {
  public:
    ReitIntro();
    void init();

    // Intro-Screen anzeigen (blockiert bis Button gedrückt)
    void showIntro(Reitabzeichen level, Touch* touch);

  private:
    TFT_eSPI tft;
    Button* weiterButton;

    void drawHorseJumping(int x, int y);
    void drawRibbon(int x, int y, uint16_t color);
};

#endif
