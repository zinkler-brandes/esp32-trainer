#ifndef SRC_WORLDCUPSAVE_H
#define SRC_WORLDCUPSAVE_H

#include <Arduino.h>
#include "mathe.h"
#include "WM2026Teams.h"
#include "WM2026Groups.h"

// Gruppenspiel-Ergebnis
struct GroupMatchResult {
  int team1Goals;
  int team2Goals;
  bool played;

  GroupMatchResult() : team1Goals(0), team2Goals(0), played(false) {}
};

// Tabellenstand eines Teams
struct GroupStanding {
  int teamIndex;
  int played;        // Gespielte Spiele
  int won;
  int drawn;
  int lost;
  int goalsFor;
  int goalsAgainst;
  int points;

  GroupStanding() : teamIndex(-1), played(0), won(0), drawn(0), lost(0),
                    goalsFor(0), goalsAgainst(0), points(0) {}

  int goalDiff() const { return goalsFor - goalsAgainst; }
};

// K.O.-Runden
enum WCKnockoutRound {
  WC_ROUND_GROUP = -1,
  WC_ROUND_OF_32 = 0,
  WC_ROUND_OF_16 = 1,
  WC_QUARTER = 2,
  WC_SEMI = 3,
  WC_FINAL = 4
};

// Rundennamen
const char* const WC_ROUND_NAMES[] = {
  "Runde der 32",
  "Achtelfinale",
  "Viertelfinale",
  "Halbfinale",
  "FINALE"
};

// Gruppen-Daten fuer Speicherung
struct GroupSaveData {
  GroupMatchResult results[6];   // Alle 6 Spiele (jeder gegen jeden)
  bool complete;

  GroupSaveData() : complete(false) {}
};

// Speicherstruktur fuer WM-Turnier
struct WorldCupSave {
  // Grunddaten
  MatheMode mode;                // Rechenart
  int playerTeamIndex;           // Index des Spieler-Teams (0-47)
  int playerGroupIndex;          // Gruppe (0-11 = A-L)

  // Gruppenphase - ALLE Gruppen (fuer Navigation)
  int groupMatchIndex;           // 0-2 (welches Gruppenspiel des Spielers)
  GroupSaveData allGroups[12];   // Alle 12 Gruppen mit Ergebnissen
  bool groupPhaseComplete;       // Alle Gruppen fertig?
  bool showGroupTable;           // Nach Spiel: Tabelle zuerst zeigen?

  // K.O.-Phase
  int knockoutRound;             // -1=Gruppe, 0=R32, 1=R16, 2=QF, 3=SF, 4=Final
  int knockoutMatchIndex;        // Welches Spiel in der Runde (fuer Bracket-Ansicht)
  int knockoutOpponentIndex;     // Team-Index des aktuellen K.O.-Gegners
  bool isSpectator;              // Zuschauer-Modus aktiv

  // Bracket-Tracking (fuer Zuschauer-Modus)
  int bracketTeams[32];          // Teams in Runde der 32
  int bracketWinners[31];        // Gewinner der K.O.-Spiele
  int bracketScores[31][2];      // Ergebnisse [Spiel][0=Team1, 1=Team2]

  // Konstruktor
  WorldCupSave() : mode(MATHE_PLUS_MINUS), playerTeamIndex(0), playerGroupIndex(0),
                   groupMatchIndex(0), groupPhaseComplete(false), showGroupTable(false),
                   knockoutRound(-1), knockoutMatchIndex(0), knockoutOpponentIndex(-1),
                   isSpectator(false) {
    for (int i = 0; i < 32; i++) bracketTeams[i] = -1;
    for (int i = 0; i < 31; i++) {
      bracketWinners[i] = -1;
      bracketScores[i][0] = 0;
      bracketScores[i][1] = 0;
    }
  }

  bool isValid() const {
    return playerTeamIndex >= 0 && playerTeamIndex < WM_TEAM_COUNT;
  }

  bool isGroupPhase() const {
    return knockoutRound < 0;
  }

  const char* getCurrentRoundName() const {
    if (knockoutRound < 0) return "Gruppenphase";
    if (knockoutRound <= 4) return WC_ROUND_NAMES[knockoutRound];
    return "?";
  }
};

#endif
