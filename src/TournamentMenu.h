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
    int handleTouch(int16_t x, int16_t y);  // 0=Zurueck, 1=DFB-Pokal, 2=CL, 3=WM2026

  private:
    TFT_eSPI tft;
    Button* dfbButton;
    Button* clButton;
    Button* wmButton;
    Button* backButton;
};

// Zweites Menü für Mathe-Modus-Auswahl (nur für DFB-Pokal)
class TournamentMatheModeMenu {
  public:
    TournamentMatheModeMenu();
    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurück, 1=+/-, 2=1x1, 3=1:1, 4=Alle

  private:
    TFT_eSPI tft;
    Button* menuButtons[4];  // +/-, 1x1, 1:1, Alle (2x2 Grid)
    Button* backButton;
};

#endif
