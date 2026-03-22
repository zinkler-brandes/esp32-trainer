#include "SoccerField.h"
#include "ClubLogoDrawer.h"
#include "FlagDrawer.h"

SoccerField::SoccerField() : _tft(nullptr), _playerName("SPIELER"),
                             _playerTeamColor(TFT_GREEN),
                             _opponentName("CPU"), _opponentColor(TFT_WHITE),
                             _goalsScored(0), _goalsAgainst(0), _stepsForGoal(5),
                             _showLogos(false), _tournamentType(TOURNAMENT_DFB_POKAL),
                             _playerTeamIndex(-1), _opponentTeamIndex(-1) {
}

void SoccerField::setDisplay(TFT_eSPI* display) {
  _tft = display;
}

void SoccerField::setPlayerName(const String& name) {
  _playerName = name;
  // Für Teams längere Namen erlauben
  if (_playerName.length() > 14) {
    _playerName = _playerName.substring(0, 14);
  }
  _playerName.toUpperCase();
}

void SoccerField::setPlayerTeamColor(uint16_t color) {
  _playerTeamColor = color;
}

void SoccerField::setOpponentName(const String& name) {
  _opponentName = name;
  if (_opponentName.length() > 10) {
    _opponentName = _opponentName.substring(0, 10);
  }
  _opponentName.toUpperCase();
}

void SoccerField::setOpponentColor(uint16_t color) {
  _opponentColor = color;
}

void SoccerField::setScore(int goalsScored, int goalsAgainst) {
  _goalsScored = goalsScored;
  _goalsAgainst = goalsAgainst;
}

void SoccerField::setStepsForGoal(int steps) {
  _stepsForGoal = steps;
}

void SoccerField::setTournamentMode(TournamentType type, int playerTeamIdx, int opponentTeamIdx) {
  _showLogos = true;
  _tournamentType = type;
  _playerTeamIndex = playerTeamIdx;
  _opponentTeamIndex = opponentTeamIdx;
}

void SoccerField::clearTournamentMode() {
  _showLogos = false;
  _playerTeamIndex = -1;
  _opponentTeamIndex = -1;
}

// ========== SPIELFELD ZEICHNEN ==========

void SoccerField::drawField(int unused) {
  if (!_tft) return;

  int fieldY = 60;
  int fieldHeight = 180;

  // Layout: Netz | Grundlinie | Spielfeld | Grundlinie | Netz
  // Netzbereiche: 0-19 (links), 301-319 (rechts)
  // Grundlinien: x=20 (links), x=300 (rechts)
  // Spielfeld: 20-300 (280px)

  int leftGoalLine = 20;
  int rightGoalLine = 300;
  int fieldWidth = rightGoalLine - leftGoalLine;  // 280px

  // Netzbereiche (dunkelgrün, hinter den Toren)
  _tft->fillRect(0, fieldY, leftGoalLine, fieldHeight, 0x1801);  // Sehr dunkel
  _tft->fillRect(rightGoalLine, fieldY, 20, fieldHeight, 0x1801);

  // Zuerst gesamtes Spielfeld mit Grundfarbe füllen
  _tft->fillRect(leftGoalLine, fieldY, fieldWidth, fieldHeight, FIELD_GREEN);

  // Bundesliga-Stil: 10 Streifen mit dezenterem Kontrast
  int numStripes = 10;
  int stripeWidth = fieldWidth / numStripes;  // 280 / 10 = 28px pro Streifen

  // Abwechselnd helle und leicht dunklere Streifen
  for (int i = 1; i < numStripes; i += 2) {
    _tft->fillRect(leftGoalLine + i * stripeWidth, fieldY, stripeWidth, fieldHeight, FIELD_GREEN_ALT);
  }

  // Dezentes Karomuster: sehr feine horizontale Linien alle 12px
  for (int y = fieldY + 6; y < fieldY + fieldHeight; y += 12) {
    for (int x = leftGoalLine; x < rightGoalLine; x += 2) {
      // Nur jeden zweiten Pixel für subtiles Muster
      _tft->drawPixel(x, y, FIELD_GREEN_LINE);
    }
  }

  // Spielfeldrahmen (weiß)
  // Grundlinien (links/rechts)
  _tft->drawLine(leftGoalLine, fieldY, leftGoalLine, fieldY + fieldHeight, TFT_WHITE);
  _tft->drawLine(rightGoalLine, fieldY, rightGoalLine, fieldY + fieldHeight, TFT_WHITE);
  // Seitenlinien (oben/unten)
  _tft->drawLine(leftGoalLine, fieldY, rightGoalLine, fieldY, TFT_WHITE);
  _tft->drawLine(leftGoalLine, fieldY + fieldHeight - 1, rightGoalLine, fieldY + fieldHeight - 1, TFT_WHITE);

  // Mittellinie
  int midX = 160;
  _tft->drawLine(midX, fieldY, midX, fieldY + fieldHeight, TFT_WHITE);

  // Mittelkreis
  _tft->drawCircle(midX, 150, 20, TFT_WHITE);
  _tft->fillCircle(midX, 150, 3, TFT_WHITE);  // Anstoßpunkt

  // Strafraum links (eigenes Tor) - beginnt an Grundlinie
  _tft->drawRect(leftGoalLine, 110, 40, 80, TFT_WHITE);
  _tft->drawRect(leftGoalLine, 130, 18, 40, TFT_WHITE);  // 5-Meter-Raum

  // Elfmeterpunkt links (ca. 27px von Grundlinie)
  int penaltySpotLeftX = leftGoalLine + 27;
  _tft->fillCircle(penaltySpotLeftX, 150, 2, TFT_WHITE);

  // Teilkreis links (nur der Teil außerhalb des Strafraums)
  int arcRadius = 18;
  for (int angle = -50; angle <= 50; angle++) {
    float rad = angle * 3.14159 / 180.0;
    int ax = penaltySpotLeftX + (int)(arcRadius * cos(rad));
    int ay = 150 + (int)(arcRadius * sin(rad));
    if (ax > leftGoalLine + 40) {  // Nur außerhalb des Strafraums
      _tft->drawPixel(ax, ay, TFT_WHITE);
    }
  }

  // Strafraum rechts (Gegner-Tor) - endet an Grundlinie
  _tft->drawRect(rightGoalLine - 40, 110, 40, 80, TFT_WHITE);
  _tft->drawRect(rightGoalLine - 18, 130, 18, 40, TFT_WHITE);  // 5-Meter-Raum

  // Elfmeterpunkt rechts
  int penaltySpotRightX = rightGoalLine - 27;
  _tft->fillCircle(penaltySpotRightX, 150, 2, TFT_WHITE);

  // Teilkreis rechts (nur der Teil außerhalb des Strafraums)
  for (int angle = 130; angle <= 230; angle++) {
    float rad = angle * 3.14159 / 180.0;
    int ax = penaltySpotRightX + (int)(arcRadius * cos(rad));
    int ay = 150 + (int)(arcRadius * sin(rad));
    if (ax < rightGoalLine - 40) {  // Nur außerhalb des Strafraums
      _tft->drawPixel(ax, ay, TFT_WHITE);
    }
  }

  // Tore zeichnen (auf den Grundlinien)
  drawGoalLeft();
  drawGoalRight();

  // Zuschauer (sitzend)
  drawSpectators(false);
}

void SoccerField::drawGoalLeft() {
  if (!_tft) return;

  // Linkes Tor - Öffnung auf Grundlinie (x=20), Latte hinten (x=2)
  int goalLineX = 20;
  int goalBackX = 2;
  int goalY = 120;
  int goalH = 60;

  // Tornetz zuerst (im Torraum)
  for (int i = 0; i < goalH; i += 6) {
    _tft->drawLine(goalBackX, goalY + i, goalLineX, goalY + i, TFT_DARKGREY);
  }
  for (int x = goalBackX; x < goalLineX; x += 6) {
    _tft->drawLine(x, goalY, x, goalY + goalH, TFT_DARKGREY);
  }

  // Torpfosten (weiß) - Seitenlinien von Öffnung zur Latte
  _tft->fillRect(goalBackX, goalY - 2, goalLineX - goalBackX, 4, TFT_WHITE);     // Obere Seitenlinie
  _tft->fillRect(goalBackX, goalY + goalH - 2, goalLineX - goalBackX, 4, TFT_WHITE); // Untere Seitenlinie

  // Latte hinten (Rückseite des Tors)
  _tft->fillRect(goalBackX - 2, goalY - 2, 4, goalH + 4, TFT_WHITE);
}

void SoccerField::drawGoalRight() {
  if (!_tft) return;

  // Rechtes Tor - Öffnung auf Grundlinie (x=300), Latte hinten (x=318)
  int goalLineX = 300;
  int goalBackX = 318;
  int goalY = 120;
  int goalH = 60;

  // Tornetz zuerst (im Torraum)
  for (int i = 0; i < goalH; i += 6) {
    _tft->drawLine(goalLineX, goalY + i, goalBackX, goalY + i, TFT_DARKGREY);
  }
  for (int x = goalLineX; x < goalBackX; x += 6) {
    _tft->drawLine(x, goalY, x, goalY + goalH, TFT_DARKGREY);
  }

  // Torpfosten (weiß) - Seitenlinien von Öffnung zur Latte
  _tft->fillRect(goalLineX, goalY - 2, goalBackX - goalLineX, 4, TFT_WHITE);     // Obere Seitenlinie
  _tft->fillRect(goalLineX, goalY + goalH - 2, goalBackX - goalLineX, 4, TFT_WHITE); // Untere Seitenlinie

  // Latte hinten (Rückseite des Tors)
  _tft->fillRect(goalBackX - 2, goalY - 2, 4, goalH + 4, TFT_WHITE);
}

void SoccerField::drawGoal() {
  // Für Kompatibilität - zeichnet beide Tore
  drawGoalLeft();
  drawGoalRight();
}

void SoccerField::drawGoalkeeper() {
  if (!_tft) return;

  // Torwart vor dem rechten Tor (Grundlinie bei x=300)
  int gkX = 280;
  int gkY = 140;

  // Körper (pinkes Trikot)
  _tft->fillRect(gkX, gkY, 10, 16, TFT_MAGENTA);
  // Kopf
  _tft->fillCircle(gkX + 5, gkY - 4, 5, 0xFE60);
  // Arme ausgestreckt
  _tft->fillRect(gkX - 6, gkY + 4, 22, 3, TFT_MAGENTA);
  // Beine
  _tft->fillRect(gkX + 2, gkY + 16, 2, 6, TFT_BLACK);
  _tft->fillRect(gkX + 6, gkY + 16, 2, 6, TFT_BLACK);
  // Handschuhe
  _tft->fillRect(gkX - 8, gkY + 3, 3, 5, TFT_ORANGE);
  _tft->fillRect(gkX + 15, gkY + 3, 3, 5, TFT_ORANGE);
}

void SoccerField::drawPlayer(int position, bool withBall) {
  if (!_tft) return;

  // Spielfeld von 20 bis 300 (280px), Spieler läuft Richtung Tor
  int fieldStartX = 30;      // Etwas Abstand von der Grundlinie
  int fieldEndX = 260;       // Vor dem Torwart aufhören
  int fieldWidth = fieldEndX - fieldStartX;
  int stepWidth = fieldWidth / _stepsForGoal;
  int playerX = fieldStartX + (position * stepWidth);
  int playerY = 142;

  // Körper (Trikot in Vereinsfarbe)
  _tft->fillRect(playerX, playerY, 10, 16, _playerTeamColor);
  // Kopf
  _tft->fillCircle(playerX + 5, playerY - 4, 5, 0xFE60);

  // Laufanimation: Beine und Arme abwechselnd basierend auf Position
  // Beim Laufen: linkes Bein vorne = rechter Arm vorne (gegenläufig)
  bool leftLegForward = (position % 2 == 0);

  // Arme (gegenläufig zu den Beinen, dick wie beim Torwart)
  if (leftLegForward) {
    // Rechter Arm vorne (Oberarm Trikot, Hand hautfarben)
    _tft->fillRect(playerX + 9, playerY + 3, 8, 3, _playerTeamColor);   // Oberarm
    _tft->fillRect(playerX + 15, playerY + 2, 3, 4, 0xFE60);            // Hand
    // Linker Arm hinten
    _tft->fillRect(playerX - 5, playerY + 3, 6, 3, _playerTeamColor);   // Oberarm
    _tft->fillRect(playerX - 7, playerY + 2, 3, 4, 0xFE60);             // Hand
  } else {
    // Linker Arm vorne
    _tft->fillRect(playerX - 5, playerY + 3, 8, 3, _playerTeamColor);   // Oberarm
    _tft->fillRect(playerX - 7, playerY + 2, 3, 4, 0xFE60);             // Hand
    // Rechter Arm hinten
    _tft->fillRect(playerX + 9, playerY + 3, 6, 3, _playerTeamColor);   // Oberarm
    _tft->fillRect(playerX + 15, playerY + 2, 3, 4, 0xFE60);            // Hand
  }

  // Beine (dicker, wie beim Torwart)
  if (leftLegForward) {
    // Linkes Bein vorne
    _tft->fillRect(playerX + 5, playerY + 16, 3, 7, TFT_BLACK);
    // Rechtes Bein hinten
    _tft->fillRect(playerX + 1, playerY + 16, 3, 7, TFT_BLACK);
  } else {
    // Rechtes Bein vorne
    _tft->fillRect(playerX + 7, playerY + 16, 3, 7, TFT_BLACK);
    // Linkes Bein hinten
    _tft->fillRect(playerX, playerY + 16, 3, 7, TFT_BLACK);
  }

  // Ball am Fuß
  if (withBall) {
    int ballX = leftLegForward ? playerX + 10 : playerX + 14;
    _tft->fillCircle(ballX, playerY + 20, 4, TFT_WHITE);
    _tft->drawCircle(ballX, playerY + 20, 4, TFT_BLACK);
  }
}

void SoccerField::drawScoreboard() {
  if (!_tft) return;

  // Schwarzer Hintergrund wie klassische Anzeigetafel
  _tft->fillRect(0, 0, 320, 55, TFT_BLACK);
  _tft->drawRect(0, 0, 320, 55, TFT_DARKGREY);

  // Logo-Groesse
  const int logoSize = 22;
  const int logoY = 16;

  // Team links mit Logo/Flagge
  if (_showLogos) {
    // Logo oder Flagge zeichnen
    if (_tournamentType == TOURNAMENT_WORLD_CUP_2026 && _playerTeamIndex >= 0) {
      // WM: Landesflagge
      FlagDrawer::drawWMTeamFlag(_tft, 4, logoY, logoSize + 6, logoSize - 2, _playerTeamIndex);
    } else {
      // DFB/CL: Vereinslogo
      ClubLogoDrawer::drawLogo(_tft, 4, logoY, logoSize, _playerName.c_str());
    }
    // Team-Name neben Logo
    _tft->setTextSize(2);
    _tft->setTextColor(_playerTeamColor, TFT_BLACK);
    _tft->setCursor(4 + logoSize + 8, 20);
    _tft->print(_playerName);
  } else {
    // Ohne Logo: wie bisher
    _tft->setTextSize(2);
    _tft->setTextColor(_playerTeamColor, TFT_BLACK);
    _tft->setCursor(8, 20);
    _tft->print(_playerName);
  }

  // Torstand zentriert (Mitte bei x=160)
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setTextSize(4);
  char scoreStr[12];
  sprintf(scoreStr, "%d : %d", _goalsScored, _goalsAgainst);
  int scoreWidth = strlen(scoreStr) * 24;  // Size 4 = 24px pro Zeichen
  int scoreX = 160 - (scoreWidth / 2);
  _tft->setCursor(scoreX, 12);
  _tft->print(scoreStr);

  // Team rechts mit Logo/Flagge
  if (_showLogos) {
    int oppWidth = _opponentName.length() * 12;  // Size 2 = 12px pro Zeichen
    int logoX = 312 - logoSize;  // Logo ganz rechts

    // Logo oder Flagge zeichnen
    if (_tournamentType == TOURNAMENT_WORLD_CUP_2026 && _opponentTeamIndex >= 0) {
      // WM: Landesflagge
      FlagDrawer::drawWMTeamFlag(_tft, logoX - 4, logoY, logoSize + 6, logoSize - 2, _opponentTeamIndex);
      // Team-Name links vom Logo
      _tft->setTextSize(2);
      _tft->setTextColor(_opponentColor, TFT_BLACK);
      _tft->setCursor(logoX - oppWidth - 8, 20);
      _tft->print(_opponentName);
    } else {
      // DFB/CL: Vereinslogo
      ClubLogoDrawer::drawLogo(_tft, logoX, logoY, logoSize, _opponentName.c_str());
      // Team-Name links vom Logo
      _tft->setTextSize(2);
      _tft->setTextColor(_opponentColor, TFT_BLACK);
      _tft->setCursor(logoX - oppWidth - 4, 20);
      _tft->print(_opponentName);
    }
  } else {
    // Ohne Logo: wie bisher
    _tft->setTextSize(2);
    _tft->setTextColor(_opponentColor, TFT_BLACK);
    int oppWidth = _opponentName.length() * 12;
    int oppX = 312 - oppWidth;
    _tft->setCursor(oppX, 20);
    _tft->print(_opponentName);
  }
}

// ========== ZUSCHAUER ==========

void SoccerField::drawSpectators(bool standing) {
  if (!_tft) return;

  // Zuschauerfarben (verschiedene Trikotfarben)
  uint16_t colors[] = {TFT_RED, TFT_BLUE, TFT_YELLOW, TFT_GREEN, TFT_WHITE, TFT_CYAN, TFT_MAGENTA};
  int numColors = 7;

  // Obere Zuschauer-Reihe (oberhalb des Spielfelds) - dichter gepackt
  for (int i = 0; i < 21; i++) {
    int x = 22 + i * 14;
    uint16_t color = colors[i % numColors];
    int yOffset = standing ? -3 : 0;

    // Kopf
    _tft->fillCircle(x, 56 + yOffset, 3, 0xFE60);
    // Körper
    _tft->fillRect(x - 3, 60 + yOffset, 6, 6, color);

    // Arme hoch wenn stehend
    if (standing) {
      _tft->drawLine(x - 3, 61 + yOffset, x - 5, 56 + yOffset, color);
      _tft->drawLine(x + 3, 61 + yOffset, x + 5, 56 + yOffset, color);
    }
  }

  // Untere Zuschauer-Reihe (unterhalb des Spielfelds) - dichter gepackt
  for (int i = 0; i < 21; i++) {
    int x = 22 + i * 14;
    uint16_t color = colors[(i + 3) % numColors];
    int yOffset = standing ? -3 : 0;

    // Kopf
    _tft->fillCircle(x, 236 + yOffset, 3, 0xFE60);
    // Körper
    _tft->fillRect(x - 3, 240 + yOffset, 6, 6, color);

    // Arme hoch wenn stehend
    if (standing) {
      _tft->drawLine(x - 3, 241 + yOffset, x - 5, 236 + yOffset, color);
      _tft->drawLine(x + 3, 241 + yOffset, x + 5, 236 + yOffset, color);
    }
  }

  // Zuschauer neben dem rechten Tor (Gegnertribüne) - mehr Zuschauer
  // Oben neben Tor
  for (int i = 0; i < 4; i++) {
    int x = 305;
    int y = 72 + i * 11;
    uint16_t color = colors[(i + 2) % numColors];
    int yOff = standing ? -2 : 0;

    _tft->fillCircle(x, y + yOff, 2, 0xFE60);
    _tft->fillRect(x - 2, y + 3 + yOff, 4, 5, color);
    if (standing) {
      _tft->drawLine(x - 2, y + 4 + yOff, x - 4, y + yOff, color);
      _tft->drawLine(x + 2, y + 4 + yOff, x + 4, y + yOff, color);
    }
  }

  // Unten neben Tor
  for (int i = 0; i < 4; i++) {
    int x = 305;
    int y = 182 + i * 11;
    uint16_t color = colors[(i + 5) % numColors];
    int yOff = standing ? -2 : 0;

    _tft->fillCircle(x, y + yOff, 2, 0xFE60);
    _tft->fillRect(x - 2, y + 3 + yOff, 4, 5, color);
    if (standing) {
      _tft->drawLine(x - 2, y + 4 + yOff, x - 4, y + yOff, color);
      _tft->drawLine(x + 2, y + 4 + yOff, x + 4, y + yOff, color);
    }
  }

  // Zuschauer neben dem linken Tor (Heimtribüne) - mehr Zuschauer
  // Oben neben Tor
  for (int i = 0; i < 4; i++) {
    int x = 12;
    int y = 72 + i * 11;
    uint16_t color = colors[(i + 1) % numColors];
    int yOff = standing ? -2 : 0;

    _tft->fillCircle(x, y + yOff, 2, 0xFE60);
    _tft->fillRect(x - 2, y + 3 + yOff, 4, 5, color);
    if (standing) {
      _tft->drawLine(x - 2, y + 4 + yOff, x - 4, y + yOff, color);
      _tft->drawLine(x + 2, y + 4 + yOff, x + 4, y + yOff, color);
    }
  }

  // Unten neben Tor
  for (int i = 0; i < 4; i++) {
    int x = 12;
    int y = 182 + i * 11;
    uint16_t color = colors[(i + 4) % numColors];
    int yOff = standing ? -2 : 0;

    _tft->fillCircle(x, y + yOff, 2, 0xFE60);
    _tft->fillRect(x - 2, y + 3 + yOff, 4, 5, color);
    if (standing) {
      _tft->drawLine(x - 2, y + 4 + yOff, x - 4, y + yOff, color);
      _tft->drawLine(x + 2, y + 4 + yOff, x + 4, y + yOff, color);
    }
  }
}

void SoccerField::drawJubilatingSpectators(int frame) {
  if (!_tft) return;

  // Zuschauerfarben
  uint16_t colors[] = {TFT_RED, TFT_BLUE, TFT_YELLOW, TFT_GREEN, TFT_WHITE, TFT_CYAN, TFT_MAGENTA};
  int numColors = 7;

  // Obere Zuschauer-Reihe - jubeln abwechselnd (dichter gepackt)
  int topY = 58;
  for (int i = 0; i < 21; i++) {
    int x = 22 + i * 14;
    uint16_t color = colors[i % numColors];

    // Abwechselnd hoch/normal basierend auf frame und Position
    bool jumping = ((i + frame) % 2 == 0);
    int yOffset = jumping ? -4 : 0;

    // Erst Bereich löschen
    _tft->fillRect(x - 4, topY - 14, 10, 18, TFT_BLACK);

    // Kopf
    _tft->fillCircle(x, topY - 6 + yOffset, 3, 0xFE60);
    // Körper
    _tft->fillRect(x - 3, topY - 2 + yOffset, 6, 8, color);

    // Arme hoch wenn jubelt
    if (jumping) {
      _tft->drawLine(x - 3, topY - 1 + yOffset, x - 5, topY - 6 + yOffset, color);  // Linker Arm
      _tft->drawLine(x + 3, topY - 1 + yOffset, x + 5, topY - 6 + yOffset, color);  // Rechter Arm
    }
  }

  // Untere Zuschauer-Reihe - jubeln abwechselnd (gegenläufig, dichter gepackt)
  int bottomY = 232;
  for (int i = 0; i < 21; i++) {
    int x = 22 + i * 14;
    uint16_t color = colors[(i + 3) % numColors];

    // Gegenläufig zur oberen Reihe
    bool jumping = ((i + frame + 1) % 2 == 0);
    int yOffset = jumping ? -4 : 0;

    // Erst Bereich löschen
    _tft->fillRect(x - 4, bottomY + 2, 10, 20, TFT_BLACK);

    // Kopf
    _tft->fillCircle(x, bottomY + 6 + yOffset, 3, 0xFE60);
    // Körper
    _tft->fillRect(x - 3, bottomY + 10 + yOffset, 6, 8, color);

    // Arme hoch wenn jubelt
    if (jumping) {
      _tft->drawLine(x - 3, bottomY + 11 + yOffset, x - 5, bottomY + 6 + yOffset, color);
      _tft->drawLine(x + 3, bottomY + 11 + yOffset, x + 5, bottomY + 6 + yOffset, color);
    }
  }
}

// ========== ANIMATIONEN ==========

void SoccerField::showGoalAnimation(int fromPosition) {
  if (!_tft) return;

  // Spieler läuft in Schussposition - Zuschauer stehen auf!
  int shootPos = _stepsForGoal - 1;
  for (int p = fromPosition; p <= shootPos; p++) {
    drawField(0);
    drawSpectators(true);  // Zuschauer stehen!
    drawGoalkeeper();
    drawPlayer(p, true);
    delay(80);
  }

  // Schuss-Animation: Ball fliegt ins Tor
  // Spielerposition berechnen (wie in drawPlayer)
  int fieldStartX = 30;
  int fieldEndX = 260;
  int stepWidth = (fieldEndX - fieldStartX) / _stepsForGoal;
  int playerX = fieldStartX + (shootPos * stepWidth);
  int ballStartX = playerX + 14;
  int ballStartY = 162;
  int goalY = 150;

  // Spieler in Schusspose (ohne Ball)
  drawField(0);
  drawSpectators(true);  // Zuschauer stehen!
  drawGoalkeeper();
  drawPlayer(shootPos, false);

  // Ball fliegt zum Tor (Pfosten bei x=300, Netz dahinter)
  for (int bx = ballStartX; bx <= 310; bx += 12) {
    // Ball-Flugbahn leicht nach oben gebogen
    int by = ballStartY - ((bx - ballStartX) / 8);
    if (by < goalY - 20) by = goalY - 20;

    drawField(0);
    drawSpectators(true);  // Zuschauer stehen!
    drawGoalkeeper();
    drawPlayer(shootPos, false);
    _tft->fillCircle(bx, by, 5, TFT_WHITE);
    _tft->drawCircle(bx, by, 5, TFT_BLACK);
    delay(40);
  }

  // Ball im Netz (hinter dem Pfosten bei x=300)
  drawField(0);
  drawSpectators(true);  // Zuschauer stehen!
  drawPlayer(shootPos, false);
  _tft->fillCircle(310, goalY, 5, TFT_WHITE);
  _tft->drawCircle(310, goalY, 5, TFT_BLACK);

  // TOOOOR! Text
  _tft->setTextColor(TFT_GREEN, TFT_BLACK);
  _tft->setTextSize(4);
  _tft->setCursor(80, 90);
  _tft->print("TOOOOR!");

  // Jubel-Animation mit Zuschauern (ca. 3 Sekunden)
  for (int frame = 0; frame < 12; frame++) {
    drawJubilatingSpectators(frame);
    delay(250);
  }
}

void SoccerField::showCounterGoalAnimation(int num1, char operation, int num2, int result) {
  if (!_tft) return;

  // Spielfeld zeichnen
  drawField(0);
  drawGoalkeeper();
  drawPlayer(0, false);

  // Ball fliegt von rechts nach links ins eigene Tor (Pfosten bei x=20)
  for (int bx = 250; bx >= 10; bx -= 15) {
    drawField(0);
    drawGoalkeeper();
    drawPlayer(0, false);
    _tft->fillCircle(bx, 150, 5, TFT_WHITE);
    _tft->drawCircle(bx, 150, 5, TFT_BLACK);
    delay(40);
  }

  // Ball im linken Tor (hinter dem Pfosten bei x=20)
  drawField(0);
  drawPlayer(0, false);
  _tft->fillCircle(10, 150, 5, TFT_WHITE);

  // GEGENTOR Text
  _tft->setTextColor(TFT_RED, TFT_BLACK);
  _tft->setTextSize(3);
  _tft->setCursor(70, 85);
  _tft->print("GEGENTOR!");

  // Richtige Lösung anzeigen
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setTextSize(2);
  _tft->setCursor(80, 125);
  _tft->printf("%d %c %d = %d", num1, operation, num2, result);

  delay(3000);
}

void SoccerField::showCorrectStep(int newPosition) {
  if (!_tft) return;

  drawField(0);
  drawGoalkeeper();
  drawPlayer(newPosition, true);

  // Wenn kurz vor dem Tor: Zuschauer stehen auf!
  if (newPosition >= _stepsForGoal - 1) {
    drawSpectators(true);  // Stehend übermalen
  }

  // RICHTIG! Text
  _tft->setTextColor(TFT_GREEN, FIELD_GREEN);
  _tft->setTextSize(3);
  _tft->setCursor(100, 200);
  _tft->print("RICHTIG!");

  delay(1500);
}

// ========== SPEZIAL-SCREENS ==========

void SoccerField::showNachspielzeitTafel(int sekunden) {
  if (!_tft) return;

  _tft->fillScreen(TFT_BLACK);

  // NACHSPIELZEIT oben
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setTextSize(2);
  _tft->setCursor(60, 50);
  _tft->print("NACHSPIELZEIT");

  // Schwarze Tafel mit roter Zahl
  int tafelX = 110;
  int tafelY = 90;
  int tafelW = 100;
  int tafelH = 60;

  _tft->fillRect(tafelX, tafelY, tafelW, tafelH, TFT_BLACK);
  _tft->drawRect(tafelX, tafelY, tafelW, tafelH, TFT_WHITE);
  _tft->drawRect(tafelX + 1, tafelY + 1, tafelW - 2, tafelH - 2, TFT_WHITE);

  // Rote Zahl
  _tft->setTextColor(TFT_RED, TFT_BLACK);
  _tft->setTextSize(4);
  _tft->setCursor(tafelX + 20, tafelY + 14);
  _tft->printf("+%d", sekunden);

  delay(2000);
}

void SoccerField::showGameOver(int goalsScored, int goalsAgainst) {
  if (!_tft) return;

  _tft->fillScreen(TFT_BLACK);

  // Großer Titel
  _tft->setTextColor(TFT_YELLOW, TFT_BLACK);
  _tft->setTextSize(3);
  _tft->setCursor(70, 20);
  _tft->print("ABPFIFF!");

  // Flaggen/Logos bei WM-Modus
  int flagW = 36;
  int flagH = 24;
  int flagY = 65;
  bool showFlags = (_showLogos && _tournamentType == TOURNAMENT_WORLD_CUP_2026);

  // Layout: [FLAG] [TEAM1]  X:Y  [TEAM2] [FLAG]

  // Endstand berechnen für Zentrierung
  char scoreStr[12];
  sprintf(scoreStr, "%d:%d", goalsScored, goalsAgainst);
  int scoreWidth = strlen(scoreStr) * 24;  // Size 4
  int scoreX = 160 - (scoreWidth / 2);

  // Endstand in der Mitte
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setTextSize(4);
  _tft->setCursor(scoreX, 70);
  _tft->print(scoreStr);

  // Team links mit Flagge
  int playerTextX = 10;
  if (showFlags && _playerTeamIndex >= 0) {
    FlagDrawer::drawWMTeamFlag(_tft, 5, flagY, flagW, flagH, _playerTeamIndex);
    playerTextX = 5 + flagW + 5;
  }
  _tft->setTextSize(2);
  _tft->setTextColor(_playerTeamColor, TFT_BLACK);
  _tft->setCursor(playerTextX, 75);
  _tft->print(_playerName);

  // Gegner rechts mit Flagge
  _tft->setTextSize(2);
  _tft->setTextColor(_opponentColor, TFT_BLACK);
  int oppWidth = _opponentName.length() * 12;  // Size 2 = 12px pro Zeichen
  int oppTextX;
  if (showFlags && _opponentTeamIndex >= 0) {
    int flagX = 310 - flagW;
    FlagDrawer::drawWMTeamFlag(_tft, flagX, flagY, flagW, flagH, _opponentTeamIndex);
    oppTextX = flagX - oppWidth - 5;
  } else {
    oppTextX = 310 - oppWidth;
  }
  _tft->setCursor(oppTextX, 75);
  _tft->print(_opponentName);

  // Wer hat gewonnen?
  _tft->setTextSize(2);
  _tft->setCursor(60, 130);
  if (goalsScored > goalsAgainst) {
    _tft->setTextColor(_playerTeamColor, TFT_BLACK);
    _tft->print(_playerName);
    _tft->setTextColor(TFT_GREEN, TFT_BLACK);
    _tft->print(" GEWINNT!");
  } else if (goalsScored < goalsAgainst) {
    _tft->setTextColor(_opponentColor, TFT_BLACK);
    _tft->print(_opponentName);
    _tft->setTextColor(TFT_RED, TFT_BLACK);
    _tft->print(" GEWINNT!");
  } else {
    _tft->setTextColor(TFT_CYAN, TFT_BLACK);
    _tft->print("UNENTSCHIEDEN!");
  }

  // Hinweis für Zurück-Button
  _tft->setTextColor(TFT_DARKGREY, TFT_BLACK);
  _tft->setTextSize(1);
  _tft->setCursor(90, 210);
  _tft->print("< druecken fuer Menue");
}

void SoccerField::showPenaltyResult(bool playerScored, bool cpuScored,
                                     int playerTotal, int cpuTotal, int round) {
  if (!_tft) return;

  _tft->fillScreen(TFT_BLACK);

  // Titel
  _tft->setTextColor(TFT_YELLOW, TFT_BLACK);
  _tft->setTextSize(2);
  _tft->setCursor(80, 10);
  _tft->printf("ELFMETER %d", round);

  // Spieler-Ergebnis (Teamname)
  _tft->setTextSize(2);
  _tft->setCursor(20, 50);
  _tft->setTextColor(_playerTeamColor, TFT_BLACK);
  _tft->print(_playerName);
  _tft->print(": ");
  if (playerScored) {
    _tft->setTextColor(TFT_GREEN, TFT_BLACK);
    _tft->print("TOR!");
  } else {
    _tft->setTextColor(TFT_RED, TFT_BLACK);
    _tft->print("DANEBEN!");
  }

  delay(1000);

  // Gegner-Ergebnis
  _tft->setCursor(20, 90);
  _tft->setTextColor(_opponentColor, TFT_BLACK);
  _tft->print(_opponentName);
  _tft->print(": ");

  delay(800);

  if (cpuScored) {
    _tft->setTextColor(TFT_RED, TFT_BLACK);
    _tft->print("TREFFER!");
  } else {
    _tft->setTextColor(TFT_GREEN, TFT_BLACK);
    _tft->print("DANEBEN!");
  }

  // Aktueller Stand
  _tft->setTextSize(3);
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setCursor(110, 140);
  _tft->printf("%d : %d", playerTotal, cpuTotal);

  // Team-Kürzel unter dem Stand
  _tft->setTextSize(2);
  _tft->setCursor(50, 180);
  _tft->setTextColor(_playerTeamColor, TFT_BLACK);
  _tft->print(_playerName);
  _tft->setCursor(220, 180);
  _tft->setTextColor(_opponentColor, TFT_BLACK);
  _tft->print(_opponentName);

  delay(1500);
}
