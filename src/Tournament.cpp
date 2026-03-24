#include "Tournament.h"

Tournament::Tournament() {
  // Zurück-Button (oben links, konsistent mit anderen Screens)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Anpfiff-Button
  kickoffButton = new Button(60, 180, 200, 45, "ANPFIFF!");
  kickoffButton->setColors(TFT_GREEN, 0x0000);

  // Weiter-Button
  continueButton = new Button(60, 180, 200, 45, "WEITER");
  continueButton->setColors(TFT_GREEN, 0x0000);

  // Buttons für Ausgeschieden-Screen
  retryButton = new Button(20, 180, 130, 45, "NOCHMAL");
  retryButton->setColors(TFT_BLUE, TFT_WHITE);

  menuButton = new Button(170, 180, 130, 45, "MENUE");
  menuButton->setColors(TFT_DARKGREY, TFT_WHITE);
}

void Tournament::init(TournamentType type, MatheMode matheMode) {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);

  _tournamentType = type;
  _matheMode = matheMode;
  _currentRound = 0;
  _state = TOURNAMENT_INTRO;
  _playerPenalties = 0;
  _cpuPenalties = 0;
  _penaltyRound = 0;
  _playerShot = false;

  // Zufallsgenerator initialisieren
  randomSeed(analogRead(34));

  // Ersten Gegner auswählen
  selectOpponent();
}

void Tournament::setPlayerTeam(const String& name, const String& abbrev, uint16_t color) {
  _playerTeamName = name;
  _playerTeamAbbrev = abbrev;
  _playerTeamColor = color;
  Serial.printf("Tournament: Player team set to '%s' (%s)\n", name.c_str(), abbrev.c_str());
}

void Tournament::draw() {
  switch (_state) {
    case TOURNAMENT_INTRO:
      drawIntro();
      break;
    case TOURNAMENT_ROUND_WON:
      drawRoundWon();
      break;
    case TOURNAMENT_ROUND_LOST:
      drawRoundLost();
      break;
    case TOURNAMENT_PENALTY:
      drawPenalty();
      break;
    case TOURNAMENT_CHAMPION:
      drawChampion();
      break;
    default:
      break;
  }
}

int Tournament::handleTouch(int16_t x, int16_t y) {
  // Zurück-Button nur bei INTRO und ROUND_LOST aktiv
  if ((_state == TOURNAMENT_INTRO || _state == TOURNAMENT_ROUND_LOST) && backButton->contains(x, y)) {
    return 0;  // Zurück zum Menü
  }

  switch (_state) {
    case TOURNAMENT_INTRO:
      if (kickoffButton->contains(x, y)) {
        return 1;  // Spiel starten
      }
      break;

    case TOURNAMENT_ROUND_WON:
      if (continueButton->contains(x, y)) {
        // Runde und Gegner wurden bereits in createSaveForNextRound() gesetzt
        // Nur noch State auf INTRO setzen
        _state = TOURNAMENT_INTRO;
        draw();
        return -1;
      }
      break;

    case TOURNAMENT_ROUND_LOST:
      if (retryButton->contains(x, y)) {
        // Turnier neu starten
        _currentRound = 0;
        _state = TOURNAMENT_INTRO;
        selectOpponent();
        draw();
        return -1;
      }
      if (menuButton->contains(x, y)) {
        return 0;  // Zurück zum Menü
      }
      break;

    case TOURNAMENT_CHAMPION:
      if (continueButton->contains(x, y)) {
        return 0;  // Zurück zum Menü
      }
      break;

    default:
      break;
  }

  return -1;
}

void Tournament::update() {
  // Aktuell keine regelmäßigen Updates benötigt
}

int Tournament::getTotalRounds() {
  return (_tournamentType == TOURNAMENT_DFB_POKAL) ? DFB_ROUNDS : CL_ROUNDS;
}

const char* Tournament::getCurrentRoundName() {
  if (_tournamentType == TOURNAMENT_DFB_POKAL) {
    return DFB_ROUND_NAMES[_currentRound];
  } else {
    return CL_ROUND_NAMES[_currentRound];
  }
}

void Tournament::selectOpponent() {
  const Team* cluster;
  int clusterSize;

  if (_tournamentType == TOURNAMENT_DFB_POKAL) {
    cluster = DFB_CLUSTERS[_currentRound];
    clusterSize = DFB_CLUSTER_SIZES[_currentRound];
  } else {
    cluster = CL_CLUSTERS[_currentRound];
    clusterSize = CL_CLUSTER_SIZES[_currentRound];
  }

  Serial.printf("selectOpponent: type=%d, round=%d, clusterSize=%d\n",
    _tournamentType, _currentRound, clusterSize);

  // Zufälligen Gegner aus dem Cluster wählen (nicht das eigene Team)
  int maxAttempts = 20;
  int attempts = 0;

  do {
    int idx = random(0, clusterSize);
    _currentOpponent = cluster[idx];
    attempts++;
  } while (String(_currentOpponent.name) == _playerTeamName && attempts < maxAttempts);

  // Falls kein anderer Gegner gefunden wurde, ersten nehmen der anders ist
  if (String(_currentOpponent.name) == _playerTeamName) {
    for (int i = 0; i < clusterSize; i++) {
      if (String(cluster[i].name) != _playerTeamName) {
        _currentOpponent = cluster[i];
        break;
      }
    }
  }

  Serial.printf("selectOpponent: Selected '%s' (%s)\n",
    _currentOpponent.name ? _currentOpponent.name : "NULL",
    _currentOpponent.abbrev ? _currentOpponent.abbrev : "NULL");
}

void Tournament::advanceToNextRound() {
  _currentRound++;
  if (_currentRound >= getTotalRounds()) {
    _state = TOURNAMENT_CHAMPION;
  } else {
    _state = TOURNAMENT_INTRO;
    selectOpponent();
  }
}

void Tournament::handleMatchResult(int playerGoals, int cpuGoals) {
  if (playerGoals > cpuGoals) {
    // Gewonnen - prüfen ob Finale war
    if (_currentRound >= getTotalRounds() - 1) {
      _state = TOURNAMENT_CHAMPION;
    } else {
      _state = TOURNAMENT_ROUND_WON;
    }
  } else if (playerGoals < cpuGoals) {
    // Verloren
    _state = TOURNAMENT_ROUND_LOST;
  } else {
    // Unentschieden -> Elfmeterschießen
    startPenaltyShootout();
  }
}

unsigned long Tournament::getAnswerTime() {
  if (_tournamentType == TOURNAMENT_DFB_POKAL) {
    // DFB: Runden 0-1 = Leicht, 2-3 = Mittel, 4-5 = Schwer
    if (_currentRound < 2) return ANSWER_TIME_EASY;
    if (_currentRound < 4) return ANSWER_TIME_MEDIUM;
    return ANSWER_TIME_HARD;
  } else {
    // CL: Runden 0-1 = Mittel, 2-3 = Schwer
    if (_currentRound < 2) return ANSWER_TIME_MEDIUM;
    return ANSWER_TIME_HARD;
  }
}

int Tournament::getCpuPenaltyProb() {
  if (_tournamentType == TOURNAMENT_DFB_POKAL) {
    if (_currentRound < 2) return CPU_PENALTY_PROB_EASY;
    if (_currentRound < 4) return CPU_PENALTY_PROB_MEDIUM;
    return CPU_PENALTY_PROB_HARD;
  } else {
    if (_currentRound < 2) return CPU_PENALTY_PROB_MEDIUM;
    return CPU_PENALTY_PROB_HARD;
  }
}

void Tournament::startPenaltyShootout() {
  _state = TOURNAMENT_PENALTY;
  _playerPenalties = 0;
  _cpuPenalties = 0;
  _penaltyRound = 1;
  _playerShot = false;
}

bool Tournament::handlePenalty(bool playerScored, bool cpuScored) {
  if (playerScored) {
    _playerPenalties++;
  }

  if (cpuScored) {
    _cpuPenalties++;
  }

  _penaltyRound++;

  // Nach 3 Schüssen prüfen
  if (_penaltyRound > 3) {
    if (_playerPenalties != _cpuPenalties) {
      // Entschieden
      if (_playerPenalties > _cpuPenalties) {
        if (_currentRound >= getTotalRounds() - 1) {
          _state = TOURNAMENT_CHAMPION;
        } else {
          _state = TOURNAMENT_ROUND_WON;
        }
      } else {
        _state = TOURNAMENT_ROUND_LOST;
      }
      return true;
    } else {
      // Nächste Runde (Sudden Death nach ersten 3)
      _penaltyRound = 1;  // Reset für nächste 3er-Runde
    }
  }

  return false;  // Shootout geht weiter
}

void Tournament::drawIntro() {
  tft.fillScreen(TFT_BLACK);

  // Zurück-Button (oben links)
  backButton->draw(&tft);

  // Turnier-Name (neben Zurück-Button)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 12);
  if (_tournamentType == TOURNAMENT_DFB_POKAL) {
    tft.print("DFB-POKAL");
  } else {
    tft.print("CHAMPIONS LEAGUE");
  }

  // Rundenname
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 50);
  tft.print(getCurrentRoundName());

  // Spieler-Team
  tft.setCursor(30, 80);
  tft.setTextColor(_playerTeamColor, TFT_BLACK);
  tft.print(_playerTeamName);

  // VS Anzeige
  tft.setTextSize(2);
  tft.setCursor(145, 100);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("vs.");

  // Gegner
  tft.setCursor(30, 120);
  tft.setTextColor(_currentOpponent.primaryColor, TFT_BLACK);
  tft.print(_currentOpponent.name);

  // Schwierigkeitsinfo
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(80, 155);
  unsigned long time = getAnswerTime() / 1000;
  tft.printf("Zeit: %lu Sek | 5 Schritte", time);

  // Anpfiff-Button
  kickoffButton->draw(&tft);
}

void Tournament::drawRoundWon() {
  tft.fillScreen(TFT_BLACK);
  // Kein backButton hier - nur WEITER ist möglich

  // Großer Text
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(30, 40);
  tft.print("GEWONNEN!");

  // Gegen wen (besiegter Gegner)
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(30, 90);
  tft.printf("gegen %s", _defeatedOpponent.name ? _defeatedOpponent.name : _currentOpponent.name);

  // Nächste Runde Info (aktuelle Runde ist bereits die naechste)
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(30, 130);
  if (_currentRound < getTotalRounds()) {
    tft.print("Naechste Runde:");
    tft.setCursor(30, 155);
    if (_tournamentType == TOURNAMENT_DFB_POKAL) {
      tft.print(DFB_ROUND_NAMES[_currentRound]);
    } else {
      tft.print(CL_ROUND_NAMES[_currentRound]);
    }
  }

  // Weiter-Button
  continueButton->draw(&tft);
}

void Tournament::drawRoundLost() {
  tft.fillScreen(TFT_BLACK);

  // Großer Text
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(20, 30);
  tft.print("AUSGESCHIEDEN!");

  // Gegen wen
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(30, 80);
  tft.printf("gegen %s", _currentOpponent.name);

  // Erreichte Runde
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(30, 120);
  tft.print("Erreicht: ");
  tft.print(getCurrentRoundName());

  // Buttons
  retryButton->draw(&tft);
  menuButton->draw(&tft);
}

void Tournament::drawPenalty() {
  tft.fillScreen(TFT_BLACK);
  backButton->draw(&tft);

  // Titel
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(40, 10);
  tft.print("ELFMETERSCHIESSEN!");

  // Stand
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(100, 60);
  tft.printf("%d : %d", _playerPenalties, _cpuPenalties);

  // Team-Namen statt Spielername
  tft.setTextSize(2);
  tft.setCursor(20, 100);
  tft.setTextColor(_playerTeamColor, TFT_BLACK);
  // Kürzen wenn zu lang
  String shortName = _playerTeamName;
  if (shortName.length() > 12) {
    shortName = shortName.substring(0, 10) + "..";
  }
  tft.print(shortName);

  tft.setCursor(200, 100);
  tft.setTextColor(_currentOpponent.primaryColor, TFT_BLACK);
  String oppName = String(_currentOpponent.name);
  if (oppName.length() > 10) {
    oppName = oppName.substring(0, 8) + "..";
  }
  tft.print(oppName);

  // Runde
  tft.setTextSize(2);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(100, 130);
  tft.printf("Schuss %d", _penaltyRound);

  // Hinweis
  tft.setTextSize(1);
  tft.setCursor(40, 160);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Loese die Aufgabe fuer deinen Elfmeter!");
}

void Tournament::drawChampion() {
  // Wird von TrophyDisplay übernommen
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(20, 30);
  tft.print("POKALSIEGER!");

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(50, 80);
  if (_tournamentType == TOURNAMENT_DFB_POKAL) {
    tft.print("DFB-POKAL");
  } else {
    tft.print("CHAMPIONS LEAGUE");
  }

  continueButton->draw(&tft);
}

void Tournament::drawTeamBadge(int x, int y, Team team) {
  // Einfacher Kreis in Teamfarben
  tft.fillCircle(x, y, 25, team.primaryColor);
  tft.drawCircle(x, y, 25, team.secondaryColor);
  tft.fillCircle(x, y, 15, team.secondaryColor);
  tft.fillCircle(x, y, 8, team.primaryColor);
}

// ========== SAVE/LOAD ==========

TournamentSave Tournament::createSave() {
  TournamentSave save;
  save.type = _tournamentType;
  save.mode = _matheMode;
  save.round = _currentRound;
  save.opponentName = _currentOpponent.name;
  save.opponentAbbrev = _currentOpponent.abbrev;
  save.opponentColor = _currentOpponent.primaryColor;
  return save;
}

TournamentSave Tournament::createSaveForNextRound() {
  // Besiegten Gegner und Runde fuer Anzeige speichern
  _defeatedOpponent = _currentOpponent;
  _wonRound = _currentRound;

  // Zur naechsten Runde wechseln
  _currentRound++;
  selectOpponent();  // Waehlt Gegner fuer naechste Runde

  // Save mit neuer Runde erstellen
  TournamentSave save;
  save.type = _tournamentType;
  save.mode = _matheMode;
  save.round = _currentRound;
  save.opponentName = _currentOpponent.name;
  save.opponentAbbrev = _currentOpponent.abbrev;
  save.opponentColor = _currentOpponent.primaryColor;

  // State bleibt TOURNAMENT_ROUND_WON fuer die Anzeige

  return save;
}

void Tournament::loadFromSave(const TournamentSave& save) {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);

  _tournamentType = save.type;
  _matheMode = save.mode;
  _currentRound = save.round;
  _state = TOURNAMENT_INTRO;
  _playerPenalties = 0;
  _cpuPenalties = 0;
  _penaltyRound = 0;
  _playerShot = false;

  // Gegner aus Save wiederherstellen - in statische Member kopieren
  _loadedOpponentName = save.opponentName;
  _loadedOpponentAbbrev = save.opponentAbbrev;

  // Falls Abkürzung fehlt (alter Save), im Cluster suchen
  if (_loadedOpponentAbbrev.length() == 0 && _loadedOpponentName.length() > 0) {
    const Team* cluster;
    int clusterSize;
    if (_tournamentType == TOURNAMENT_DFB_POKAL) {
      cluster = DFB_CLUSTERS[_currentRound];
      clusterSize = DFB_CLUSTER_SIZES[_currentRound];
    } else {
      cluster = CL_CLUSTERS[_currentRound];
      clusterSize = CL_CLUSTER_SIZES[_currentRound];
    }
    // Team im Cluster suchen
    for (int i = 0; i < clusterSize; i++) {
      if (String(cluster[i].name) == _loadedOpponentName) {
        _loadedOpponentAbbrev = cluster[i].abbrev;
        break;
      }
    }
    // Falls immer noch nicht gefunden, Fallback auf ersten 3 Buchstaben
    if (_loadedOpponentAbbrev.length() == 0) {
      _loadedOpponentAbbrev = _loadedOpponentName.substring(0, 3);
      _loadedOpponentAbbrev.toUpperCase();
    }
  }

  // Dann Pointer auf die statischen Strings setzen
  _currentOpponent.name = _loadedOpponentName.c_str();
  _currentOpponent.abbrev = _loadedOpponentAbbrev.c_str();
  _currentOpponent.primaryColor = save.opponentColor;
  _currentOpponent.secondaryColor = TFT_WHITE;

  Serial.printf("Tournament: Loaded from save - round %d vs %s (%s)\n",
    _currentRound, _currentOpponent.name, _currentOpponent.abbrev);
}
