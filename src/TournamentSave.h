#ifndef SRC_TOURNAMENTSAVE_H
#define SRC_TOURNAMENTSAVE_H

#include <Arduino.h>
#include "Teams.h"
#include "mathe.h"

struct TournamentSave {
  TournamentType type;       // DFB oder CL
  MatheMode mode;            // +/-, 1x1, 1:1
  int round;                 // Aktuelle Runde (0-basiert)
  String opponentName;       // Name des Gegners
  String opponentAbbrev;     // Abkürzung des Gegners
  uint16_t opponentColor;    // Farbe des Gegners

  TournamentSave() : type(TOURNAMENT_DFB_POKAL), mode(MATHE_PLUS_MINUS),
                     round(0), opponentName(""), opponentAbbrev(""), opponentColor(0) {}

  bool isValid() const {
    return opponentName.length() > 0;
  }
};

#endif
