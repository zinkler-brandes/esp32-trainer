#include "GroupTable.h"
#include "FlagDrawer.h"

GroupTable::GroupTable() : _groupIndex(0) {
}

void GroupTable::init(int groupIndex) {
  _groupIndex = groupIndex;

  // Standings initialisieren
  for (int i = 0; i < 4; i++) {
    _standings[i] = GroupStanding();
    _standings[i].teamIndex = WM_GROUPS[_groupIndex].teamIndices[i];
  }

  // Results zuruecksetzen
  for (int i = 0; i < 6; i++) {
    _results[i] = GroupMatchResult();
  }
}

int GroupTable::getLocalIndex(int teamIndex) {
  for (int i = 0; i < 4; i++) {
    if (WM_GROUPS[_groupIndex].teamIndices[i] == teamIndex) {
      return i;
    }
  }
  return -1;
}

int GroupTable::getMatchIndex(int team1Local, int team2Local) {
  // Spielplan fuer 4er-Gruppe (6 Spiele):
  // Match 0: Team 0 vs Team 1
  // Match 1: Team 2 vs Team 3
  // Match 2: Team 0 vs Team 2
  // Match 3: Team 1 vs Team 3
  // Match 4: Team 0 vs Team 3
  // Match 5: Team 1 vs Team 2

  // Sortiere die Indices damit Suche eindeutig ist
  int a = min(team1Local, team2Local);
  int b = max(team1Local, team2Local);

  if (a == 0 && b == 1) return 0;
  if (a == 2 && b == 3) return 1;
  if (a == 0 && b == 2) return 2;
  if (a == 1 && b == 3) return 3;
  if (a == 0 && b == 3) return 4;
  if (a == 1 && b == 2) return 5;

  return -1;  // Sollte nicht passieren
}

void GroupTable::getMatchTeams(int localMatch, int& team1Local, int& team2Local) {
  // Spielplan (gleiche Reihenfolge wie getMatchIndex)
  switch (localMatch) {
    case 0: team1Local = 0; team2Local = 1; break;
    case 1: team1Local = 2; team2Local = 3; break;
    case 2: team1Local = 0; team2Local = 2; break;
    case 3: team1Local = 1; team2Local = 3; break;
    case 4: team1Local = 0; team2Local = 3; break;
    case 5: team1Local = 1; team2Local = 2; break;
    default: team1Local = 0; team2Local = 1; break;
  }
}

void GroupTable::addResult(int team1Index, int team2Index, int goals1, int goals2) {
  int local1 = getLocalIndex(team1Index);
  int local2 = getLocalIndex(team2Index);

  if (local1 < 0 || local2 < 0) {
    Serial.printf("GroupTable: Team not in group! team1=%d, team2=%d\n", team1Index, team2Index);
    return;
  }

  int matchIdx = getMatchIndex(local1, local2);
  if (matchIdx < 0) return;

  // WICHTIG: getMatchTeams() erwartet die Tore in der Reihenfolge
  // (kleinerer local Index, groesserer local Index)
  // Da getMatchIndex() die Indices sortiert, muessen wir die Tore
  // entsprechend anordnen
  if (local1 < local2) {
    // Normale Reihenfolge
    _results[matchIdx].team1Goals = goals1;
    _results[matchIdx].team2Goals = goals2;
  } else {
    // Umgekehrte Reihenfolge - Tore tauschen!
    _results[matchIdx].team1Goals = goals2;
    _results[matchIdx].team2Goals = goals1;
  }
  _results[matchIdx].played = true;

  Serial.printf("GroupTable: Added result match %d: %s %d:%d %s (stored as %d:%d)\n",
    matchIdx,
    WM_TEAMS[team1Index].abbrev,
    goals1, goals2,
    WM_TEAMS[team2Index].abbrev,
    _results[matchIdx].team1Goals,
    _results[matchIdx].team2Goals);
}

void GroupTable::calculateStandings() {
  // Standings komplett zuruecksetzen (inkl. teamIndex!)
  // Wichtig: teamIndex muss auf lokale Position zurueckgesetzt werden,
  // da die Standings nach dem Sortieren durcheinander sind
  for (int i = 0; i < 4; i++) {
    _standings[i].teamIndex = WM_GROUPS[_groupIndex].teamIndices[i];
    _standings[i].played = 0;
    _standings[i].won = 0;
    _standings[i].drawn = 0;
    _standings[i].lost = 0;
    _standings[i].goalsFor = 0;
    _standings[i].goalsAgainst = 0;
    _standings[i].points = 0;
  }

  // Alle gespielten Matches durchgehen
  for (int m = 0; m < 6; m++) {
    if (!_results[m].played) continue;

    int t1Local, t2Local;
    getMatchTeams(m, t1Local, t2Local);

    int g1 = _results[m].team1Goals;
    int g2 = _results[m].team2Goals;

    // Team 1 Stats
    _standings[t1Local].played++;
    _standings[t1Local].goalsFor += g1;
    _standings[t1Local].goalsAgainst += g2;

    // Team 2 Stats
    _standings[t2Local].played++;
    _standings[t2Local].goalsFor += g2;
    _standings[t2Local].goalsAgainst += g1;

    if (g1 > g2) {
      // Team 1 gewinnt
      _standings[t1Local].won++;
      _standings[t1Local].points += 3;
      _standings[t2Local].lost++;
    } else if (g1 < g2) {
      // Team 2 gewinnt
      _standings[t2Local].won++;
      _standings[t2Local].points += 3;
      _standings[t1Local].lost++;
    } else {
      // Unentschieden
      _standings[t1Local].drawn++;
      _standings[t1Local].points++;
      _standings[t2Local].drawn++;
      _standings[t2Local].points++;
    }
  }

  // Nach Punkten sortieren (Bubble Sort reicht fuer 4 Teams)
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3 - i; j++) {
      bool swap = false;

      // Primaer: Punkte
      if (_standings[j].points < _standings[j+1].points) {
        swap = true;
      } else if (_standings[j].points == _standings[j+1].points) {
        // Sekundaer: Tordifferenz
        if (_standings[j].goalDiff() < _standings[j+1].goalDiff()) {
          swap = true;
        } else if (_standings[j].goalDiff() == _standings[j+1].goalDiff()) {
          // Tertiaer: Geschossene Tore
          if (_standings[j].goalsFor < _standings[j+1].goalsFor) {
            swap = true;
          }
        }
      }

      if (swap) {
        GroupStanding temp = _standings[j];
        _standings[j] = _standings[j+1];
        _standings[j+1] = temp;
      }
    }
  }
}

void GroupTable::draw(TFT_eSPI* tft, int playerTeamIndex) {
  // Hintergrund
  tft->fillScreen(TFT_BLACK);

  // Titel
  tft->setTextColor(TFT_YELLOW, TFT_BLACK);
  tft->setTextSize(2);
  tft->setCursor(100, 10);
  tft->printf("GRUPPE %c", getGroupLetter());

  // Tabellenkopf
  int startY = 45;
  int rowHeight = 28;

  tft->setTextSize(1);
  tft->setTextColor(TFT_DARKGREY, TFT_BLACK);
  // Spaltenkoepfe an gleichen X-Positionen wie Daten
  tft->setCursor(80, startY);
  tft->print("Sp  S  U  N");
  tft->setCursor(175, startY);
  tft->print("Tore");
  tft->setCursor(218, startY);
  tft->print("Diff");
  tft->setTextColor(TFT_YELLOW, TFT_BLACK);  // Punkte hervorheben
  tft->setCursor(268, startY);
  tft->print("Pkt");

  // Trennlinie
  tft->drawLine(10, startY + 12, 310, startY + 12, TFT_DARKGREY);

  // Teams
  tft->setTextSize(2);
  for (int i = 0; i < 4; i++) {
    int y = startY + 20 + (i * rowHeight);
    int teamIdx = _standings[i].teamIndex;

    // Hintergrund fuer Spieler-Team
    if (teamIdx == playerTeamIndex) {
      tft->fillRect(5, y - 2, 310, rowHeight - 2, 0x0320);  // Dunkelgruen
    }

    // Mini-Flagge links
    FlagDrawer::drawWMTeamFlag(tft, 8, y, 24, 16, teamIdx);

    // Platzierung (1-4)
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    if (teamIdx == playerTeamIndex) {
      tft->setTextColor(TFT_WHITE, 0x0320);
    }

    // Qualifikations-Markierung
    if (i < 2) {
      // Top 2 qualifiziert
      tft->setTextColor(TFT_GREEN, TFT_BLACK);
      if (teamIdx == playerTeamIndex) tft->setTextColor(TFT_GREEN, 0x0320);
    } else if (i == 2) {
      // Dritter - evtl. als bester Dritter
      tft->setTextColor(TFT_YELLOW, TFT_BLACK);
      if (teamIdx == playerTeamIndex) tft->setTextColor(TFT_YELLOW, 0x0320);
    } else {
      // Vierter - ausgeschieden
      tft->setTextColor(TFT_RED, TFT_BLACK);
      if (teamIdx == playerTeamIndex) tft->setTextColor(TFT_RED, 0x0320);
    }

    // Team-Name (Abkuerzung) - nach rechts verschoben fuer Flagge
    tft->setCursor(36, y);
    tft->print(WM_TEAMS[teamIdx].abbrev);

    // Stats in kompakter Form
    tft->setTextSize(1);
    if (teamIdx == playerTeamIndex) {
      tft->setTextColor(TFT_WHITE, 0x0320);
    } else {
      tft->setTextColor(TFT_WHITE, TFT_BLACK);
    }

    // Sp S U N
    tft->setCursor(80, y + 4);
    tft->printf("%2d %2d %2d %2d",
      _standings[i].played,
      _standings[i].won,
      _standings[i].drawn,
      _standings[i].lost);

    // Tore
    tft->setCursor(175, y + 4);
    tft->printf("%2d:%2d",
      _standings[i].goalsFor,
      _standings[i].goalsAgainst);

    // Tordifferenz
    int diff = _standings[i].goalDiff();
    tft->setCursor(220, y + 4);
    if (diff > 0) {
      tft->printf("+%d", diff);
    } else {
      tft->printf("%d", diff);
    }

    // Punkte (groesser)
    tft->setTextSize(2);
    tft->setCursor(270, y);
    tft->printf("%2d", _standings[i].points);
  }

}

void GroupTable::drawResults(TFT_eSPI* tft, int playerTeamIndex, int page) {
  tft->fillScreen(TFT_BLACK);

  // Titel
  tft->setTextColor(TFT_YELLOW, TFT_BLACK);
  tft->setTextSize(2);
  tft->setCursor(60, 10);
  tft->printf("ERGEBNISSE %c", getGroupLetter());

  // Spieltag-Labels
  const char* matchdayLabels[] = {"Spieltag 1", "Spieltag 2", "Spieltag 3"};

  // Seite 0: Spieltag 1+2, Seite 1: Spieltag 3
  int startDay = (page == 0) ? 0 : 2;
  int endDay = (page == 0) ? 2 : 3;

  int y = 45;
  int rowHeight = 32;

  for (int day = startDay; day < endDay; day++) {
    // Spieltag-Header
    tft->setTextSize(1);
    tft->setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft->setCursor(10, y);
    tft->print(matchdayLabels[day]);
    y += 16;

    // 2 Spiele pro Spieltag
    for (int m = 0; m < 2; m++) {
      int matchIdx = day * 2 + m;
      int t1Local, t2Local;
      getMatchTeams(matchIdx, t1Local, t2Local);

      int team1 = WM_GROUPS[_groupIndex].teamIndices[t1Local];
      int team2 = WM_GROUPS[_groupIndex].teamIndices[t2Local];

      bool isPlayerMatch = (team1 == playerTeamIndex || team2 == playerTeamIndex);

      // Hintergrund fuer Spieler-Spiele
      if (isPlayerMatch) {
        tft->fillRect(5, y - 2, 310, rowHeight - 4, 0x0320);
      }

      tft->setTextSize(2);
      uint16_t bgColor = isPlayerMatch ? 0x0320 : TFT_BLACK;

      // Flagge Team 1
      FlagDrawer::drawWMTeamFlag(tft, 10, y, 24, 16, team1);

      // Team 1 Name (etwas mehr Platz)
      tft->setTextColor(TFT_WHITE, bgColor);
      tft->setCursor(40, y);
      tft->print(WM_TEAMS[team1].abbrev);

      // Ergebnis oder "- : -" (zentrierter)
      tft->setCursor(130, y);
      if (_results[matchIdx].played) {
        tft->setTextColor(TFT_YELLOW, bgColor);
        tft->printf("%d : %d", _results[matchIdx].team1Goals, _results[matchIdx].team2Goals);
      } else {
        tft->setTextColor(TFT_DARKGREY, bgColor);
        tft->print("- : -");
      }

      // Flagge Team 2
      FlagDrawer::drawWMTeamFlag(tft, 210, y, 24, 16, team2);

      // Team 2 Name
      tft->setTextColor(TFT_WHITE, bgColor);
      tft->setCursor(240, y);
      tft->print(WM_TEAMS[team2].abbrev);

      y += rowHeight;
    }

    y += 8;  // Abstand zwischen Spieltagen
  }
}

int GroupTable::getTeamPosition(int teamIndex) {
  for (int i = 0; i < 4; i++) {
    if (_standings[i].teamIndex == teamIndex) {
      return i + 1;  // 1-basiert
    }
  }
  return 0;
}

bool GroupTable::isTeamQualified(int teamIndex, bool* asBestThird) {
  int pos = getTeamPosition(teamIndex);

  if (asBestThird) *asBestThird = false;

  // Top 2 sind immer qualifiziert
  if (pos <= 2) return true;

  // Dritter koennte als bester Dritter weiterkommen
  // (wird spaeter in WorldCupTournament entschieden)
  if (pos == 3 && asBestThird) {
    *asBestThird = true;
  }

  return false;
}

// ========== SPEICHERN/LADEN HILFSFUNKTIONEN ==========

bool GroupTable::isComplete() {
  // Alle 6 Spiele muessen gespielt sein
  for (int i = 0; i < 6; i++) {
    if (!_results[i].played) return false;
  }
  return true;
}

bool GroupTable::hasMatchResult(int matchIndex) {
  if (matchIndex < 0 || matchIndex >= 6) return false;
  return _results[matchIndex].played;
}

void GroupTable::getMatchResult(int matchIndex, int& goals1, int& goals2) {
  if (matchIndex < 0 || matchIndex >= 6) {
    goals1 = 0;
    goals2 = 0;
    return;
  }
  goals1 = _results[matchIndex].team1Goals;
  goals2 = _results[matchIndex].team2Goals;
}
