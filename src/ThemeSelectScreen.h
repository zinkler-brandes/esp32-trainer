#ifndef SRC_THEME_SELECT_SCREEN_H
#define SRC_THEME_SELECT_SCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

// Thema-Auswahl nach Profil-Login
enum Theme {
  THEME_FUSSBALL,
  THEME_KITA,
  THEME_REITEN
};

class ThemeSelectScreen {
  public:
    ThemeSelectScreen();
    void init();
    void draw();
    void setPlayerName(const String& name);
    int handleTouch(int16_t x, int16_t y);  // 0=Zurueck, 1=Fussball, 2=KiTa, 3=Reiten

  private:
    TFT_eSPI tft;
    Button* backButton;
    Button* fussballButton;
    Button* kitaButton;
    Button* reitenButton;
    String _playerName;

    void drawSoccerIcon(int x, int y);
    void drawKitaIcon(int x, int y);
    void drawHorseIcon(int x, int y);
};

#endif
