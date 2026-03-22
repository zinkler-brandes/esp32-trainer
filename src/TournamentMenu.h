#ifndef SRC_TOURNAMENTMENU_H
#define SRC_TOURNAMENTMENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "Teams.h"
#include "mathe.h"

class TournamentMenu {
  public:
    TournamentMenu();
    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurück, 1=DFB-Pokal, 2=CL

  private:
    TFT_eSPI tft;
    Button* dfbButton;
    Button* clButton;
    Button* backButton;
};

// Zweites Menü für Mathe-Modus-Auswahl (nur für DFB-Pokal)
class TournamentMatheModeMenu {
  public:
    TournamentMatheModeMenu();
    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurück, 1=+/-, 2=1x1

  private:
    TFT_eSPI tft;
    Button* plusMinusButton;
    Button* multiplyButton;
    Button* backButton;
};

#endif
