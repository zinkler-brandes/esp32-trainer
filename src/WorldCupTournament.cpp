#include "WorldCupTournament.h"
#include "FlagDrawer.h"

WorldCupTournament::WorldCupTournament() {
  // Zurueck-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Anpfiff-Button
  kickoffButton = new Button(60, 180, 200, 45, "ANPFIFF!");
  kickoffButton->setColors(TFT_GREEN, 0x0000);

  // Weiter-Button
  continueButton = new Button(60, 180, 200, 45, "WEITER");
  continueButton->setColors(TFT_GREEN, 0x0000);

  // Zuschauer-Button
  spectatorButton = new Button(20, 180, 130, 45, "ZUSCHAUEN");
  spectatorButton->setColors(TFT_BLUE, TFT_WHITE);

  // Menue-Button
  menuButton = new Button(170, 180, 130, 45, "MENUE");
  menuButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Gruppen-Navigation
  prevGroupButton = new Button(260, 5, 25, 25, "<");
  prevGroupButton->setColors(TFT_BLUE, TFT_WHITE);

  nextGroupButton = new Button(290, 5, 25, 25, ">");
  nextGroupButton->setColors(TFT_BLUE, TFT_WHITE);

  // Bracket-Ansicht Button (oben rechts auf K.O.-Intro)
  bracketButton = new Button(250, 8, 60, 25, "BAUM");
  bracketButton->setColors(TFT_PURPLE, TFT_WHITE);

  // K.O.-Runden Navigation (im Bracket)
  prevRoundButton = new Button(10, 200, 40, 30, "<");
  prevRoundButton->setColors(TFT_BLUE, TFT_WHITE);

  nextRoundButton = new Button(270, 200, 40, 30, ">");
  nextRoundButton->setColors(TFT_BLUE, TFT_WHITE);

  _viewingGroupIndex = -1;  // Zeigt Spieler-Gruppe
  _viewingRound = 0;        // Zeigt aktuelle K.O.-Runde
}

void WorldCupTournament::init(MatheMode matheMode, int playerTeamIndex) {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);

  _matheMode = matheMode;
  _playerTeamIndex = playerTeamIndex;
  _playerGroupIndex = findGroupForTeam(playerTeamIndex);

  _state = WC_GROUP_INTRO;
  _groupMatchIndex = 0;
  _knockoutRound = -1;
  _currentOpponentIndex = -1;
  _isSpectator = false;
  _currentVenueIndex = 0;
  _viewingGroupIndex = -1;  // -1 = Spieler-Gruppe

  // Elfmeter zuruecksetzen
  _playerPenalties = 0;
  _cpuPenalties = 0;
  _penaltyRound = 0;

  // Bracket initialisieren
  for (int i = 0; i < 32; i++) _bracketTeams[i] = -1;
  for (int i = 0; i < 31; i++) {
    _bracketWinners[i] = -1;
    _bracketResults[i][0] = 0;
    _bracketResults[i][1] = 0;
    _bracketPlayed[i] = false;
  }
  _viewingRound = 0;

  // Alle 12 Gruppen initialisieren
  initAllGroups();

  // Spieler-Gruppentabelle referenzieren
  _groupTable.init(_playerGroupIndex);

  // Zufallsgenerator
  randomSeed(analogRead(34));

  Serial.printf("WorldCup: Init player=%s group=%c mode=%d\n",
    WM_TEAMS[_playerTeamIndex].abbrev,
    getGroupLetter(_playerGroupIndex),
    _matheMode);
}

void WorldCupTournament::draw() {
  switch (_state) {
    case WC_GROUP_INTRO:
      drawGroupIntro();
      break;
    case WC_MATCH_INTRO:
      drawMatchIntro();
      break;
    case WC_GROUP_TABLE:
      drawGroupTable();
      break;
    case WC_GROUP_COMPLETE:
      drawGroupComplete();
      break;
    case WC_KNOCKOUT_INTRO:
      drawKnockoutIntro();
      break;
    case WC_BRACKET_VIEW:
      drawBracket();
      break;
    case WC_ELIMINATED:
      drawEliminated();
      break;
    case WC_CHAMPION:
      drawChampion();
      break;
    default:
      break;
  }
}

int WorldCupTournament::handleTouch(int16_t x, int16_t y) {
  switch (_state) {
    case WC_GROUP_INTRO:
      if (backButton->contains(x, y)) return 0;
      if (continueButton->contains(x, y)) {
        selectNextGroupOpponent();
        selectVenueForMatch();
        _state = WC_MATCH_INTRO;
        draw();
        return -1;
      }
      break;

    case WC_MATCH_INTRO:
      if (backButton->contains(x, y)) return 0;
      if (kickoffButton->contains(x, y)) {
        return 1;  // Spiel starten
      }
      break;

    case WC_GROUP_TABLE:
      // Gruppen-Navigation (alle 12 Gruppen inkl. Spieler-Gruppe)
      if (prevGroupButton->contains(x, y)) {
        if (_viewingGroupIndex == -1) {
          // Von Spieler-Gruppe zur vorherigen
          _viewingGroupIndex = (_playerGroupIndex - 1 + WM_GROUP_COUNT) % WM_GROUP_COUNT;
        } else {
          _viewingGroupIndex = (_viewingGroupIndex - 1 + WM_GROUP_COUNT) % WM_GROUP_COUNT;
        }
        draw();
        return -1;
      }
      if (nextGroupButton->contains(x, y)) {
        if (_viewingGroupIndex == -1) {
          // Von Spieler-Gruppe zur naechsten
          _viewingGroupIndex = (_playerGroupIndex + 1) % WM_GROUP_COUNT;
        } else {
          _viewingGroupIndex = (_viewingGroupIndex + 1) % WM_GROUP_COUNT;
        }
        draw();
        return -1;
      }
      // Zurueck-Button: Zur Spieler-Gruppe zurueckkehren
      if (backButton->contains(x, y) && _viewingGroupIndex != -1 && _viewingGroupIndex != _playerGroupIndex) {
        _viewingGroupIndex = -1;
        draw();
        return -1;
      }
      if (continueButton->contains(x, y)) {
        // Nur fortfahren wenn Spieler-Gruppe angezeigt wird
        bool isPlayerGroup = (_viewingGroupIndex == -1 || _viewingGroupIndex == _playerGroupIndex);
        if (!isPlayerGroup) {
          _viewingGroupIndex = -1;  // Zurueck zur Spieler-Gruppe
          draw();
          return -1;
        }
        // Reset viewing index und weiter zum naechsten Spiel
        _viewingGroupIndex = -1;
        _groupMatchIndex++;
        if (_groupMatchIndex >= 3) {
          // Gruppenphase beendet
          _groupTable.calculateStandings();
          _playerGroupPosition = _groupTable.getTeamPosition(_playerTeamIndex);
          simulateOtherGroupMatches();  // Restliche Gruppenspiele der Spieler-Gruppe simulieren
          simulateAllOtherGroups();     // Alle anderen Gruppen komplett simulieren
          _state = WC_GROUP_COMPLETE;
        } else {
          // Naechstes Gruppenspiel
          selectNextGroupOpponent();
          selectVenueForMatch();
          _state = WC_MATCH_INTRO;
        }
        draw();
        return -1;
      }
      break;

    case WC_GROUP_COMPLETE:
      if (continueButton->contains(x, y)) {
        if (isPlayerQualifiedFromGroup()) {
          advanceToKnockout();
          _state = WC_KNOCKOUT_INTRO;
        } else {
          _state = WC_ELIMINATED;
        }
        draw();
        return -1;
      }
      break;

    case WC_KNOCKOUT_INTRO:
      if (backButton->contains(x, y)) return 0;
      if (bracketButton->contains(x, y)) {
        _viewingRound = _knockoutRound;  // Aktuelle Runde anzeigen
        _state = WC_BRACKET_VIEW;
        draw();
        return -1;
      }
      if (kickoffButton->contains(x, y)) {
        return 1;  // K.O.-Spiel starten
      }
      break;

    case WC_BRACKET_VIEW:
      // Zurueck zum K.O.-Intro
      if (backButton->contains(x, y)) {
        _state = WC_KNOCKOUT_INTRO;
        draw();
        return -1;
      }
      // Runden-Navigation
      if (prevRoundButton->contains(x, y) && _viewingRound > 0) {
        _viewingRound--;
        draw();
        return -1;
      }
      if (nextRoundButton->contains(x, y) && _viewingRound < 4) {
        _viewingRound++;
        draw();
        return -1;
      }
      // Anpfiff direkt aus Bracket-Ansicht (wenn eigene Runde)
      if (kickoffButton->contains(x, y) && _viewingRound == _knockoutRound) {
        _state = WC_KNOCKOUT_INTRO;  // Zurueck zum Intro fuer Anpfiff
        return 1;
      }
      break;

    case WC_ELIMINATED:
      if (spectatorButton->contains(x, y)) {
        startSpectatorMode();
        return 2;
      }
      if (menuButton->contains(x, y)) {
        return 0;  // Zurueck zum Menue
      }
      break;

    case WC_CHAMPION:
      if (continueButton->contains(x, y)) {
        return 0;  // Zurueck zum Menue
      }
      break;

    default:
      break;
  }

  return -1;
}

void WorldCupTournament::update() {
  // Aktuell keine regelmaessigen Updates benoetigt
}

// ========== GRUPPENPHASE ==========

void WorldCupTournament::selectNextGroupOpponent() {
  // Spielplan fuer Spieler (immer Team 0 in der Gruppe = Topf-1-Team):
  // Spiel 0: Spieler vs Team 1 (Topf 2)
  // Spiel 1: Spieler vs Team 2 (Topf 3)
  // Spiel 2: Spieler vs Team 3 (Topf 4)

  // Finde lokalen Index des Spielers in der Gruppe
  int playerLocal = -1;
  for (int i = 0; i < 4; i++) {
    if (WM_GROUPS[_playerGroupIndex].teamIndices[i] == _playerTeamIndex) {
      playerLocal = i;
      break;
    }
  }

  // Gegner ist das naechste Team in der Reihenfolge
  int opponentLocal = (playerLocal + _groupMatchIndex + 1) % 4;
  _currentOpponentIndex = WM_GROUPS[_playerGroupIndex].teamIndices[opponentLocal];

  Serial.printf("WorldCup: Group match %d - %s vs %s\n",
    _groupMatchIndex,
    WM_TEAMS[_playerTeamIndex].abbrev,
    WM_TEAMS[_currentOpponentIndex].abbrev);
}

void WorldCupTournament::handleGroupMatchResult(int playerGoals, int cpuGoals) {
  Serial.printf("WorldCup: Group match result %s %d:%d %s\n",
    WM_TEAMS[_playerTeamIndex].abbrev, playerGoals, cpuGoals,
    WM_TEAMS[_currentOpponentIndex].abbrev);

  // Ergebnis in Spieler-Tabelle eintragen
  _groupTable.addResult(_playerTeamIndex, _currentOpponentIndex, playerGoals, cpuGoals);
  _groupTable.calculateStandings();

  // Auch in _allGroups synchronisieren (fuer konsistente Navigation)
  _allGroups[_playerGroupIndex].addResult(_playerTeamIndex, _currentOpponentIndex, playerGoals, cpuGoals);
  _allGroups[_playerGroupIndex].calculateStandings();

  // Kompletten Spieltag fuer ALLE Gruppen simulieren
  simulateMatchday(_groupMatchIndex);

  _state = WC_GROUP_TABLE;
}

void WorldCupTournament::simulateMatchday(int matchday) {
  // Spieltag-Zuordnung (welche Matches pro Spieltag):
  // Spieltag 0: matches 0, 1 (0vs1, 2vs3)
  // Spieltag 1: matches 2, 3 (0vs2, 1vs3)
  // Spieltag 2: matches 4, 5 (0vs3, 1vs2)
  int matchA = matchday * 2;
  int matchB = matchday * 2 + 1;

  Serial.printf("WorldCup: Simulating matchday %d (matches %d, %d) for all groups\n",
    matchday + 1, matchA, matchB);

  // Spieler-Gruppe: nur das andere Spiel dieses Spieltags simulieren
  int playerLocal = -1;
  for (int i = 0; i < 4; i++) {
    if (WM_GROUPS[_playerGroupIndex].teamIndices[i] == _playerTeamIndex) {
      playerLocal = i;
      break;
    }
  }

  // Das andere Spiel in der Spieler-Gruppe
  for (int m = matchA; m <= matchB; m++) {
    int t1Local, t2Local;
    _groupTable.getMatchTeams(m, t1Local, t2Local);

    // Spieler-Spiel ueberspringen
    if (t1Local == playerLocal || t2Local == playerLocal) continue;
    if (_groupTable.hasMatchResult(m)) continue;  // Schon gespielt

    int team1 = WM_GROUPS[_playerGroupIndex].teamIndices[t1Local];
    int team2 = WM_GROUPS[_playerGroupIndex].teamIndices[t2Local];

    int result = simulateCPUMatch(team1, team2);
    int goals1 = result / 10;
    int goals2 = result % 10;

    // In beide Tabellen eintragen (Spieler-Gruppe + _allGroups)
    _groupTable.addResult(team1, team2, goals1, goals2);
    _allGroups[_playerGroupIndex].addResult(team1, team2, goals1, goals2);
  }
  _groupTable.calculateStandings();
  _allGroups[_playerGroupIndex].calculateStandings();

  // Alle anderen Gruppen: beide Spiele dieses Spieltags simulieren
  for (int g = 0; g < WM_GROUP_COUNT; g++) {
    if (g == _playerGroupIndex) continue;

    for (int m = matchA; m <= matchB; m++) {
      if (_allGroups[g].hasMatchResult(m)) continue;  // Schon gespielt

      int t1Local, t2Local;
      _allGroups[g].getMatchTeams(m, t1Local, t2Local);

      int team1 = WM_GROUPS[g].teamIndices[t1Local];
      int team2 = WM_GROUPS[g].teamIndices[t2Local];

      int result = simulateCPUMatch(team1, team2);
      _allGroups[g].addResult(team1, team2, result / 10, result % 10);
    }
    _allGroups[g].calculateStandings();
  }
}

void WorldCupTournament::simulateOtherGroupMatches() {
  // Simuliere alle Gruppenspiele die der Spieler nicht selbst gespielt hat
  int playerLocal = -1;
  for (int i = 0; i < 4; i++) {
    if (WM_GROUPS[_playerGroupIndex].teamIndices[i] == _playerTeamIndex) {
      playerLocal = i;
      break;
    }
  }

  // Alle 6 moeglichen Paarungen durchgehen
  for (int m = 0; m < 6; m++) {
    int t1Local, t2Local;
    _groupTable.getMatchTeams(m, t1Local, t2Local);

    // Spieler-Spiele ueberspringen (bereits eingetragen)
    if (t1Local == playerLocal || t2Local == playerLocal) continue;

    int team1 = WM_GROUPS[_playerGroupIndex].teamIndices[t1Local];
    int team2 = WM_GROUPS[_playerGroupIndex].teamIndices[t2Local];

    // CPU vs CPU simulieren
    int result = simulateCPUMatch(team1, team2);
    int goals1 = result / 10;
    int goals2 = result % 10;

    _groupTable.addResult(team1, team2, goals1, goals2);
  }

  _groupTable.calculateStandings();
}

void WorldCupTournament::initAllGroups() {
  // Alle 12 Gruppen initialisieren
  for (int g = 0; g < WM_GROUP_COUNT; g++) {
    _allGroups[g].init(g);
  }
  Serial.println("WorldCup: All 12 groups initialized");
}

void WorldCupTournament::simulateAllOtherGroups() {
  // Alle Gruppen außer der Spieler-Gruppe komplett simulieren
  for (int g = 0; g < WM_GROUP_COUNT; g++) {
    if (g == _playerGroupIndex) continue;  // Spieler-Gruppe überspringen

    // Alle 6 Spiele dieser Gruppe simulieren
    for (int m = 0; m < 6; m++) {
      int t1Local, t2Local;
      _allGroups[g].getMatchTeams(m, t1Local, t2Local);

      int team1 = WM_GROUPS[g].teamIndices[t1Local];
      int team2 = WM_GROUPS[g].teamIndices[t2Local];

      // CPU vs CPU simulieren
      int result = simulateCPUMatch(team1, team2);
      int goals1 = result / 10;
      int goals2 = result % 10;

      _allGroups[g].addResult(team1, team2, goals1, goals2);
    }

    _allGroups[g].calculateStandings();
    Serial.printf("WorldCup: Group %c simulated\n", getGroupLetter(g));
  }
}

int WorldCupTournament::simulateCPUMatch(int team1Index, int team2Index) {
  // Simuliere ein CPU vs CPU Spiel basierend auf Team-Tiers
  TeamTier tier1 = WM_TEAMS[team1Index].tier;
  TeamTier tier2 = WM_TEAMS[team2Index].tier;

  // Staerke-Differenz (-3 bis +3)
  int tierDiff = (int)tier2 - (int)tier1;  // positiv = Team 1 staerker

  // Basis-Torwahrscheinlichkeit
  int team1Strength = 50 + (tierDiff * 15);  // 5-95%
  team1Strength = constrain(team1Strength, 20, 80);

  int goals1 = 0, goals2 = 0;

  // 5 "Chancen" pro Team
  for (int i = 0; i < 5; i++) {
    if (random(0, 100) < team1Strength) goals1++;
    if (random(0, 100) < (100 - team1Strength)) goals2++;
  }

  // Ergebnis normalisieren (max 5 Tore)
  goals1 = min(goals1, 5);
  goals2 = min(goals2, 5);

  Serial.printf("WorldCup: CPU Match %s %d:%d %s\n",
    WM_TEAMS[team1Index].abbrev, goals1, goals2,
    WM_TEAMS[team2Index].abbrev);

  return goals1 * 10 + goals2;  // Encoded result
}

bool WorldCupTournament::isGroupPhaseComplete() {
  return _groupMatchIndex >= 3;
}

bool WorldCupTournament::isPlayerQualifiedFromGroup() {
  int pos = _groupTable.getTeamPosition(_playerTeamIndex);

  // Top 2 qualifiziert
  if (pos <= 2) return true;

  // Dritter: Abhaengig von "beste Dritte" Berechnung
  // Vereinfacht: 8 von 12 Dritten kommen weiter
  // Wir lassen die 4 Gruppen mit den schlechtesten Dritten raus
  if (pos == 3) {
    // Vereinfachte Regel: Spieler qualifiziert sich als Dritter wenn >= 3 Punkte
    GroupStanding standing = _groupTable.getStanding(2);  // Dritter Platz
    return standing.points >= 3;
  }

  return false;
}

// ========== K.O.-PHASE ==========

void WorldCupTournament::advanceToKnockout() {
  _knockoutRound = 0;  // Runde der 32

  // Bracket aufbauen (vereinfacht)
  buildKnockoutBracket();

  // Gegner auswaehlen
  selectNextKnockoutOpponent();
  selectVenueForMatch();

  Serial.printf("WorldCup: Advancing to knockout round %d\n", _knockoutRound);
}

void WorldCupTournament::buildKnockoutBracket() {
  // Vereinfachter Bracket-Aufbau
  // In der echten WM gibt es komplexe Regeln fuer die Paarungen
  // Hier: Zufaellige Auswahl aus anderen Gruppen

  int bracketIdx = 0;

  // Alle Gruppenersten und -zweiten + 8 beste Dritte
  for (int g = 0; g < WM_GROUP_COUNT; g++) {
    // Erstmal nur Erste und Zweite jeder Gruppe
    // (Vereinfachte Version ohne beste-Dritte-Logik)
    _bracketTeams[bracketIdx++] = WM_GROUPS[g].teamIndices[0];  // Vereinfacht
    if (bracketIdx < 32) {
      _bracketTeams[bracketIdx++] = WM_GROUPS[g].teamIndices[1];
    }
  }

  // Rest mit zufaelligen Dritten fuellen
  while (bracketIdx < 32) {
    int g = random(0, WM_GROUP_COUNT);
    _bracketTeams[bracketIdx++] = WM_GROUPS[g].teamIndices[2];
  }
}

void WorldCupTournament::selectNextKnockoutOpponent() {
  // Waehle einen Gegner basierend auf der K.O.-Runde
  // Regel: Gegner darf nicht aus der eigenen Gruppe sein (in R32)

  int attempts = 0;
  do {
    // Zufaelligen Index im Bracket waehlen
    int idx = random(0, 32);
    _currentOpponentIndex = _bracketTeams[idx];
    attempts++;
  } while (
    (_currentOpponentIndex == _playerTeamIndex ||
     (_knockoutRound == 0 && findGroupForTeam(_currentOpponentIndex) == _playerGroupIndex)) &&
    attempts < 50
  );

  // Fallback: Einfach irgendeinen anderen Gegner nehmen
  if (_currentOpponentIndex == _playerTeamIndex || _currentOpponentIndex < 0) {
    for (int i = 0; i < 32; i++) {
      if (_bracketTeams[i] != _playerTeamIndex && _bracketTeams[i] >= 0) {
        _currentOpponentIndex = _bracketTeams[i];
        break;
      }
    }
  }

  Serial.printf("WorldCup: K.O. Round %d - %s vs %s\n",
    _knockoutRound,
    WM_TEAMS[_playerTeamIndex].abbrev,
    WM_TEAMS[_currentOpponentIndex].abbrev);
}

void WorldCupTournament::handleKnockoutMatchResult(int playerGoals, int cpuGoals) {
  Serial.printf("WorldCup: K.O. match result %s %d:%d %s (round=%d)\n",
    WM_TEAMS[_playerTeamIndex].abbrev, playerGoals, cpuGoals,
    WM_TEAMS[_currentOpponentIndex].abbrev, _knockoutRound);

  if (playerGoals > cpuGoals) {
    // Gewonnen
    Serial.println("WorldCup: Player WON");
    if (_knockoutRound >= 4) {
      // Finale gewonnen!
      _state = WC_CHAMPION;
      Serial.println("WorldCup: State -> WC_CHAMPION");
    } else {
      // Naechste Runde
      _knockoutRound++;
      selectNextKnockoutOpponent();
      selectVenueForMatch();
      _state = WC_KNOCKOUT_INTRO;
      Serial.printf("WorldCup: State -> WC_KNOCKOUT_INTRO (round=%d)\n", _knockoutRound);
    }
  } else if (playerGoals < cpuGoals) {
    // Verloren
    Serial.println("WorldCup: Player LOST -> WC_ELIMINATED");
    _state = WC_ELIMINATED;
  } else {
    // Unentschieden - sollte hier nicht ankommen
    Serial.println("WorldCup: DRAW - should not happen here!");
  }
}

void WorldCupTournament::selectVenueForMatch() {
  if (_knockoutRound < 0) {
    // Gruppenphase
    VenueRegion region = getRegionForGroup(_playerGroupIndex);
    _currentVenueIndex = getRandomVenueInRegion(region);
  } else {
    // K.O.-Phase
    _currentVenueIndex = getVenueForKnockoutRound(_knockoutRound, 0);
  }
}

// ========== SCHWIERIGKEIT ==========

unsigned long WorldCupTournament::getAnswerTime() {
  if (_currentOpponentIndex < 0) return WM_ANSWER_TIME_TIER3;
  return getAnswerTimeForTier(WM_TEAMS[_currentOpponentIndex].tier);
}

int WorldCupTournament::getCpuPenaltyProb() {
  if (_currentOpponentIndex < 0) return WM_PENALTY_PROB_TIER3;
  return getPenaltyProbForTier(WM_TEAMS[_currentOpponentIndex].tier);
}

const char* WorldCupTournament::getCurrentRoundName() {
  if (_knockoutRound < 0) return "Gruppenphase";
  if (_knockoutRound <= 4) return WC_ROUND_NAMES[_knockoutRound];
  return "?";
}

// ========== ELFMETERSCHIESSEN ==========

void WorldCupTournament::startPenaltyShootout() {
  _playerPenalties = 0;
  _cpuPenalties = 0;
  _penaltyRound = 1;
}

bool WorldCupTournament::handlePenalty(bool playerScored, bool cpuScored) {
  if (playerScored) _playerPenalties++;
  if (cpuScored) _cpuPenalties++;

  _penaltyRound++;

  // Nach 3 Schuessen pruefen
  if (_penaltyRound > 3) {
    if (_playerPenalties != _cpuPenalties) {
      // Entschieden
      if (_playerPenalties > _cpuPenalties) {
        if (_knockoutRound >= 4) {
          _state = WC_CHAMPION;
        } else {
          _knockoutRound++;
          selectNextKnockoutOpponent();
          selectVenueForMatch();
          _state = WC_KNOCKOUT_INTRO;
        }
      } else {
        _state = WC_ELIMINATED;
      }
      return true;
    }
    // Sudden Death - weiter mit Runde 1
    _penaltyRound = 1;
  }

  return false;  // Weitermachen
}

// ========== ZUSCHAUER-MODUS ==========

void WorldCupTournament::startSpectatorMode() {
  _isSpectator = true;
  // TODO: Turnier bis zum Ende simulieren und anzeigen
}

void WorldCupTournament::simulateNextSpectatorMatch() {
  // TODO: Naechstes Spiel im Bracket simulieren
}

// ========== ZEICHNEN ==========

void WorldCupTournament::drawGroupIntro() {
  tft.fillScreen(TFT_BLACK);
  backButton->draw(&tft);

  // Titel
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 12);
  tft.print("WM 2026");

  // Gruppen-Info
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(60, 50);
  tft.printf("GRUPPE %c", getGroupLetter(_playerGroupIndex));

  // Teams in der Gruppe
  tft.setTextSize(2);
  int y = 95;
  for (int i = 0; i < 4; i++) {
    int teamIdx = WM_GROUPS[_playerGroupIndex].teamIndices[i];
    if (teamIdx == _playerTeamIndex) {
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft.setCursor(60, y);
    tft.print(WM_TEAMS[teamIdx].name);
    y += 22;
  }

  continueButton->draw(&tft);
}

void WorldCupTournament::drawMatchIntro() {
  tft.fillScreen(TFT_BLACK);
  backButton->draw(&tft);

  // Runden-Info
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 8);
  if (_knockoutRound < 0) {
    tft.printf("GRUPPE %c - SPIEL %d/3", getGroupLetter(_playerGroupIndex), _groupMatchIndex + 1);
  } else {
    tft.print(getCurrentRoundName());
  }

  // Spieltermin holen
  WMMatchDate matchDate;
  if (_knockoutRound < 0) {
    matchDate = getGroupMatchDate(_playerGroupIndex, _groupMatchIndex);
  } else {
    matchDate = getKnockoutMatchDate(_knockoutRound, 0);
  }

  // Datum und Wochentag
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(60, 28);
  tft.printf("%s, %02d.%02d.%04d", matchDate.weekday, matchDate.day, matchDate.month, matchDate.year);

  // Uhrzeiten (Lokal und Deutsch)
  uint8_t germanHour, germanMin;
  getGermanTime(matchDate, germanHour, germanMin);

  tft.setCursor(60, 40);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.printf("Lokal: %02d:%02d Uhr  |  DE: %02d:%02d Uhr",
    matchDate.hourLocal, matchDate.minuteLocal, germanHour, germanMin);

  // Stadion
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(60, 55);
  tft.printf("%s, %s", WM_VENUES[_currentVenueIndex].city, WM_VENUES[_currentVenueIndex].country);

  // Teams mit Flaggen
  int flagW = 48;
  int flagH = 32;
  int teamY = 72;

  // Spieler: Flagge + Name
  FlagDrawer::drawWMTeamFlag(&tft, 15, teamY, flagW, flagH, _playerTeamIndex);
  tft.setTextSize(2);
  tft.setCursor(70, teamY + 8);
  uint16_t playerColor = getTextColorForBg(WM_TEAMS[_playerTeamIndex].shirtColor);
  tft.setTextColor(playerColor, TFT_BLACK);
  tft.print(WM_TEAMS[_playerTeamIndex].name);

  // VS in der Mitte
  tft.setTextSize(2);
  tft.setCursor(140, teamY + flagH + 5);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("vs.");

  // Gegner: Flagge + Name
  int oppY = teamY + flagH + 25;
  FlagDrawer::drawWMTeamFlag(&tft, 15, oppY, flagW, flagH, _currentOpponentIndex);
  tft.setTextSize(2);
  tft.setCursor(70, oppY + 8);
  uint16_t opponentColor = getTextColorForBg(WM_TEAMS[_currentOpponentIndex].shirtColor);
  tft.setTextColor(opponentColor, TFT_BLACK);
  tft.print(WM_TEAMS[_currentOpponentIndex].name);

  // Schwierigkeitsinfo
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(60, 165);
  unsigned long time = getAnswerTime() / 1000;
  tft.printf("Zeit: %lu Sek pro Antwort", time);

  kickoffButton->draw(&tft);
}

void WorldCupTournament::drawGroupTable() {
  int displayGroupIndex;
  GroupTable* displayTable;

  // Bestimme welche Gruppe angezeigt wird
  bool isPlayerGroup = (_viewingGroupIndex == -1 || _viewingGroupIndex == _playerGroupIndex);

  if (isPlayerGroup) {
    // Spieler-Gruppe anzeigen (immer aus _groupTable, da diese live aktualisiert wird)
    displayGroupIndex = _playerGroupIndex;
    displayTable = &_groupTable;
  } else {
    // Andere Gruppe anzeigen
    displayGroupIndex = _viewingGroupIndex;
    displayTable = &_allGroups[_viewingGroupIndex];
  }

  // Tabelle zeichnen
  displayTable->draw(&tft, _playerTeamIndex);

  // Gruppen-Navigation Buttons (oben rechts)
  prevGroupButton->draw(&tft);
  nextGroupButton->draw(&tft);

  // Buttons unten
  if (isPlayerGroup) {
    // Auf Spieler-Gruppe: WEITER Button
    continueButton->draw(&tft);
  } else {
    // Auf anderer Gruppe: Zurueck zur Spieler-Gruppe
    backButton->draw(&tft);
  }
}

void WorldCupTournament::drawGroupComplete() {
  tft.fillScreen(TFT_BLACK);

  int pos = _groupTable.getTeamPosition(_playerTeamIndex);

  if (pos <= 2) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(40, 30);
    tft.print("QUALIFIZIERT!");
    tft.setTextSize(2);
    tft.setCursor(40, 60);
    tft.printf("Platz %d in Gruppe %c", pos, getGroupLetter(_playerGroupIndex));
  } else if (pos == 3 && isPlayerQualifiedFromGroup()) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 30);
    tft.print("BESTER DRITTER!");
    tft.setCursor(40, 60);
    tft.print("Weiter als Dritter");
  } else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(40, 30);
    tft.print("AUSGESCHIEDEN");
    tft.setCursor(40, 60);
    tft.printf("Platz %d in Gruppe %c", pos, getGroupLetter(_playerGroupIndex));
  }

  // Tabelle kompakt anzeigen
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  int y = 100;
  for (int i = 0; i < 4; i++) {
    GroupStanding s = _groupTable.getStanding(i);
    tft.setCursor(40, y);
    if (s.teamIndex == _playerTeamIndex) {
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft.printf("%d. %s  %dP  %d:%d", i + 1,
      WM_TEAMS[s.teamIndex].abbrev,
      s.points,
      s.goalsFor, s.goalsAgainst);
    y += 18;
  }

  continueButton->draw(&tft);
}

void WorldCupTournament::drawKnockoutIntro() {
  // Match-Intro zeichnen
  drawMatchIntro();

  // Bracket-Button oben rechts hinzufuegen
  bracketButton->draw(&tft);
}

void WorldCupTournament::drawEliminated() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(20, 30);
  tft.print("AUSGESCHIEDEN!");

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(30, 80);
  if (_knockoutRound < 0) {
    tft.print("In der Gruppenphase");
  } else {
    tft.printf("Im %s", getCurrentRoundName());
  }

  tft.setCursor(30, 110);
  tft.printf("gegen %s", WM_TEAMS[_currentOpponentIndex].name);

  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(30, 150);
  tft.print("Als Zuschauer weiterschauen?");

  spectatorButton->draw(&tft);
  menuButton->draw(&tft);
}

void WorldCupTournament::drawChampion() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(10, 20);
  tft.print("WELTMEISTER!");

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(30, 70);
  tft.print(WM_TEAMS[_playerTeamIndex].name);

  tft.setTextSize(2);
  tft.setCursor(60, 100);
  tft.print("WM 2026");

  // TODO: Pokal-Animation

  continueButton->draw(&tft);
}

// ========== SAVE/LOAD ==========

WorldCupSave WorldCupTournament::createSave() {
  WorldCupSave save;
  save.mode = _matheMode;
  save.playerTeamIndex = _playerTeamIndex;
  save.playerGroupIndex = _playerGroupIndex;
  save.groupMatchIndex = _groupMatchIndex;
  save.groupPhaseComplete = (_groupMatchIndex >= 3);
  save.showGroupTable = (_state == WC_GROUP_TABLE);  // Nach Spiel Tabelle zeigen
  save.knockoutRound = _knockoutRound;
  save.knockoutOpponentIndex = _currentOpponentIndex;
  save.isSpectator = _isSpectator;

  // Alle 12 Gruppen speichern
  for (int g = 0; g < WM_GROUP_COUNT; g++) {
    GroupTable* table = (g == _playerGroupIndex) ? &_groupTable : &_allGroups[g];
    save.allGroups[g].complete = table->isComplete();

    // Alle 6 Ergebnisse dieser Gruppe kopieren
    for (int m = 0; m < 6; m++) {
      GroupMatchResult result;
      int t1Local, t2Local;
      table->getMatchTeams(m, t1Local, t2Local);

      if (table->hasMatchResult(m)) {
        int goals1, goals2;
        table->getMatchResult(m, goals1, goals2);
        result.team1Goals = goals1;
        result.team2Goals = goals2;
        result.played = true;
      } else {
        result.played = false;
      }
      save.allGroups[g].results[m] = result;
    }
  }

  // Bracket kopieren
  for (int i = 0; i < 32; i++) save.bracketTeams[i] = _bracketTeams[i];
  for (int i = 0; i < 31; i++) {
    save.bracketWinners[i] = _bracketWinners[i];
  }

  Serial.printf("WorldCup: Save created - match %d, round %d\n",
    _groupMatchIndex, _knockoutRound);

  return save;
}

void WorldCupTournament::loadFromSave(const WorldCupSave& save) {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);

  _matheMode = save.mode;
  _playerTeamIndex = save.playerTeamIndex;
  _playerGroupIndex = save.playerGroupIndex;
  _groupMatchIndex = save.groupMatchIndex;
  _knockoutRound = save.knockoutRound;
  _currentOpponentIndex = save.knockoutOpponentIndex;
  _isSpectator = save.isSpectator;
  _viewingGroupIndex = -1;  // Spieler-Gruppe anzeigen

  // Elfmeter zuruecksetzen
  _playerPenalties = 0;
  _cpuPenalties = 0;
  _penaltyRound = 0;

  // Bracket laden
  for (int i = 0; i < 32; i++) _bracketTeams[i] = save.bracketTeams[i];
  for (int i = 0; i < 31; i++) _bracketWinners[i] = save.bracketWinners[i];

  // Alle 12 Gruppen initialisieren
  initAllGroups();

  // Spieler-Gruppentabelle initialisieren
  _groupTable.init(_playerGroupIndex);

  // Alle Gruppen laden
  for (int g = 0; g < WM_GROUP_COUNT; g++) {
    // Gespeicherte Ergebnisse laden
    for (int m = 0; m < 6; m++) {
      if (save.allGroups[g].results[m].played) {
        int t1Local, t2Local;
        _allGroups[g].getMatchTeams(m, t1Local, t2Local);
        int team1 = WM_GROUPS[g].teamIndices[t1Local];
        int team2 = WM_GROUPS[g].teamIndices[t2Local];
        int goals1 = save.allGroups[g].results[m].team1Goals;
        int goals2 = save.allGroups[g].results[m].team2Goals;

        // In _allGroups laden
        _allGroups[g].addResult(team1, team2, goals1, goals2);

        // Spieler-Gruppe auch in _groupTable laden
        if (g == _playerGroupIndex) {
          _groupTable.addResult(team1, team2, goals1, goals2);
        }
      }
    }

    _allGroups[g].calculateStandings();
  }
  _groupTable.calculateStandings();

  // Status bestimmen
  if (save.groupPhaseComplete) {
    if (_knockoutRound >= 0) {
      _state = WC_KNOCKOUT_INTRO;
      selectVenueForMatch();
    } else {
      _state = WC_GROUP_COMPLETE;
    }
  } else if (save.showGroupTable) {
    // Nach Spiel: Tabelle zuerst zeigen
    _state = WC_GROUP_TABLE;
  } else {
    _state = WC_MATCH_INTRO;
    selectNextGroupOpponent();
    selectVenueForMatch();
  }

  Serial.printf("WorldCup: Loaded save - player=%s match=%d round=%d\n",
    WM_TEAMS[_playerTeamIndex].abbrev, _groupMatchIndex, _knockoutRound);
}
