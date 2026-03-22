#ifndef SRC_SCHREIBEN_H
#define SRC_SCHREIBEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "SoccerField.h"

class Schreiben {
  public:
    Schreiben();
    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurueck, -1=nichts

  private:
    void nextWord();
    void resetWordPool();
    void showJubelAnimation();
    void showGoalAnimation();
    void drawScoreboard();
    void drawPlayer();

    TFT_eSPI _tft;
    SoccerField _field;
    Button* _weiterButton;
    Button* _backButton;
    String _currentWord;
    int _wordsCompleted;    // Gesamtzaehler
    int _stepPosition;      // 0-4, nach 5 = Tor

    // Tracking fuer bereits gezeigte Woerter
    bool* _usedVornamen;
    bool* _usedNachnamen;
    bool* _usedTeams;
    int _remainingVornamen;
    int _remainingNachnamen;
    int _remainingTeams;
};

#endif
