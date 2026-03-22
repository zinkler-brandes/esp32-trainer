#ifndef SRC_TROPHYDISPLAY_H
#define SRC_TROPHYDISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "Teams.h"

// Farben für Pokale
#define GOLD_COLOR 0xFE60
#define GOLD_DARK 0xC500
#define SILVER_COLOR 0xC618
#define SILVER_DARK 0x8410

class TrophyDisplay {
  public:
    TrophyDisplay();
    void init();
    void showTrophy(TournamentType type);
    int handleTouch(int16_t x, int16_t y);  // 0=Zurück zum Menü

  private:
    TFT_eSPI tft;
    Button* menuButton;

    void drawDFBPokal();
    void drawHenkelpott();
    void drawConfetti();
};

#endif
