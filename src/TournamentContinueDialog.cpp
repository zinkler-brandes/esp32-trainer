#include "TournamentContinueDialog.h"
#include "WM2026Teams.h"
#include "WM2026Groups.h"

TournamentContinueDialog::TournamentContinueDialog() {
  continueButton = new Button(20, 150, 130, 50, "WEITER");
  continueButton->setColors(TFT_GREEN, TFT_BLACK);

  newButton = new Button(170, 150, 130, 50, "NEU");
  newButton->setColors(TFT_ORANGE, TFT_WHITE);
}

void TournamentContinueDialog::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

const char* TournamentContinueDialog::getRoundName(TournamentType type, int round) {
  if (type == TOURNAMENT_DFB_POKAL) {
    if (round >= 0 && round < DFB_ROUNDS) {
      return DFB_ROUND_NAMES[round];
    }
  } else {
    if (round >= 0 && round < CL_ROUNDS) {
      return CL_ROUND_NAMES[round];
    }
  }
  return "???";
}

void TournamentContinueDialog::show(const TournamentSave& save) {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(40, 20);
  tft.print("TURNIER FORTSETZEN?");

  // Turnier-Info
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(30, 60);
  tft.print(save.type == TOURNAMENT_DFB_POKAL ? "DFB-Pokal" : "Champions League");

  // Runde
  tft.setCursor(30, 85);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.print(getRoundName(save.type, save.round));

  // Gegner
  tft.setCursor(30, 115);
  tft.setTextColor(save.opponentColor, TFT_BLACK);
  tft.print("vs ");
  tft.print(save.opponentName);

  // Buttons
  continueButton->draw(&tft);
  newButton->draw(&tft);
}

void TournamentContinueDialog::showWC(const WorldCupSave& save) {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(40, 20);
  tft.print("WM FORTSETZEN?");

  // Team
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(30, 55);
  tft.print(WM_TEAMS[save.playerTeamIndex].name);

  // Gruppe
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(30, 78);
  tft.printf("Gruppe %c", getGroupLetter(save.playerGroupIndex));

  // Aktuelle Runde
  tft.setCursor(30, 95);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.print(save.getCurrentRoundName());

  // Details
  tft.setCursor(30, 120);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  if (save.knockoutRound < 0) {
    // Gruppenphase
    tft.printf("Spiel %d von 3", save.groupMatchIndex + 1);
  } else {
    // K.O.-Phase - Gegner anzeigen
    if (save.knockoutOpponentIndex >= 0 && save.knockoutOpponentIndex < WM_TEAM_COUNT) {
      tft.printf("vs %s", WM_TEAMS[save.knockoutOpponentIndex].name);
    }
  }

  // Buttons
  continueButton->draw(&tft);
  newButton->draw(&tft);
}

int TournamentContinueDialog::handleTouch(int16_t x, int16_t y) {
  if (continueButton->contains(x, y)) {
    return 1;  // Fortsetzen
  }
  if (newButton->contains(x, y)) {
    return 2;  // Neu starten
  }
  return 0;
}
