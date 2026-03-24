#ifndef SRC_WORLDCUPTOURNAMENT_H
#define SRC_WORLDCUPTOURNAMENT_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "Teams.h"
#include "WM2026Teams.h"
#include "WM2026Groups.h"
#include "WM2026Venues.h"
#include "WM2026Schedule.h"
#include "WorldCupSave.h"
#include "GroupTable.h"
#include "mathe.h"

// WM Turnier-Status
enum WCState {
  WC_GROUP_INTRO,        // "Gruppenphase Gruppe E"
  WC_MATCH_INTRO,        // Zeigt naechstes Spiel + Stadion
  WC_PLAYING,            // Mathe-Spiel laeuft
  WC_MATCH_RESULT,       // Ergebnis anzeigen
  WC_GROUP_TABLE,        // Tabelle nach jedem Spiel
  WC_GROUP_RESULTS,      // Ergebnisse einer Gruppe
  WC_GROUP_COMPLETE,     // Gruppenphase beendet
  WC_KNOCKOUT_INTRO,     // K.O.-Runden Intro
  WC_BRACKET_VIEW,       // K.O.-Bracket Uebersicht
  WC_ELIMINATED,         // Ausgeschieden (kann als Zuschauer weiterschauen)
  WC_SPECTATOR,          // Zuschauer-Modus
  WC_CHAMPION            // Weltmeister!
};

class WorldCupTournament {
  public:
    WorldCupTournament();

    // Initialisierung
    void init(MatheMode matheMode, int playerTeamIndex);
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 0=Zurueck, 1=Weiter/Anpfiff, 2=Zuschauer, -1=nichts
    void update();

    // Status
    WCState getState() { return _state; }
    void setState(WCState state) { _state = state; }

    // Spieler-Team Info
    int getPlayerTeamIndex() { return _playerTeamIndex; }
    const WMTeam& getPlayerTeam() { return WM_TEAMS[_playerTeamIndex]; }
    int getPlayerGroupIndex() { return _playerGroupIndex; }

    // Aktueller Gegner
    int getCurrentOpponentIndex() { return _currentOpponentIndex; }
    const WMTeam& getCurrentOpponent() { return WM_TEAMS[_currentOpponentIndex]; }

    // Schwierigkeit basierend auf Gegner
    unsigned long getAnswerTime();
    int getCpuPenaltyProb();
    int getStepsForGoal() { return STEPS_FOR_GOAL; }

    // Spielort
    const WMVenue& getCurrentVenue() { return WM_VENUES[_currentVenueIndex]; }

    // Mathe-Modus
    MatheMode getMatheMode() { return _matheMode; }

    // Gruppenphase
    void handleGroupMatchResult(int playerGoals, int cpuGoals);
    bool isGroupPhaseComplete();
    bool isPlayerQualifiedFromGroup();
    GroupTable& getGroupTable() { return _groupTable; }

    // K.O.-Phase
    void advanceToKnockout();
    void handleKnockoutMatchResult(int playerGoals, int cpuGoals);
    int getKnockoutRound() { return _knockoutRound; }
    const char* getCurrentRoundName();

    // Zuschauer-Modus
    void startSpectatorMode();
    void simulateNextSpectatorMatch();
    bool isSpectator() { return _isSpectator; }

    // Elfmeterschießen
    void startPenaltyShootout(int playerGoals, int cpuGoals);
    bool handlePenalty(bool playerScored, bool cpuScored);
    int getPlayerPenalties() { return _playerPenalties; }
    int getCpuPenalties() { return _cpuPenalties; }
    int getPenaltyRound() { return _penaltyRound; }

    // Save/Load
    WorldCupSave createSave();
    void loadFromSave(const WorldCupSave& save);

  private:
    TFT_eSPI tft;
    Button* kickoffButton;
    Button* continueButton;
    Button* backButton;
    Button* spectatorButton;
    Button* menuButton;
    Button* prevGroupButton;    // Gruppe links
    Button* nextGroupButton;    // Gruppe rechts
    Button* bracketButton;      // Bracket-Ansicht Button
    Button* prevRoundButton;    // K.O.-Runde zurueck
    Button* nextRoundButton;    // K.O.-Runde vor
    Button* resultsButton;      // Ergebnisse anzeigen
    Button* scrollUpButton;     // Ergebnisse hoch scrollen
    Button* scrollDownButton;   // Ergebnisse runter scrollen
    Button* bracketPrevButton;  // Bracket-Seite zurueck
    Button* bracketNextButton;  // Bracket-Seite vor

    // Spieler-Team und Gruppe
    int _playerTeamIndex;
    int _playerGroupIndex;
    MatheMode _matheMode;

    // Status
    WCState _state;

    // Gruppenphase
    int _groupMatchIndex;      // 0, 1, 2 (3 Spiele)
    int _playerGroupPosition;  // 1-4 nach Gruppenphase
    GroupTable _groupTable;    // Spieler-Gruppe (fuer Live-Updates)
    GroupTable _allGroups[12]; // Alle 12 Gruppen (fuer Navigation)
    int _viewingGroupIndex;    // Welche Gruppe wird angezeigt (-1 = Spieler-Gruppe)
    int _resultsPage;          // Ergebnisse-Seite (0 = Spieltag 1+2, 1 = Spieltag 3)
    int _bracketPage;          // Bracket-Seite (fuer R32: 0 = Spiele 1-8, 1 = Spiele 9-16)

    // K.O.-Phase
    int _knockoutRound;        // 0=R32, 1=R16, 2=QF, 3=SF, 4=Final
    int _currentOpponentIndex;
    bool _isSpectator;

    // Spielort
    int _currentVenueIndex;

    // Elfmeterschießen
    int _playerPenalties;
    int _cpuPenalties;
    int _penaltyRound;

    // K.O.-Bracket
    int _bracketTeams[32];     // Teams in Runde der 32
    int _bracketWinners[31];   // Gewinner der K.O.-Spiele
    int _bracketResults[31][2]; // Ergebnisse (Tore) der K.O.-Spiele
    bool _bracketPlayed[31];   // Ob Spiel gespielt wurde
    bool _bracketPenalty[31];  // Ob durch Elfmeterschiessen entschieden
    int _bracketPenaltyResults[31][2]; // Elfmeter-Ergebnis
    int _viewingRound;         // Welche K.O.-Runde wird im Bracket angezeigt

    // Private Methoden
    void selectNextGroupOpponent();
    void selectNextKnockoutOpponent();
    void selectVenueForMatch();
    void simulateOtherGroupMatches();
    void simulateAllOtherGroups();  // Alle anderen Gruppen komplett simulieren
    void simulateMatchday(int matchday);  // Spieltag fuer alle Gruppen simulieren
    void initAllGroups();           // Alle 12 Gruppen initialisieren
    int simulateMatch(int team1Index, int team2Index);

    // CPU vs CPU Match-Simulation (gibt Ergebnis als encoded int: team1Goals * 10 + team2Goals)
    int simulateCPUMatch(int team1Index, int team2Index);

    // Beste Dritte Berechnung
    void calculateBestThirds();
    bool isThirdPlaceQualified(int groupIndex);

    // Bracket aufbauen
    void buildKnockoutBracket();

    // Zeichnen
    void drawGroupIntro();
    void drawMatchIntro();
    void drawGroupTable();
    void drawGroupResults();
    void drawGroupComplete();
    void drawKnockoutIntro();
    void drawBracket();        // K.O.-Bracket Uebersicht
    void drawEliminated();
    void drawChampion();

    // Bracket-Hilfsfunktionen
    int getMatchIndexForRound(int round, int matchInRound);
    void getTeamsForMatch(int matchIndex, int& team1, int& team2);
    int getMatchCountForRound(int round);
    const char* getRoundName(int round);
};


#endif
