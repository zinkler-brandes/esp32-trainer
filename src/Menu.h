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
    int handleTouch(int16_t x, int16_t y);  // 0=Zurueck, 1=Mathe, 2=Rekorde, 3=Quiz

  private:
    TFT_eSPI tft;
    Button* backButton;      // Zurueck zur Themenauswahl
    Button* matheButton;     // Mathe-Fach
    Button* recordsButton;   // Rekordematrix
    Button* quizButton;      // Quiz-Menue (Flaggen, Vereine, Stadien)
    String _playerName;

    // Icons
    void drawMatheIcon(int x, int y);
    void drawRecordsIcon(int x, int y);
    void drawQuizIcon(int x, int y);
};

#endif
