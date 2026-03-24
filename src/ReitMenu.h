#ifndef SRC_REIT_MENU_H
#define SRC_REIT_MENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

#define MAX_PLAYER_NAME_LENGTH 8

class ReitMenu {
  public:
    ReitMenu();
    void init();
    void draw();
    void setPlayerName(const String& name);
    int handleTouch(int16_t x, int16_t y);  // 0=Zurueck, 1=Parcours, 2=Turnier, 3=Rekorde

  private:
    TFT_eSPI tft;
    Button* backButton;       // Zurueck zur Themenauswahl
    Button* parcoursButton;   // Einzelspiel Parcours
    Button* turnierButton;    // Turnier-Modus
    Button* recordsButton;    // Reit-Rekorde
    String _playerName;

    void drawHorseIcon(int x, int y, int size);
};

#endif
