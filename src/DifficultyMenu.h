#ifndef SRC_DIFFICULTYMENU_H
#define SRC_DIFFICULTYMENU_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

// Schwierigkeitsgrade
struct Difficulty {
  const char* name;
  unsigned long answerTimeMs;  // Zeit pro Antwort in ms
  int stepsForGoal;            // Richtige Antworten für ein Tor
};

class DifficultyMenu {
  public:
    DifficultyMenu();
    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurück, 1=Bayern, 2=Dortmund, 3=Hannover

    // Schwierigkeits-Daten abrufen
    static Difficulty getDifficulty(int choice);

  private:
    TFT_eSPI tft;
    Button* menuButtons[3];  // Bayern, Dortmund, Hannover
    Button* backButton;
};

#endif
