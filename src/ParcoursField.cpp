#include "ParcoursField.h"

ParcoursField::ParcoursField() : _tft(nullptr), _riderName("REITER"),
                                  _horseName("PFERD"),
                                  _faults(0), _totalTime(0.0),
                                  _obstacleCount(6), _currentObstacle(0),
                                  _tournamentName("") {
  for (int i = 0; i < 12; i++) {
    _obstacleStatus[i] = OBSTACLE_PENDING;
  }
}

void ParcoursField::setDisplay(TFT_eSPI* display) {
  _tft = display;
}

void ParcoursField::setRiderName(const String& name) {
  _riderName = name;
  if (_riderName.length() > 10) {
    _riderName = _riderName.substring(0, 10);
  }
  _riderName.toUpperCase();
}

void ParcoursField::setHorseName(const String& name) {
  _horseName = name;
  if (_horseName.length() > 10) {
    _horseName = _horseName.substring(0, 10);
  }
  _horseName.toUpperCase();
}

void ParcoursField::setTournamentName(const String& name) {
  _tournamentName = name;
}

void ParcoursField::setFaults(int faults) {
  _faults = faults;
}

void ParcoursField::setTotalTime(float seconds) {
  _totalTime = seconds;
}

void ParcoursField::setObstacleCount(int count) {
  _obstacleCount = min(count, 12);
  for (int i = 0; i < 12; i++) {
    _obstacleStatus[i] = OBSTACLE_PENDING;
  }
}

void ParcoursField::setCurrentObstacle(int obstacle) {
  _currentObstacle = obstacle;
}

void ParcoursField::setObstacleStatus(int obstacle, ObstacleStatus status) {
  if (obstacle >= 0 && obstacle < 12) {
    _obstacleStatus[obstacle] = status;
  }
}

// ========== VOLLBILD-ARENA ==========

void ParcoursField::drawParcours() {
  if (!_tft) return;

  // Vollbild Sandboden
  _tft->fillScreen(SAND_COLOR);

  // Weisse Umrandung
  _tft->drawRect(5, 5, 310, 230, TFT_WHITE);
  _tft->drawRect(6, 6, 308, 228, TFT_WHITE);

  // Hindernisse zeichnen
  drawAllObstacles();
}

void ParcoursField::drawScoreboard() {
  if (!_tft) return;

  // Kleine schwarze Box oben rechts
  int boxX = 220;
  int boxY = 10;
  int boxW = 95;
  int boxH = 45;

  _tft->fillRect(boxX, boxY, boxW, boxH, TFT_BLACK);
  _tft->drawRect(boxX, boxY, boxW, boxH, TFT_WHITE);

  // Fehler
  _tft->setTextSize(1);
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setCursor(boxX + 5, boxY + 5);
  _tft->print("Fehler:");

  _tft->setTextSize(2);
  if (_faults == 0) {
    _tft->setTextColor(TFT_GREEN, TFT_BLACK);
  } else {
    _tft->setTextColor(TFT_RED, TFT_BLACK);
  }
  _tft->setCursor(boxX + 55, boxY + 3);
  _tft->print(_faults);

  // Zeit
  _tft->setTextSize(1);
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setCursor(boxX + 5, boxY + 25);
  _tft->print("Zeit:");

  _tft->setTextSize(2);
  _tft->setCursor(boxX + 35, boxY + 23);
  int secs = (int)_totalTime;
  int tenths = (int)((_totalTime - secs) * 10);
  _tft->printf("%d.%d", secs, tenths);

  // Fortschritt (kleine Punkte)
  int dotY = boxY + 42;
  int dotStartX = boxX + 5;
  for (int i = 0; i < _obstacleCount && i < 10; i++) {
    int dx = dotStartX + i * 9;
    if (_obstacleStatus[i] == OBSTACLE_CLEARED) {
      _tft->fillCircle(dx, dotY, 3, TFT_GREEN);
    } else if (_obstacleStatus[i] == OBSTACLE_KNOCKED) {
      _tft->fillCircle(dx, dotY, 3, TFT_RED);
    } else {
      _tft->drawCircle(dx, dotY, 3, TFT_DARKGREY);
    }
  }
}

void ParcoursField::drawAllObstacles() {
  if (!_tft) return;

  // Hindernisse in 2 Reihen verteilen
  int obstaclesPerRow = (_obstacleCount + 1) / 2;
  int spacingX = 280 / (obstaclesPerRow + 1);

  for (int i = 0; i < _obstacleCount; i++) {
    int row = i / obstaclesPerRow;
    int col = i % obstaclesPerRow;

    int x = 20 + (col + 1) * spacingX;
    int y = (row == 0) ? 100 : 180;

    ObstacleType type = (ObstacleType)(i % 3);  // Vertical, Oxer, Wall
    drawJump(x, y, type, _obstacleStatus[i]);
  }
}

void ParcoursField::drawObstacle(int index, ObstacleType type, ObstacleStatus status) {
  if (!_tft || index < 0 || index >= _obstacleCount) return;

  int obstaclesPerRow = (_obstacleCount + 1) / 2;
  int spacingX = 280 / (obstaclesPerRow + 1);

  int row = index / obstaclesPerRow;
  int col = index % obstaclesPerRow;

  int x = 20 + (col + 1) * spacingX;
  int y = (row == 0) ? 100 : 180;

  // Bereich loeschen
  _tft->fillRect(x - 25, y - 30, 50, 45, SAND_COLOR);

  drawJump(x, y, type, status);
}

void ParcoursField::drawJump(int x, int y, ObstacleType type, ObstacleStatus status) {
  if (!_tft) return;

  bool knocked = (status == OBSTACLE_KNOCKED);
  bool cleared = (status == OBSTACLE_CLEARED);

  // Staender (weiss mit roten Streifen)
  _tft->fillRect(x - 18, y - 20, 4, 25, TFT_WHITE);
  _tft->fillRect(x - 18, y - 20, 4, 6, TFT_RED);
  _tft->fillRect(x + 14, y - 20, 4, 25, TFT_WHITE);
  _tft->fillRect(x + 14, y - 20, 4, 6, TFT_RED);

  if (type == OBSTACLE_VERTICAL) {
    if (knocked) {
      // Stange am Boden
      _tft->fillRect(x - 15, y + 8, 30, 4, FENCE_RED);
    } else {
      // Stange oben
      _tft->fillRect(x - 14, y - 15, 28, 4, FENCE_RED);
      _tft->fillRect(x - 14, y - 12, 28, 2, FENCE_WHITE);
    }
  } else if (type == OBSTACLE_OXER) {
    if (knocked) {
      _tft->fillRect(x - 15, y + 6, 30, 3, FENCE_WHITE);
      _tft->fillRect(x - 12, y + 10, 28, 3, FENCE_RED);
    } else {
      // Zwei Stangen
      _tft->fillRect(x - 14, y - 10, 28, 3, FENCE_WHITE);
      _tft->fillRect(x - 14, y - 16, 28, 3, FENCE_RED);
    }
  } else {
    // Mauer
    if (knocked) {
      _tft->fillRect(x - 10, y + 5, 8, 5, 0xB596);
      _tft->fillRect(x + 2, y + 7, 10, 4, 0xB596);
    } else {
      _tft->fillRect(x - 12, y - 18, 24, 14, 0xC000);
      _tft->drawLine(x - 12, y - 12, x + 11, y - 12, 0x8000);
      _tft->fillRect(x - 13, y - 20, 26, 3, 0x8410);
    }
  }

  // Status-Anzeige
  if (cleared) {
    _tft->setTextSize(1);
    _tft->setTextColor(TFT_GREEN, SAND_COLOR);
    _tft->setCursor(x - 4, y + 10);
    _tft->print("OK");
  }
}

void ParcoursField::drawHorseAndRider(int position, bool jumping) {
  if (!_tft) return;

  int x = 50 + position * 40;
  int y = jumping ? 130 : 145;

  drawHorse(x, y, jumping);
  drawRider(x, y, jumping);
}

void ParcoursField::drawHorse(int x, int y, bool jumping, bool leftFacing) {
  if (!_tft) return;

  int dir = leftFacing ? -1 : 1;

  // Koerper
  _tft->fillEllipse(x, y + 5, 18, 9, HORSE_BROWN);

  // Hals
  _tft->fillTriangle(x + dir * 10, y, x + dir * 20, y - 12, x + dir * 8, y + 8, HORSE_BROWN);

  // Kopf
  _tft->fillEllipse(x + dir * 22, y - 14, 7, 4, HORSE_BROWN);
  _tft->fillCircle(x + dir * 27, y - 13, 1, TFT_BLACK);  // Auge

  // Ohren
  _tft->fillTriangle(x + dir * 18, y - 20, x + dir * 20, y - 15, x + dir * 16, y - 15, HORSE_BROWN);

  // Beine
  if (jumping) {
    _tft->fillRect(x + dir * 5, y + 10, 3, 10, HORSE_BROWN);
    _tft->fillRect(x - dir * 8, y + 8, 3, 12, HORSE_BROWN);
  } else {
    _tft->fillRect(x - 10, y + 12, 3, 14, HORSE_BROWN);
    _tft->fillRect(x - 3, y + 12, 3, 14, HORSE_BROWN);
    _tft->fillRect(x + 4, y + 12, 3, 14, HORSE_BROWN);
    _tft->fillRect(x + 11, y + 12, 3, 14, HORSE_BROWN);
  }

  // Schweif
  _tft->fillTriangle(x - dir * 18, y + 3, x - dir * 26, y + 10, x - dir * 16, y + 10, 0x4100);
}

void ParcoursField::drawRider(int x, int y, bool jumping) {
  if (!_tft) return;

  int rY = jumping ? y - 5 : y;

  // Beine
  _tft->fillRect(x - 5, rY + 2, 3, 10, TFT_BLACK);
  _tft->fillRect(x + 2, rY + 2, 3, 10, TFT_BLACK);

  // Koerper
  _tft->fillRect(x - 4, rY - 8, 8, 12, 0xF800);  // Rote Jacke

  // Kopf
  _tft->fillCircle(x, rY - 12, 4, 0xFE60);

  // Helm
  _tft->fillEllipse(x, rY - 16, 5, 3, TFT_BLACK);
}

// ========== ANIMATIONEN ==========

void ParcoursField::showJumpAnimation(int obstacleIndex) {
  if (!_tft) return;

  // Hindernis als übersprungen markieren
  _obstacleStatus[obstacleIndex] = OBSTACLE_CLEARED;

  // Parcours mit aktualisiertem Status zeichnen
  drawParcours();

  // Großes RICHTIG! in der Mitte
  _tft->setTextColor(TFT_GREEN, SAND_COLOR);
  _tft->setTextSize(4);
  _tft->setCursor(60, 100);
  _tft->print("RICHTIG!");

  delay(1200);
}

void ParcoursField::showKnockdownAnimation(int obstacleIndex) {
  if (!_tft) return;

  // Hindernis als abgeworfen markieren
  _obstacleStatus[obstacleIndex] = OBSTACLE_KNOCKED;

  // Parcours mit aktualisiertem Status zeichnen
  drawParcours();

  // FALSCH! und Fehler in der Mitte
  _tft->setTextColor(TFT_RED, SAND_COLOR);
  _tft->setTextSize(3);
  _tft->setCursor(90, 90);
  _tft->print("FALSCH!");
  _tft->setTextSize(2);
  _tft->setCursor(85, 125);
  _tft->print("+4 Fehler");

  delay(1500);
}

void ParcoursField::showRefusalAnimation(int obstacleIndex) {
  if (!_tft) return;

  // Hindernis als nicht geschafft markieren
  _obstacleStatus[obstacleIndex] = OBSTACLE_KNOCKED;

  // Parcours mit aktualisiertem Status zeichnen
  drawParcours();

  // ZEIT ABGELAUFEN! in der Mitte
  _tft->setTextColor(TFT_ORANGE, SAND_COLOR);
  _tft->setTextSize(2);
  _tft->setCursor(55, 90);
  _tft->print("ZEIT ABGELAUFEN!");
  _tft->setTextSize(2);
  _tft->setCursor(85, 120);
  _tft->print("+4 Fehler");

  delay(1500);
}

void ParcoursField::showRoundComplete(int faults, float totalTime) {
  if (!_tft) return;

  _tft->fillScreen(TFT_BLACK);

  // Titel
  _tft->setTextColor(TFT_YELLOW, TFT_BLACK);
  _tft->setTextSize(3);
  _tft->setCursor(52, 20);
  _tft->print("RITT BEENDET");

  // Ergebnis-Box
  _tft->drawRect(40, 60, 240, 100, TFT_WHITE);

  // Reiter
  _tft->setTextSize(2);
  _tft->setTextColor(TFT_CYAN, TFT_BLACK);
  _tft->setCursor(55, 75);
  _tft->print(_riderName);

  // Fehler
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setCursor(55, 100);
  _tft->print("Fehler: ");
  if (faults == 0) {
    _tft->setTextColor(TFT_GREEN, TFT_BLACK);
    _tft->print("0");
  } else {
    _tft->setTextColor(TFT_RED, TFT_BLACK);
    _tft->print(faults);
  }

  // Zeit
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setCursor(55, 125);
  _tft->print("Zeit:   ");
  int secs = (int)totalTime;
  int tenths = (int)((totalTime - secs) * 10);
  _tft->printf("%d.%d Sek.", secs, tenths);

  // Hinweis
  _tft->setTextColor(TFT_DARKGREY, TFT_BLACK);
  _tft->setTextSize(1);
  _tft->setCursor(85, 220);
  _tft->print("Tippen zum Fortfahren");
}

void ParcoursField::showStechen(int stechenNum) {
  if (!_tft) return;

  _tft->fillScreen(TFT_BLACK);

  _tft->setTextColor(TFT_YELLOW, TFT_BLACK);
  _tft->setTextSize(4);
  _tft->setCursor(50, 50);
  _tft->print("STECHEN!");

  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setTextSize(2);
  _tft->setCursor(110, 110);
  _tft->printf("Runde %d", stechenNum);

  _tft->setTextSize(1);
  _tft->setCursor(50, 150);
  _tft->print("Gleichstand! Schnellster Ritt gewinnt!");

  delay(2000);
}

void ParcoursField::showChampionCelebration() {
  if (!_tft) return;

  // Arena zeichnen
  drawParcours();

  // Siegestext gross in der Mitte
  _tft->setTextColor(TFT_YELLOW, SAND_COLOR);
  _tft->setTextSize(4);
  _tft->setCursor(45, 100);
  _tft->print("GEWONNEN!");

  delay(2500);
}

// ========== NICHT MEHR BENOETIGTE METHODEN ==========

void ParcoursField::drawArenaFence() {}
void ParcoursField::drawFlowerBoxes() {}
void ParcoursField::drawSponsorBoard() {}
void ParcoursField::drawSpectators() {}
void ParcoursField::drawObstacleProgress() {}
void ParcoursField::drawMiniHorse(int x, int y) {}
void ParcoursField::clearObstacleArea(int index) {}
void ParcoursField::showSpectatorReaction(bool positive) {}
void ParcoursField::showJubilatingSpectators(int frame) {}
