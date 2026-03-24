#ifndef SRC_TOURNAMENT_H
#define SRC_TOURNAMENT_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "Teams.h"
#include "mathe.h"
#include "TournamentSave.h"

// Turnier-Status
enum TournamentState {
  TOURNAMENT_INTRO,           // Zeigt aktuelle Runde und Gegner
  TOURNAMENT_PLAYING,         // Mathe-Spiel läuft
  TOURNAMENT_ROUND_WON,       // Runde gewonnen
  TOURNAMENT_PENALTY,         // Elfmeterschießen bei Unentschieden
  TOURNAMENT_ROUND_LOST,      // Runde verloren (ausgeschieden)
  TOURNAMENT_CHAMPION         // Turnier gewonnen!
};

class Tournament {
  public:
    Tournament();
    void init(TournamentType type, MatheMode matheMode);
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurück zum Menü, 1=Weiter/Anpfiff, -1=nichts
    void update();  // Timer etc.

    // Spielzustand
    TournamentState getState() { return _state; }
    void setState(TournamentState state) { _state = state; }

    // Runden-Info
    int getCurrentRound() { return _currentRound; }
    int getTotalRounds();
    const char* getCurrentRoundName();
    Team getCurrentOpponent() { return _currentOpponent; }

    // Fortschritt
    void advanceToNextRound();
    void handleMatchResult(int playerGoals, int cpuGoals);

    // Schwierigkeit für aktuelle Runde
    unsigned long getAnswerTime();
    int getStepsForGoal() { return STEPS_FOR_GOAL; }
    int getCpuPenaltyProb();

    // Elfmeterschießen
    void startPenaltyShootout();
    bool handlePenalty(bool playerScored, bool cpuScored);  // true = Shootout beendet
    int getPlayerPenalties() { return _playerPenalties; }
    int getCpuPenalties() { return _cpuPenalties; }
    int getPenaltyRound() { return _penaltyRound; }

    // Mathe-Modus
    MatheMode getMatheMode() { return _matheMode; }
    TournamentType getTournamentType() { return _tournamentType; }

    // Spieler-Team
    void setPlayerTeam(const String& name, const String& abbrev, uint16_t color);
    String getPlayerTeamName() { return _playerTeamName; }
    String getPlayerTeamAbbrev() { return _playerTeamAbbrev; }
    uint16_t getPlayerTeamColor() { return _playerTeamColor; }

    // Save/Load
    TournamentSave createSave();
    TournamentSave createSaveForNextRound();  // Speichert mit naechster Runde + neuem Gegner
    void loadFromSave(const TournamentSave& save);

  private:
    TFT_eSPI tft;
    Button* kickoffButton;
    Button* continueButton;
    Button* backButton;
    Button* retryButton;
    Button* menuButton;

    TournamentType _tournamentType;
    MatheMode _matheMode;
    TournamentState _state;
    int _currentRound;  // 0-basiert
    Team _currentOpponent;

    // Spieler-Team
    String _playerTeamName;
    String _playerTeamAbbrev;
    uint16_t _playerTeamColor;

    // Elfmeterschießen
    int _playerPenalties;
    int _cpuPenalties;
    int _penaltyRound;  // 1-3 (erste 3 Schüsse), dann weitere
    bool _playerShot;   // Spieler hat in dieser Runde geschossen

    // Besiegter Gegner (fuer "GEWONNEN" Anzeige)
    Team _defeatedOpponent;
    int _wonRound;  // Die gewonnene Runde

    // Statische Speicher für geladene Gegnerdaten (weil Team const char* verwendet)
    String _loadedOpponentName;
    String _loadedOpponentAbbrev;

    void selectOpponent();
    void drawIntro();
    void drawRoundWon();
    void drawRoundLost();
    void drawPenalty();
    void drawChampion();
    void drawTeamBadge(int x, int y, Team team);
};

#endif
