#ifndef SRC_TOURNAMENTCONTINUEDIALOG_H
#define SRC_TOURNAMENTCONTINUEDIALOG_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "TournamentSave.h"
#include "WorldCupSave.h"
#include "Teams.h"

class TournamentContinueDialog {
  public:
    TournamentContinueDialog();
    void init();
    void show(const TournamentSave& save);
    void showWC(const WorldCupSave& save);  // WM-Version
    int handleTouch(int16_t x, int16_t y);  // 1=Fortsetzen, 2=Neu starten, 0=nichts

  private:
    TFT_eSPI tft;
    Button* continueButton;
    Button* newButton;

    const char* getRoundName(TournamentType type, int round);
};

#endif
