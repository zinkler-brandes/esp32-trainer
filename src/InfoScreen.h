#ifndef SRC_INFOSCREEN_H
#define SRC_INFOSCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Touch.h"

// Info-Typen (entsprechen den INTRO_* Konstanten)
enum InfoType {
  INFO_SCHREIBEN = 0,
  INFO_FLAG_QUIZ,
  INFO_CLUB_QUIZ,
  INFO_STADION_QUIZ,
  INFO_KENNZEICHEN_QUIZ,
  INFO_EINZELSPIEL,
  INFO_DFB_POKAL,
  INFO_CHAMPIONS_LEAGUE,
  INFO_WM
};

class InfoScreen {
  public:
    InfoScreen();

    void init();
    bool show(InfoType type, Touch* touch);  // Zeigt Infoscreen, wartet auf Button

  private:
    TFT_eSPI tft;

    // Icon-Zeichenfunktionen
    void drawPencilIcon(int x, int y, int size);
    void drawFlagIcon(int x, int y, int size);
    void drawShieldIcon(int x, int y, int size);
    void drawStadiumIcon(int x, int y, int size);
    void drawLicensePlateIcon(int x, int y, int size);
    void drawCalculatorIcon(int x, int y, int size);
    void drawTrophyIcon(int x, int y, int size, uint16_t color);

    // Text umbrechen
    void drawWrappedText(const char* text, int x, int y, int maxWidth, uint16_t color);
};

#endif
