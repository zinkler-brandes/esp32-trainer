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

  // Ergebnisse-Button (auf Gruppen-Screen)
  resultsButton = new Button(200, 5, 55, 25, "ERG");
  resultsButton->setColors(TFT_PURPLE, TFT_WHITE);

  // Scroll-Buttons fuer Ergebnisse (unten rechts)
  scrollUpButton = new Button(280, 170, 35, 30, "^");
  scrollUpButton->setColors(TFT_BLUE, TFT_WHITE);

  scrollDownButton = new Button(280, 205, 35, 30, "v");
  scrollDownButton->setColors(TFT_BLUE, TFT_WHITE);

  // Bracket-Pagination (rechts neben den Paarungen)
  bracketPrevButton = new Button(265, 60, 45, 35, "<");
  bracketPrevButton->setColors(TFT_BLUE, TFT_WHITE);

  bracketNextButton = new Button(265, 105, 45, 35, ">");
  bracketNextButton->setColors(TFT_BLUE, TFT_WHITE);

  _viewingGroupIndex = -1;  // Zeigt Spieler-Gruppe
  _viewingRound = 0;        // Zeigt aktuelle K.O.-Runde
  _resultsPage = 0;         // Erste Seite der Ergebnisse
  _bracketPage = 0;         // Erste Seite des Brackets
}

void WorldCupTournament::init(MatheMode matheMode, int playerTeamIndex) {
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
    _bracketPenalty[i] = false;
    _bracketPenaltyResults[i][0] = 0;
    _bracketPenaltyResults[i][1] = 0;
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
    case WC_GROUP_RESULTS:
      drawGroupResults();
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
      // Ergebnisse-Button
      if (resultsButton->contains(x, y)) {
        _resultsPage = 0;  // Immer bei Seite 0 starten
        _state = WC_GROUP_RESULTS;
        draw();
        return -1;
      }
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

    case WC_GROUP_RESULTS:
      // Zurueck zur Tabelle
      if (backButton->contains(x, y)) {
        _resultsPage = 0;  // Reset beim Verlassen
        _state = WC_GROUP_TABLE;
        draw();
        return -1;
      }
      // Scroll hoch
      if (_resultsPage > 0 && scrollUpButton->contains(x, y)) {
        _resultsPage--;
        draw();
        return -1;
      }
      // Scroll runter
      if (_resultsPage < 1 && scrollDownButton->contains(x, y)) {
        _resultsPage++;
        draw();
        return -1;
      }
      // Gruppen-Navigation auch auf Ergebnisse-Screen
      if (prevGroupButton->contains(x, y)) {
        _resultsPage = 0;  // Reset bei Gruppenwechsel
        if (_viewingGroupIndex == -1) {
          _viewingGroupIndex = (_playerGroupIndex - 1 + WM_GROUP_COUNT) % WM_GROUP_COUNT;
        } else {
          _viewingGroupIndex = (_viewingGroupIndex - 1 + WM_GROUP_COUNT) % WM_GROUP_COUNT;
        }
        draw();
        return -1;
      }
      if (nextGroupButton->contains(x, y)) {
        _resultsPage = 0;  // Reset bei Gruppenwechsel
        if (_viewingGroupIndex == -1) {
          _viewingGroupIndex = (_playerGroupIndex + 1) % WM_GROUP_COUNT;
        } else {
          _viewingGroupIndex = (_viewingGroupIndex + 1) % WM_GROUP_COUNT;
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

    case WC_BRACKET_VIEW: {
      // Zurueck zum K.O.-Intro
      if (backButton->contains(x, y)) {
        _state = WC_KNOCKOUT_INTRO;
        draw();
        return -1;
      }
      // Bracket-Pagination (4 Paarungen pro Seite)
      int matchCount = getMatchCountForRound(_viewingRound);
      int maxPages = (matchCount + 3) / 4;  // Aufgerundet, 4 pro Seite
      if (_bracketPage > 0 && bracketPrevButton->contains(x, y)) {
        _bracketPage--;
        draw();
        return -1;
      }
      if (_bracketPage < maxPages - 1 && bracketNextButton->contains(x, y)) {
        _bracketPage++;
        draw();
        return -1;
      }
      // Runden-Navigation
      if (prevRoundButton->contains(x, y) && _viewingRound > 0) {
        _viewingRound--;
        _bracketPage = 0;  // Reset bei Rundenwechsel
        draw();
        return -1;
      }
      if (nextRoundButton->contains(x, y) && _viewingRound < 4) {
        _viewingRound++;
        _bracketPage = 0;  // Reset bei Rundenwechsel
        draw();
        return -1;
      }
      // Anpfiff direkt aus Bracket-Ansicht (wenn eigene Runde)
      if (kickoffButton->contains(x, y) && _viewingRound == _knockoutRound) {
        _state = WC_KNOCKOUT_INTRO;  // Zurueck zum Intro fuer Anpfiff
        return 1;
      }
      break;
    }

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
  int team1Strength = 50 + (tierDiff * 12);

  // Tagesform: Zufaelliger Bonus/Malus fuer beide Teams (-10 bis +10)
  int form1 = random(-10, 11);
  int form2 = random(-10, 11);
  team1Strength += (form1 - form2);

  // Upset-Chance (10%): Schwächeres Team bekommt grossen Boost
  bool upset = (random(0, 100) < 10);
  if (upset) {
    if (tierDiff > 0) {
      // Team 1 ist staerker, Team 2 bekommt Upset-Bonus
      team1Strength -= random(20, 31);
    } else if (tierDiff < 0) {
      // Team 2 ist staerker, Team 1 bekommt Upset-Bonus
      team1Strength += random(20, 31);
    }
  }

  // Erweiterte Grenzen fuer mehr Varianz
  team1Strength = constrain(team1Strength, 12, 88);

  int goals1 = 0, goals2 = 0;

  // 4 "Chancen" pro Team (weniger = mehr Varianz)
  for (int i = 0; i < 4; i++) {
    if (random(0, 100) < team1Strength) goals1++;
    if (random(0, 100) < (100 - team1Strength)) goals2++;
  }

  // Ergebnis normalisieren (max 5 Tore)
  goals1 = min(goals1, 5);
  goals2 = min(goals2, 5);

  if (upset && tierDiff != 0) {
    Serial.printf("WorldCup: UPSET! %s %d:%d %s\n",
      WM_TEAMS[team1Index].abbrev, goals1, goals2,
      WM_TEAMS[team2Index].abbrev);
  } else {
    Serial.printf("WorldCup: CPU Match %s %d:%d %s\n",
      WM_TEAMS[team1Index].abbrev, goals1, goals2,
      WM_TEAMS[team2Index].abbrev);
  }

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
  // Sammle alle qualifizierten Teams aus den Gruppen

  int qualifiedTeams[32];
  int qualCount = 0;

  // Erste und Zweite jeder Gruppe (24 Teams)
  for (int g = 0; g < WM_GROUP_COUNT; g++) {
    GroupTable* table = (g == _playerGroupIndex) ? &_groupTable : &_allGroups[g];
    qualifiedTeams[qualCount++] = table->getStanding(0).teamIndex;  // Erster
    qualifiedTeams[qualCount++] = table->getStanding(1).teamIndex;  // Zweiter
  }

  // 8 beste Dritte (vereinfacht: alle Dritten mit >= 3 Punkten)
  for (int g = 0; g < WM_GROUP_COUNT && qualCount < 32; g++) {
    GroupTable* table = (g == _playerGroupIndex) ? &_groupTable : &_allGroups[g];
    GroupStanding third = table->getStanding(2);
    if (third.points >= 3) {
      qualifiedTeams[qualCount++] = third.teamIndex;
    }
  }

  // Falls noch nicht 32 Teams: zufaellig auffuellen
  while (qualCount < 32) {
    int g = random(0, WM_GROUP_COUNT);
    GroupTable* table = (g == _playerGroupIndex) ? &_groupTable : &_allGroups[g];
    qualifiedTeams[qualCount++] = table->getStanding(2).teamIndex;
  }

  // Spieler muss dabei sein!
  bool playerIncluded = false;
  for (int i = 0; i < 32; i++) {
    if (qualifiedTeams[i] == _playerTeamIndex) {
      playerIncluded = true;
      break;
    }
  }
  if (!playerIncluded) {
    qualifiedTeams[31] = _playerTeamIndex;  // Spieler am Ende einfuegen
  }

  // Bracket fuellen: Erste gegen Dritte, Zweite gegen Zweite (vereinfacht)
  // Spieler in die erste Haelfte des Brackets
  int playerPos = -1;
  for (int i = 0; i < 32; i++) {
    if (qualifiedTeams[i] == _playerTeamIndex) {
      playerPos = i;
      break;
    }
  }

  // Spieler an Position 0 verschieben (fuer einfachere Bracket-Verfolgung)
  if (playerPos > 0) {
    int temp = qualifiedTeams[0];
    qualifiedTeams[0] = qualifiedTeams[playerPos];
    qualifiedTeams[playerPos] = temp;
  }

  // In Bracket kopieren
  for (int i = 0; i < 32; i++) {
    _bracketTeams[i] = qualifiedTeams[i];
  }

  // Bracket-Results zuruecksetzen
  for (int i = 0; i < 31; i++) {
    _bracketWinners[i] = -1;
    _bracketResults[i][0] = 0;
    _bracketResults[i][1] = 0;
    _bracketPlayed[i] = false;
    _bracketPenalty[i] = false;
    _bracketPenaltyResults[i][0] = 0;
    _bracketPenaltyResults[i][1] = 0;
  }

  Serial.printf("WorldCup: Bracket built with %d teams, player at pos 0\n", qualCount);
}

void WorldCupTournament::selectNextKnockoutOpponent() {
  // Finde den korrekten Gegner basierend auf der Bracket-Struktur
  int matchCount = getMatchCountForRound(_knockoutRound);

  for (int m = 0; m < matchCount; m++) {
    int matchIdx = getMatchIndexForRound(_knockoutRound, m);
    int team1, team2;
    getTeamsForMatch(matchIdx, team1, team2);

    // Prüfe ob der Spieler in diesem Match ist
    if (team1 == _playerTeamIndex) {
      _currentOpponentIndex = team2;
      Serial.printf("WorldCup: K.O. Round %d Match %d - %s vs %s\n",
        _knockoutRound, m,
        WM_TEAMS[_playerTeamIndex].abbrev,
        WM_TEAMS[_currentOpponentIndex].abbrev);
      return;
    }
    if (team2 == _playerTeamIndex) {
      _currentOpponentIndex = team1;
      Serial.printf("WorldCup: K.O. Round %d Match %d - %s vs %s\n",
        _knockoutRound, m,
        WM_TEAMS[_playerTeamIndex].abbrev,
        WM_TEAMS[_currentOpponentIndex].abbrev);
      return;
    }
  }

  // Fallback (sollte nicht passieren)
  Serial.println("WorldCup: ERROR - Player not found in bracket!");
  _currentOpponentIndex = _bracketTeams[1];
}

void WorldCupTournament::handleKnockoutMatchResult(int playerGoals, int cpuGoals) {
  Serial.printf("WorldCup: K.O. match result %s %d:%d %s (round=%d)\n",
    WM_TEAMS[_playerTeamIndex].abbrev, playerGoals, cpuGoals,
    WM_TEAMS[_currentOpponentIndex].abbrev, _knockoutRound);

  // Finde das Spieler-Match im Bracket und speichere Ergebnis
  int matchCount = getMatchCountForRound(_knockoutRound);
  for (int m = 0; m < matchCount; m++) {
    int matchIdx = getMatchIndexForRound(_knockoutRound, m);
    int team1, team2;
    getTeamsForMatch(matchIdx, team1, team2);

    if ((team1 == _playerTeamIndex && team2 == _currentOpponentIndex) ||
        (team2 == _playerTeamIndex && team1 == _currentOpponentIndex)) {
      // Match gefunden - Ergebnis speichern
      if (team1 == _playerTeamIndex) {
        _bracketResults[matchIdx][0] = playerGoals;
        _bracketResults[matchIdx][1] = cpuGoals;
      } else {
        _bracketResults[matchIdx][0] = cpuGoals;
        _bracketResults[matchIdx][1] = playerGoals;
      }
      _bracketPlayed[matchIdx] = true;

      // Gewinner eintragen
      if (playerGoals > cpuGoals) {
        _bracketWinners[matchIdx] = _playerTeamIndex;
      } else {
        _bracketWinners[matchIdx] = _currentOpponentIndex;
      }

      Serial.printf("WorldCup: Bracket match %d recorded: %d:%d, winner=%s\n",
        matchIdx, _bracketResults[matchIdx][0], _bracketResults[matchIdx][1],
        WM_TEAMS[_bracketWinners[matchIdx]].abbrev);
      break;
    }
  }

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

void WorldCupTournament::startPenaltyShootout(int playerGoals, int cpuGoals) {
  _playerPenalties = 0;
  _cpuPenalties = 0;
  _penaltyRound = 1;

  // Regulaeres Ergebnis (Unentschieden) im Bracket speichern
  int matchCount = getMatchCountForRound(_knockoutRound);
  for (int m = 0; m < matchCount; m++) {
    int matchIdx = getMatchIndexForRound(_knockoutRound, m);
    int team1, team2;
    getTeamsForMatch(matchIdx, team1, team2);

    if ((team1 == _playerTeamIndex && team2 == _currentOpponentIndex) ||
        (team2 == _playerTeamIndex && team1 == _currentOpponentIndex)) {
      // Regulaeres Ergebnis speichern
      if (team1 == _playerTeamIndex) {
        _bracketResults[matchIdx][0] = playerGoals;
        _bracketResults[matchIdx][1] = cpuGoals;
      } else {
        _bracketResults[matchIdx][0] = cpuGoals;
        _bracketResults[matchIdx][1] = playerGoals;
      }
      break;
    }
  }
}

bool WorldCupTournament::handlePenalty(bool playerScored, bool cpuScored) {
  if (playerScored) _playerPenalties++;
  if (cpuScored) _cpuPenalties++;

  _penaltyRound++;

  // Nach 3 Schuessen pruefen
  if (_penaltyRound > 3) {
    if (_playerPenalties != _cpuPenalties) {
      // Entschieden - Elfmeter-Ergebnis separat speichern
      int matchCount = getMatchCountForRound(_knockoutRound);
      for (int m = 0; m < matchCount; m++) {
        int matchIdx = getMatchIndexForRound(_knockoutRound, m);
        int team1, team2;
        getTeamsForMatch(matchIdx, team1, team2);

        if ((team1 == _playerTeamIndex && team2 == _currentOpponentIndex) ||
            (team2 == _playerTeamIndex && team1 == _currentOpponentIndex)) {
          // Elfmeter-Ergebnis speichern
          _bracketPenalty[matchIdx] = true;
          if (team1 == _playerTeamIndex) {
            _bracketPenaltyResults[matchIdx][0] = _playerPenalties;
            _bracketPenaltyResults[matchIdx][1] = _cpuPenalties;
          } else {
            _bracketPenaltyResults[matchIdx][0] = _cpuPenalties;
            _bracketPenaltyResults[matchIdx][1] = _playerPenalties;
          }
          _bracketPlayed[matchIdx] = true;

          // Gewinner eintragen
          if (_playerPenalties > _cpuPenalties) {
            _bracketWinners[matchIdx] = _playerTeamIndex;
          } else {
            _bracketWinners[matchIdx] = _currentOpponentIndex;
          }
          break;
        }
      }

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

  // Ergebnisse-Button (oben)
  resultsButton->draw(&tft);

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

void WorldCupTournament::drawGroupResults() {
  int displayGroupIndex;
  GroupTable* displayTable;

  // Bestimme welche Gruppe angezeigt wird
  if (_viewingGroupIndex == -1 || _viewingGroupIndex == _playerGroupIndex) {
    displayGroupIndex = _playerGroupIndex;
    displayTable = &_groupTable;
  } else {
    displayGroupIndex = _viewingGroupIndex;
    displayTable = &_allGroups[_viewingGroupIndex];
  }

  // Ergebnisse zeichnen (mit Seite)
  displayTable->drawResults(&tft, _playerTeamIndex, _resultsPage);

  // Zurueck-Button
  backButton->draw(&tft);

  // Gruppen-Navigation
  prevGroupButton->draw(&tft);
  nextGroupButton->draw(&tft);

  // Scroll-Buttons (unten rechts)
  if (_resultsPage > 0) {
    scrollUpButton->draw(&tft);
  }
  if (_resultsPage < 1) {
    scrollDownButton->draw(&tft);
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
    save.bracketScores[i][0] = _bracketResults[i][0];
    save.bracketScores[i][1] = _bracketResults[i][1];
    save.bracketPlayed[i] = _bracketPlayed[i];
    save.bracketPenalty[i] = _bracketPenalty[i];
    save.bracketPenaltyScores[i][0] = _bracketPenaltyResults[i][0];
    save.bracketPenaltyScores[i][1] = _bracketPenaltyResults[i][1];
  }

  Serial.printf("WorldCup: Save created - match %d, round %d\n",
    _groupMatchIndex, _knockoutRound);

  return save;
}

void WorldCupTournament::loadFromSave(const WorldCupSave& save) {
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
  for (int i = 0; i < 31; i++) {
    _bracketWinners[i] = save.bracketWinners[i];
    _bracketResults[i][0] = save.bracketScores[i][0];
    _bracketResults[i][1] = save.bracketScores[i][1];
    _bracketPlayed[i] = save.bracketPlayed[i];
    _bracketPenalty[i] = save.bracketPenalty[i];
    _bracketPenaltyResults[i][0] = save.bracketPenaltyScores[i][0];
    _bracketPenaltyResults[i][1] = save.bracketPenaltyScores[i][1];
  }
  _viewingRound = (_knockoutRound >= 0) ? _knockoutRound : 0;

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

// ========== K.O.-BRACKET HILFSFUNKTIONEN ==========

int WorldCupTournament::getMatchCountForRound(int round) {
  // Anzahl Spiele pro Runde
  switch (round) {
    case 0: return 16;  // R32
    case 1: return 8;   // R16
    case 2: return 4;   // QF
    case 3: return 2;   // SF
    case 4: return 1;   // Final
    default: return 0;
  }
}

int WorldCupTournament::getMatchIndexForRound(int round, int matchInRound) {
  // Globaler Match-Index im Bracket
  int base = 0;
  for (int r = 0; r < round; r++) {
    base += getMatchCountForRound(r);
  }
  return base + matchInRound;
}

void WorldCupTournament::getTeamsForMatch(int matchIndex, int& team1, int& team2) {
  // Finde welche Runde dieser Match gehoert
  int round = 0;
  int idx = matchIndex;
  while (idx >= getMatchCountForRound(round)) {
    idx -= getMatchCountForRound(round);
    round++;
  }

  if (round == 0) {
    // R32: Teams kommen direkt aus _bracketTeams
    team1 = _bracketTeams[idx * 2];
    team2 = _bracketTeams[idx * 2 + 1];
  } else {
    // Spaetere Runden: Teams kommen aus vorherigen Gewinnern
    int prevRoundBase = getMatchIndexForRound(round - 1, 0);
    int match1 = prevRoundBase + idx * 2;
    int match2 = prevRoundBase + idx * 2 + 1;
    team1 = _bracketWinners[match1];
    team2 = _bracketWinners[match2];
  }
}

const char* WorldCupTournament::getRoundName(int round) {
  switch (round) {
    case 0: return "RUNDE DER 32";
    case 1: return "ACHTELFINALE";
    case 2: return "VIERTELFINALE";
    case 3: return "HALBFINALE";
    case 4: return "FINALE";
    default: return "?";
  }
}

void WorldCupTournament::drawBracket() {
  tft.fillScreen(TFT_BLACK);

  // Header
  backButton->draw(&tft);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 10);
  tft.print(getRoundName(_viewingRound));

  // Matches dieser Runde anzeigen
  int matchCount = getMatchCountForRound(_viewingRound);
  int startY = 45;
  int rowHeight = 32;  // Mehr Platz pro Paarung

  // Maximal 4 Matches pro Seite
  int matchesPerPage = 4;
  int displayCount = min(matchCount - _bracketPage * matchesPerPage, matchesPerPage);
  int displayOffset = _bracketPage * matchesPerPage;

  // Anzahl der Seiten fuer diese Runde
  int maxPages = (matchCount + matchesPerPage - 1) / matchesPerPage;

  // Pagination-Buttons rechts anzeigen (nur wenn noetig)
  if (maxPages > 1) {
    if (_bracketPage > 0) {
      bracketPrevButton->draw(&tft);
    }
    if (_bracketPage < maxPages - 1) {
      bracketNextButton->draw(&tft);
    }

    // Seiten-Anzeige rechts
    tft.setTextSize(1);
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.setCursor(270, 150);
    tft.printf("%d/%d", _bracketPage + 1, maxPages);
  }

  // Runden-Navigation unten
  if (_viewingRound > 0) {
    prevRoundButton->draw(&tft);
  }
  if (_viewingRound < 4) {
    nextRoundButton->draw(&tft);
  }

  tft.setTextSize(1);

  for (int i = 0; i < displayCount; i++) {
    int matchIdx = getMatchIndexForRound(_viewingRound, displayOffset + i);
    int team1, team2;
    getTeamsForMatch(matchIdx, team1, team2);

    int y = startY + i * rowHeight;

    // Spieler-Match hervorheben
    bool isPlayerMatch = (team1 == _playerTeamIndex || team2 == _playerTeamIndex);
    if (isPlayerMatch) {
      tft.fillRect(5, y - 4, 255, rowHeight - 2, 0x0320);  // Dunkelgruen
    }

    // Team 1
    if (team1 >= 0) {
      tft.setTextColor(TFT_WHITE, isPlayerMatch ? 0x0320 : TFT_BLACK);
      tft.setCursor(10, y);
      tft.print(WM_TEAMS[team1].abbrev);

      // Mini-Flagge
      FlagDrawer::drawWMTeamFlag(&tft, 45, y - 3, 22, 15, team1);
    } else {
      tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
      tft.setCursor(10, y);
      tft.print("???");
    }

    // Ergebnis oder "vs"
    tft.setCursor(115, y);
    if (_bracketPlayed[matchIdx]) {
      // Ergebnis anzeigen
      tft.setTextColor(TFT_CYAN, isPlayerMatch ? 0x0320 : TFT_BLACK);
      tft.printf("%d : %d", _bracketResults[matchIdx][0], _bracketResults[matchIdx][1]);

      // Bei Elfmeter: zusaetzlich Elfmeter-Stand anzeigen
      if (_bracketPenalty[matchIdx]) {
        tft.setTextColor(TFT_YELLOW, isPlayerMatch ? 0x0320 : TFT_BLACK);
        tft.setCursor(100, y + 12);
        tft.printf("n.E. %d:%d", _bracketPenaltyResults[matchIdx][0], _bracketPenaltyResults[matchIdx][1]);
      }
    } else if (_viewingRound == _knockoutRound && isPlayerMatch) {
      // Spieler's naechstes Spiel
      tft.setTextColor(TFT_GREEN, 0x0320);
      tft.print("vs");
    } else {
      tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
      tft.print("-:-");
    }

    // Team 2
    if (team2 >= 0) {
      tft.setTextColor(TFT_WHITE, isPlayerMatch ? 0x0320 : TFT_BLACK);
      tft.setCursor(170, y);
      tft.print(WM_TEAMS[team2].abbrev);

      // Mini-Flagge
      FlagDrawer::drawWMTeamFlag(&tft, 205, y - 3, 22, 15, team2);
    } else {
      tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
      tft.setCursor(170, y);
      tft.print("???");
    }
  }

  // Anpfiff-Button wenn eigene Runde und noch nicht gespielt
  if (_viewingRound == _knockoutRound && _state == WC_BRACKET_VIEW) {
    kickoffButton->draw(&tft);
  }
}
