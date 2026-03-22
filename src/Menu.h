#ifndef SRC_MENU_H
#define SRC_MENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

#define MAX_PLAYER_NAME_LENGTH 8

class Menu {
  public:
    Menu();
    void init();
    void draw();
    void setPlayerName(const String& name);
    int handleTouch(int16_t x, int16_t y);  // 0=Zurück, 1=Mathe, 2=Englisch

  private:
    TFT_eSPI tft;
    Button* backButton;      // Zurück zur Profilauswahl
    Button* menuButtons[2];  // Fächer: Mathe, Englisch
    String _playerName;
};

#endif
