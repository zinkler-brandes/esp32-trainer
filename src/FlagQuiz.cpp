#include "FlagQuiz.h"
#include "FlagDrawer.h"
#include "WM2026Teams.h"
#include "SoccerField.h"

// Konfliktgruppen: Flaggen die NICHT zusammen als Antworten erscheinen duerfen
// -1 markiert Ende der Gruppe (bei weniger als 4 Mitgliedern)
const int FlagQuiz::CONFLICT_GROUPS[CONFLICT_GROUP_COUNT][4] = {
  {23, 30, -1, -1},      // Tuerkei, Tunesien - beide rot mit Halbmond
  {19, 7, 42, -1},       // Oesterreich, Niederlande, Paraguay - Rot-Weiss-X horizontal
  {11, 18, -1, -1},      // Kolumbien, Ecuador - Gelb-Blau-Rot
  {20, 47, -1, -1},      // Norwegen, Daenemark - Nordisches Kreuz
  {3, 21, 32, -1},       // Frankreich, Italien, Elfenbeinkueste - Vertikale Trikolore
  {4, 10, -1, -1}        // Argentinien, Uruguay - Hellblau-Weiss
};

// Layout-Konstanten
#define FLAG_X      80
#define FLAG_Y      42
#define FLAG_WIDTH  120
#define FLAG_HEIGHT 70

#define BTN_WIDTH   125
#define BTN_HEIGHT  40
#define BTN_START_Y 120
#define BTN_SPACING 8
#define BTN_LEFT_X  10
#define BTN_RIGHT_X 145

// Fortschrittsanzeige rechts
#define PROGRESS_X  285
#define PROGRESS_START_Y  40
#define PROGRESS_SPACING  13

// Orange fuer "eingeloggt" Animation
#define COLOR_ORANGE 0xFD20

FlagQuiz::FlagQuiz() {
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // 4 Antwort-Buttons in 2x2 Layout (schmaler fuer Platz rechts)
  answerButtons[0] = new Button(BTN_LEFT_X, BTN_START_Y, BTN_WIDTH, BTN_HEIGHT, "");
  answerButtons[1] = new Button(BTN_RIGHT_X, BTN_START_Y, BTN_WIDTH, BTN_HEIGHT, "");
  answerButtons[2] = new Button(BTN_LEFT_X, BTN_START_Y + BTN_HEIGHT + BTN_SPACING, BTN_WIDTH, BTN_HEIGHT, "");
  answerButtons[3] = new Button(BTN_RIGHT_X, BTN_START_Y + BTN_HEIGHT + BTN_SPACING, BTN_WIDTH, BTN_HEIGHT, "");

  for (int i = 0; i < 4; i++) {
    answerButtons[i]->setColors(TFT_BLUE, TFT_WHITE);
    answerButtons[i]->setTextSize(1);  // Kleinere Schrift fuer lange Laendernamen
  }
}

void FlagQuiz::init() {
  tft.setRotation(1);

  _lives = 3;
  _correctCount = 0;
  _wasCorrect = false;
  _gameOver = false;
  _highestMedal = MEDAL_NONE;

  // Alle Flaggen als unbenutzt markieren
  for (int i = 0; i < 48; i++) {
    _usedFlags[i] = false;
  }

  generateQuestion();
  draw();
}

void FlagQuiz::draw() {
  if (_gameOver) {
    showGameOverScreen();
    return;
  }

  tft.fillScreen(TFT_BLACK);

  // Header
  backButton->draw(&tft);

  // Titel
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 15);
  tft.print("FLAGGENRATEN");

  // Leben (Herzen) oben rechts
  drawLives();

  // Flagge zeichnen
  drawQuestion();

  // Antwort-Buttons
  for (int i = 0; i < 4; i++) {
    answerButtons[i]->draw(&tft);
  }

  // Fortschrittsanzeige unten
  drawProgress();
}

void FlagQuiz::drawLives() {
  int startX = 260;
  int y = 12;

  for (int i = 0; i < 3; i++) {
    uint16_t color = (i < _lives) ? TFT_RED : TFT_DARKGREY;
    int x = startX + i * 20;
    // Herz zeichnen (vereinfacht als zwei ueberlappende Kreise + Dreieck)
    tft.fillCircle(x - 4, y, 5, color);
    tft.fillCircle(x + 4, y, 5, color);
    tft.fillTriangle(x - 9, y, x + 9, y, x, y + 10, color);
  }
}

void FlagQuiz::drawMedal(int x, int y, uint16_t color) {
  // Kleine Medaille: Kreis mit Band
  tft.fillCircle(x, y, 5, color);
  tft.drawCircle(x, y, 5, TFT_WHITE);
  // Band unten
  tft.fillRect(x - 2, y + 5, 2, 4, TFT_RED);
  tft.fillRect(x + 1, y + 5, 2, 4, TFT_BLUE);
}

void FlagQuiz::drawProgress() {
  // Vertikale Fortschrittsanzeige am rechten Rand (von unten nach oben)
  int x = PROGRESS_X;

  // 15 Kreise fuer den Fortschritt (von unten nach oben = 15 oben, 1 unten)
  for (int i = 0; i < 15; i++) {
    int y = PROGRESS_START_Y + (14 - i) * PROGRESS_SPACING;  // 15 oben, 1 unten
    uint16_t color;

    if (i < _correctCount) {
      // Bereits richtig beantwortet
      color = TFT_GREEN;
    } else if (i == _correctCount) {
      // Aktuelle Frage
      color = TFT_YELLOW;
    } else {
      // Noch offen
      color = TFT_DARKGREY;
    }

    tft.fillCircle(x, y, 4, color);

    // Meilenstein-Markierungen mit Rahmen
    if (i == MILESTONE_BRONZE - 1 || i == MILESTONE_SILVER - 1 || i == MILESTONE_GOLD - 1) {
      tft.drawCircle(x, y, 6, TFT_WHITE);
    }
  }

  // Medaillen-Symbole links neben den Meilensteinen
  int y5 = PROGRESS_START_Y + (14 - (MILESTONE_BRONZE - 1)) * PROGRESS_SPACING;
  drawMedal(x - 18, y5, 0xB5A0);  // Bronze

  int y10 = PROGRESS_START_Y + (14 - (MILESTONE_SILVER - 1)) * PROGRESS_SPACING;
  drawMedal(x - 18, y10, TFT_LIGHTGREY);  // Silber

  int y15 = PROGRESS_START_Y + (14 - (MILESTONE_GOLD - 1)) * PROGRESS_SPACING;
  drawMedal(x - 18, y15, TFT_YELLOW);  // Gold
}

void FlagQuiz::drawQuestion() {
  // Flagge gross in der Mitte
  FlagDrawer::drawWMTeamFlag(&tft, FLAG_X, FLAG_Y, FLAG_WIDTH, FLAG_HEIGHT, _currentFlag);
}

void FlagQuiz::generateQuestion() {
  // Zufaellige Flagge waehlen, die noch nicht dran war
  int available[48];
  int availableCount = 0;

  for (int i = 0; i < 48; i++) {
    if (!_usedFlags[i]) {
      available[availableCount++] = i;
    }
  }

  if (availableCount == 0) {
    // Alle Flaggen durch - von vorne beginnen
    for (int i = 0; i < 48; i++) {
      _usedFlags[i] = false;
    }
    availableCount = 48;
    for (int i = 0; i < 48; i++) {
      available[i] = i;
    }
  }

  // Zufaellige Flagge auswaehlen
  int randIdx = random(0, availableCount);
  _currentFlag = available[randIdx];
  _usedFlags[_currentFlag] = true;

  // Konflikgruppe der aktuellen Flagge ermitteln
  int currentConflict = getConflictGroup(_currentFlag);

  // 4 Antworten generieren (1 richtig, 3 falsch)
  _correctAnswer = random(0, 4);

  for (int i = 0; i < 4; i++) {
    if (i == _correctAnswer) {
      _answers[i] = _currentFlag;
    } else {
      // Falsche Antwort waehlen
      bool valid = false;
      int attempts = 0;
      while (!valid && attempts < 100) {
        int wrongFlag = random(0, 48);
        valid = true;

        // Nicht die richtige Antwort
        if (wrongFlag == _currentFlag) valid = false;

        // Nicht bereits in Antworten
        for (int j = 0; j < i; j++) {
          if (_answers[j] == wrongFlag) valid = false;
        }

        // Nicht in Konflikt mit der richtigen Flagge
        if (valid && areFlagsInConflict(_currentFlag, wrongFlag)) {
          valid = false;
        }

        if (valid) {
          _answers[i] = wrongFlag;
        }
        attempts++;
      }

      // Fallback: einfach irgendeine andere Flagge nehmen
      if (!valid) {
        for (int f = 0; f < 48; f++) {
          if (f != _currentFlag) {
            bool used = false;
            for (int j = 0; j < i; j++) {
              if (_answers[j] == f) used = true;
            }
            if (!used) {
              _answers[i] = f;
              break;
            }
          }
        }
      }
    }
  }

  // Button-Labels setzen
  for (int i = 0; i < 4; i++) {
    answerButtons[i]->setLabel(WM_TEAMS[_answers[i]].name);
    answerButtons[i]->setColors(TFT_BLUE, TFT_WHITE);
  }
}

bool FlagQuiz::areFlagsInConflict(int flag1, int flag2) {
  int group1 = getConflictGroup(flag1);
  int group2 = getConflictGroup(flag2);

  // Wenn beide in derselben Konfliktgruppe sind
  return (group1 >= 0 && group1 == group2);
}

int FlagQuiz::getConflictGroup(int flagIndex) {
  for (int g = 0; g < CONFLICT_GROUP_COUNT; g++) {
    for (int i = 0; i < 4; i++) {
      if (CONFLICT_GROUPS[g][i] == -1) break;
      if (CONFLICT_GROUPS[g][i] == flagIndex) {
        return g;
      }
    }
  }
  return -1;  // Keine Konfliktgruppe
}

int FlagQuiz::handleTouch(int16_t x, int16_t y) {
  if (_gameOver) {
    // Bei Game Over: jeder Touch geht zurueck
    return 0;
  }

  // Zurueck-Button
  if (backButton->contains(x, y)) {
    return 0;
  }

  // Antwort-Buttons pruefen
  for (int i = 0; i < 4; i++) {
    if (answerButtons[i]->contains(x, y)) {
      // === "Wer wird Millionaer" Animation ===

      // 1. Orange einloggen
      answerButtons[i]->setColors(COLOR_ORANGE, TFT_BLACK);
      answerButtons[i]->draw(&tft);
      delay(400);

      // 2. Countdown 3, 2, 1
      showCountdown();

      // 3. Aufloesung
      if (i == _correctAnswer) {
        // Richtig!
        _correctCount++;
        _wasCorrect = true;
        showCorrectFeedback();

        // Medaillen-Check
        if (_correctCount >= MILESTONE_GOLD && _highestMedal < MEDAL_GOLD) {
          _highestMedal = MEDAL_GOLD;
          showMilestoneAnimation(MEDAL_GOLD);
        } else if (_correctCount >= MILESTONE_SILVER && _highestMedal < MEDAL_SILVER) {
          _highestMedal = MEDAL_SILVER;
          showMilestoneAnimation(MEDAL_SILVER);
        } else if (_correctCount >= MILESTONE_BRONZE && _highestMedal < MEDAL_BRONZE) {
          _highestMedal = MEDAL_BRONZE;
          showMilestoneAnimation(MEDAL_BRONZE);
        }
      } else {
        // Falsch!
        _lives--;
        _wasCorrect = false;
        showWrongFeedback(i);
      }

      // 4. Auto-Weiter nach 2 Sekunden (oder Game Over)
      delay(1500);

      if (_lives <= 0 || _correctCount >= MILESTONE_GOLD) {
        _gameOver = true;
        showGameOverScreen();
        return 2;  // Game Over
      }

      // Naechste Frage automatisch
      generateQuestion();
      draw();
      return 1;  // Weiter
    }
  }

  return -1;  // Nichts getroffen
}

void FlagQuiz::showCountdown() {
  // Countdown 3, 2, 1 in der Mitte der Flagge anzeigen
  int cx = FLAG_X + FLAG_WIDTH / 2;
  int cy = FLAG_Y + FLAG_HEIGHT / 2;

  for (int n = 3; n >= 1; n--) {
    // Schwarzer Kreis als Hintergrund
    tft.fillCircle(cx, cy, 18, TFT_BLACK);
    tft.drawCircle(cx, cy, 18, COLOR_ORANGE);

    // Zahl
    tft.setTextSize(3);
    tft.setTextColor(COLOR_ORANGE, TFT_BLACK);
    tft.setCursor(cx - 9, cy - 11);
    tft.print(n);

    delay(400);
  }

  // Countdown-Kreis wieder loeschen und Flagge neu zeichnen
  FlagDrawer::drawWMTeamFlag(&tft, FLAG_X, FLAG_Y, FLAG_WIDTH, FLAG_HEIGHT, _currentFlag);
}

void FlagQuiz::showCorrectFeedback() {
  // Gruener Blitz / Rahmen
  tft.drawRect(FLAG_X - 5, FLAG_Y - 5, FLAG_WIDTH + 10, FLAG_HEIGHT + 10, TFT_GREEN);
  tft.drawRect(FLAG_X - 4, FLAG_Y - 4, FLAG_WIDTH + 8, FLAG_HEIGHT + 8, TFT_GREEN);
  tft.drawRect(FLAG_X - 3, FLAG_Y - 3, FLAG_WIDTH + 6, FLAG_HEIGHT + 6, TFT_GREEN);

  // Richtigen Button gruen faerben
  answerButtons[_correctAnswer]->setColors(TFT_GREEN, TFT_WHITE);
  answerButtons[_correctAnswer]->draw(&tft);

  // Fortschritt aktualisieren
  drawProgress();
}

void FlagQuiz::showWrongFeedback(int wrongAnswer) {
  // Roter Blitz / Rahmen
  tft.drawRect(FLAG_X - 5, FLAG_Y - 5, FLAG_WIDTH + 10, FLAG_HEIGHT + 10, TFT_RED);
  tft.drawRect(FLAG_X - 4, FLAG_Y - 4, FLAG_WIDTH + 8, FLAG_HEIGHT + 8, TFT_RED);
  tft.drawRect(FLAG_X - 3, FLAG_Y - 3, FLAG_WIDTH + 6, FLAG_HEIGHT + 6, TFT_RED);

  // Falschen Button rot faerben
  answerButtons[wrongAnswer]->setColors(TFT_RED, TFT_WHITE);
  answerButtons[wrongAnswer]->draw(&tft);

  // Richtigen Button gruen faerben (Lerneffekt)
  answerButtons[_correctAnswer]->setColors(TFT_GREEN, TFT_WHITE);
  answerButtons[_correctAnswer]->draw(&tft);

  // Leben aktualisieren
  drawLives();
}

void FlagQuiz::drawJubelndeZuschauer(int frame) {
  // Zuschauerfarben
  uint16_t colors[] = {TFT_RED, TFT_BLUE, TFT_WHITE, TFT_YELLOW, 0xFBE0, TFT_GREEN, TFT_MAGENTA};

  // Tribuene oben
  tft.fillRect(0, 0, 320, 35, 0x4208);

  // Zuschauer zeichnen (stehend, jubelnd)
  for (int i = 0; i < 20; i++) {
    int x = 10 + i * 15;
    int baseY = 30;
    uint16_t color = colors[i % 7];

    // Kopf
    tft.fillCircle(x, baseY - 12, 4, 0xFED0);

    // Koerper
    tft.fillRect(x - 3, baseY - 8, 6, 8, color);

    // Arme hoch (wechselnd)
    if ((i + frame) % 2 == 0) {
      tft.drawLine(x - 3, baseY - 6, x - 6, baseY - 16, color);
      tft.drawLine(x + 3, baseY - 6, x + 6, baseY - 16, color);
    } else {
      tft.drawLine(x - 3, baseY - 6, x - 7, baseY - 10, color);
      tft.drawLine(x + 3, baseY - 6, x + 7, baseY - 10, color);
    }
  }
}

void FlagQuiz::showMilestoneAnimation(int medal) {
  String medalText;
  uint16_t medalColor;
  String jubelText;

  switch (medal) {
    case MEDAL_BRONZE:
      medalText = "BRONZE!";
      medalColor = 0xB5A0;
      jubelText = "KLASSE!";
      break;
    case MEDAL_SILVER:
      medalText = "SILBER!";
      medalColor = TFT_LIGHTGREY;
      jubelText = "SUPER!";
      break;
    case MEDAL_GOLD:
      medalText = "GOLD!";
      medalColor = TFT_YELLOW;
      jubelText = "WELTKLASSE!";
      break;
    default:
      medalText = "";
      medalColor = TFT_WHITE;
      jubelText = "";
  }

  // Animation mit jubelnden Fans (8 Frames)
  for (int frame = 0; frame < 8; frame++) {
    tft.fillScreen(TFT_BLACK);

    // Jubelnde Zuschauer oben
    drawJubelndeZuschauer(frame);

    // Rasen
    tft.fillRect(0, 35, 320, 130, 0x3665);

    // Grosse Medaille in der Mitte
    int medalY = 100;
    tft.fillCircle(160, medalY, 40, medalColor);
    tft.drawCircle(160, medalY, 40, TFT_WHITE);
    tft.drawCircle(160, medalY, 38, TFT_WHITE);

    // Band unter der Medaille
    tft.fillRect(150, medalY + 40, 8, 20, TFT_RED);
    tft.fillRect(162, medalY + 40, 8, 20, TFT_BLUE);

    // Stern auf Goldmedaille
    if (medal == MEDAL_GOLD) {
      for (int i = 0; i < 5; i++) {
        float angle = (i * 72 - 90) * PI / 180.0;
        int sx = 160 + cos(angle) * 20;
        int sy = medalY + sin(angle) * 20;
        tft.drawLine(160, medalY, sx, sy, TFT_BLACK);
      }
    }

    // Jubeltext (blinkend)
    if (frame % 2 == 0) {
      tft.setTextSize(2);
      tft.setTextColor(TFT_WHITE, 0x3665);
      int jubelX = 160 - (jubelText.length() * 6);
      tft.setCursor(jubelX, 45);
      tft.print(jubelText);
    }

    // Medaillen-Text unten
    tft.setTextSize(3);
    tft.setTextColor(medalColor, TFT_BLACK);
    int textX = 160 - (medalText.length() * 9);
    tft.setCursor(textX, 180);
    tft.print(medalText);

    // Anzahl richtig
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(100, 210);
    tft.printf("%d von 15", _correctCount);

    // Konfetti bei Gold
    if (medal == MEDAL_GOLD) {
      uint16_t confettiColors[] = {TFT_RED, TFT_BLUE, TFT_GREEN, TFT_YELLOW, TFT_MAGENTA, TFT_CYAN};
      for (int c = 0; c < 10; c++) {
        int cx = random(0, 320);
        int cy = random(35, 170);
        tft.fillRect(cx, cy, random(3, 6), random(3, 6), confettiColors[random(0, 6)]);
      }
    }

    delay(250);
  }

  // Zurueck zum Quiz (falls nicht Gold)
  if (medal < MEDAL_GOLD) {
    draw();
  }
}

void FlagQuiz::showConfetti() {
  // Konfetti-Animation: bunte Rechtecke regnen herunter
  uint16_t colors[] = {TFT_RED, TFT_BLUE, TFT_GREEN, TFT_YELLOW, TFT_MAGENTA, TFT_CYAN, 0xFBE0};

  for (int frame = 0; frame < 30; frame++) {
    for (int i = 0; i < 8; i++) {
      int x = random(0, 320);
      int y = random(0, 240);
      int w = random(3, 8);
      int h = random(3, 8);
      uint16_t color = colors[random(0, 7)];
      tft.fillRect(x, y, w, h, color);
    }
    delay(50);
  }
}

void FlagQuiz::showGameOverScreen() {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextSize(3);
  if (_correctCount >= MILESTONE_GOLD) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(80, 30);
    tft.print("PERFEKT!");
  } else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(50, 30);
    tft.print("GAME OVER");
  }

  // Ergebnis
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(70, 80);
  tft.printf("%d von 15 richtig", _correctCount);

  // Medaille anzeigen
  tft.setTextSize(2);
  tft.setCursor(100, 120);

  if (_highestMedal == MEDAL_GOLD) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.print("GOLD");
    // Grosser Pokal
    tft.fillRect(130, 150, 60, 40, TFT_YELLOW);
    tft.fillRect(145, 190, 30, 15, TFT_YELLOW);
    tft.fillRect(135, 205, 50, 10, TFT_YELLOW);

    // Konfetti-Animation bei Gold!
    delay(500);
    showConfetti();

    // Pokal und Text neu zeichnen nach Konfetti
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(80, 30);
    tft.print("PERFEKT!");
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(70, 80);
    tft.printf("%d von 15 richtig", _correctCount);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(100, 120);
    tft.print("GOLD");
    tft.fillRect(130, 150, 60, 40, TFT_YELLOW);
    tft.fillRect(145, 190, 30, 15, TFT_YELLOW);
    tft.fillRect(135, 205, 50, 10, TFT_YELLOW);
  } else if (_highestMedal == MEDAL_SILVER) {
    tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    tft.print("SILBER");
    // Silber-Medaille gross
    tft.fillCircle(160, 175, 30, TFT_LIGHTGREY);
    tft.drawCircle(160, 175, 30, TFT_WHITE);
    tft.fillRect(155, 205, 10, 15, TFT_RED);
  } else if (_highestMedal == MEDAL_BRONZE) {
    tft.setTextColor(0xB5A0, TFT_BLACK);
    tft.print("BRONZE");
    // Bronze-Medaille gross
    tft.fillCircle(160, 175, 30, 0xB5A0);
    tft.drawCircle(160, 175, 30, TFT_WHITE);
    tft.fillRect(155, 205, 10, 15, TFT_RED);
  } else {
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.print("Keine Medaille");
  }

  // Hinweis
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(80, 230);
  tft.print("Tippen zum Beenden");
}

int FlagQuiz::getHighestMedal() {
  return _highestMedal;
}

int FlagQuiz::getCorrectCount() {
  return _correctCount;
}
