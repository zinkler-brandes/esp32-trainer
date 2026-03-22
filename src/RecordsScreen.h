#ifndef SRC_RECORDSSCREEN_H
#define SRC_RECORDSSCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "Profile.h"

// Rekordematrix Anzeige
// 3x4 Grid: Turniere (DFB, CL, WM) x Rechenarten (+/-, 1x1, 1:1, Alle)
// Zeigt Pokal-Icons fuer gewonnene Kombinationen

class RecordsScreen {
  public:
    RecordsScreen();
    void init(const RecordsMatrix& records, const String& playerName,
              int flagQuizMedal = 0, int clubQuizMedal = 0,
              int stadionQuizMedal = 0, int kennzeichenQuizMedal = 0);
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurueck

  private:
    TFT_eSPI tft;
    Button* backButton;
    RecordsMatrix _records;
    String _playerName;
    int _flagQuizMedal;
    int _clubQuizMedal;
    int _stadionQuizMedal;
    int _kennzeichenQuizMedal;

    void drawGrid();
    void drawCell(int row, int col, bool won);
    void drawMiniTrophy(int x, int y, int tournamentType);
    void drawEmptyCell(int x, int y);
    void drawQuizMedals();
    void drawMedal(int x, int y, int medal);
};

#endif
