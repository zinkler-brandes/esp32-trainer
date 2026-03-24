#include "ClubQuiz.h"
#include "ClubLogoDrawer.h"
#include "BundesligaClubs.h"

// Layout-Konstanten
#define CLUB_NAME_Y    50   // Vereinsname oben

#define BTN_WIDTH   260     // Volle Breite (mit etwas Rand)
#define BTN_HEIGHT  30      // Kleiner fuer mehr Platz
#define BTN_START_Y 80      // Buttons starten unter dem Namen
#define BTN_SPACING 4       // Weniger Abstand
#define BTN_X       10      // Links beginnen

// Fortschrittsanzeige rechts
#define PROGRESS_X  295
#define PROGRESS_START_Y  40
#define PROGRESS_SPACING  13

// Orange fuer "eingeloggt" Animation
#define COLOR_ORANGE 0xFD20

ClubQuiz::ClubQuiz() {
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // 4 Antwort-Buttons untereinander (volle Breite)
  for (int i = 0; i < 4; i++) {
    answerButtons[i] = new Button(BTN_X, BTN_START_Y + i * (BTN_HEIGHT + BTN_SPACING), BTN_WIDTH, BTN_HEIGHT, "");
    answerButtons[i]->setColors(TFT_BLUE, TFT_WHITE);
  }
}

void ClubQuiz::init() {
  tft.setRotation(1);

  _lives = 3;
  _correctCount = 0;
  _wasCorrect = false;
  _gameOver = false;
  _highestMedal = CLUB_MEDAL_NONE;

  // Alle Clubs als unbenutzt markieren
  for (int i = 0; i < 60; i++) {
    _usedClubs[i] = false;
  }

  generateQuestion();
  draw();
}

void ClubQuiz::draw() {
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
  tft.print("VEREINSQUIZ");

  // Leben (Herzen) oben rechts
  drawLives();

  // Club-Logo und Name zeichnen
  drawQuestion();

  // Antwort-Buttons
  for (int i = 0; i < 4; i++) {
    answerButtons[i]->draw(&tft);
  }

  // Fortschrittsanzeige
  drawProgress();
}

void ClubQuiz::drawLives() {
  int startX = 260;
  int y = 12;

  for (int i = 0; i < 3; i++) {
    uint16_t color = (i < _lives) ? TFT_RED : TFT_DARKGREY;
    int x = startX + i * 20;
    // Herz zeichnen
    tft.fillCircle(x - 4, y, 5, color);
    tft.fillCircle(x + 4, y, 5, color);
    tft.fillTriangle(x - 9, y, x + 9, y, x, y + 10, color);
  }
}

void ClubQuiz::drawMedal(int x, int y, uint16_t color) {
  // Kleine Medaille: Kreis mit Band
  tft.fillCircle(x, y, 5, color);
  tft.drawCircle(x, y, 5, TFT_WHITE);
  // Band unten
  tft.fillRect(x - 2, y + 5, 2, 4, TFT_RED);
  tft.fillRect(x + 1, y + 5, 2, 4, TFT_BLUE);
}

void ClubQuiz::drawProgress() {
  int x = PROGRESS_X;

  // 15 Kreise fuer den Fortschritt
  for (int i = 0; i < 15; i++) {
    int y = PROGRESS_START_Y + (14 - i) * PROGRESS_SPACING;
    uint16_t color;

    if (i < _correctCount) {
      color = TFT_GREEN;
    } else if (i == _correctCount) {
      color = TFT_YELLOW;
    } else {
      color = TFT_DARKGREY;
    }

    tft.fillCircle(x, y, 4, color);

    // Meilenstein-Markierungen
    if (i == CLUB_MILESTONE_BRONZE - 1 || i == CLUB_MILESTONE_SILVER - 1 || i == CLUB_MILESTONE_GOLD - 1) {
      tft.drawCircle(x, y, 6, TFT_WHITE);
    }
  }

  // Medaillen-Symbole links neben den Meilensteinen
  int y5 = PROGRESS_START_Y + (14 - (CLUB_MILESTONE_BRONZE - 1)) * PROGRESS_SPACING;
  drawMedal(x - 18, y5, 0xB5A0);  // Bronze

  int y10 = PROGRESS_START_Y + (14 - (CLUB_MILESTONE_SILVER - 1)) * PROGRESS_SPACING;
  drawMedal(x - 18, y10, TFT_LIGHTGREY);  // Silber

  int y15 = PROGRESS_START_Y + (14 - (CLUB_MILESTONE_GOLD - 1)) * PROGRESS_SPACING;
  drawMedal(x - 18, y15, TFT_YELLOW);  // Gold
}

void ClubQuiz::drawQuestion() {
  // Vereinsname gross und zentriert oben
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);

  const char* name = BUNDESLIGA_CLUBS[_currentClub].name;
  int nameLen = strlen(name);
  int nameX = 140 - (nameLen * 6);  // ca. 12 Pixel pro Buchstabe bei Size 2
  if (nameX < 10) nameX = 10;

  tft.setCursor(nameX, CLUB_NAME_Y);
  tft.print(name);
}

void ClubQuiz::generateQuestion() {
  // Zufaelligen Club waehlen, der noch nicht dran war
  int available[BUNDESLIGA_CLUB_COUNT];
  int availableCount = 0;

  for (int i = 0; i < BUNDESLIGA_CLUB_COUNT; i++) {
    if (!_usedClubs[i]) {
      available[availableCount++] = i;
    }
  }

  if (availableCount == 0) {
    // Alle Clubs durch - von vorne beginnen
    for (int i = 0; i < BUNDESLIGA_CLUB_COUNT; i++) {
      _usedClubs[i] = false;
    }
    availableCount = BUNDESLIGA_CLUB_COUNT;
    for (int i = 0; i < BUNDESLIGA_CLUB_COUNT; i++) {
      available[i] = i;
    }
  }

  // Zufaelligen Club auswaehlen
  int randIdx = random(0, availableCount);
  _currentClub = available[randIdx];
  _usedClubs[_currentClub] = true;

  // Das richtige Bundesland
  Bundesland correctBL = BUNDESLIGA_CLUBS[_currentClub].bundesland;

  // Richtige Antwort auf zufaellige Position
  _correctAnswer = random(0, 4);

  // 4 verschiedene Bundeslaender auswaehlen
  bool usedBL[BL_COUNT] = {false};
  usedBL[correctBL] = true;

  for (int i = 0; i < 4; i++) {
    if (i == _correctAnswer) {
      _answers[i] = correctBL;
    } else {
      // Zufaelliges anderes Bundesland
      int wrongBL;
      int attempts = 0;
      do {
        wrongBL = random(0, BL_COUNT);
        attempts++;
      } while (usedBL[wrongBL] && attempts < 50);

      usedBL[wrongBL] = true;
      _answers[i] = wrongBL;
    }
  }

  // Button-Labels setzen
  for (int i = 0; i < 4; i++) {
    answerButtons[i]->setLabel(getBundeslandName((Bundesland)_answers[i]));
    answerButtons[i]->setColors(TFT_BLUE, TFT_WHITE);
  }
}

int ClubQuiz::handleTouch(int16_t x, int16_t y) {
  if (_gameOver) {
    return 0;  // Bei Game Over: zurueck zum Menue
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
        _correctCount++;
        _wasCorrect = true;
        showCorrectFeedback();

        // Medaillen-Check
        if (_correctCount >= CLUB_MILESTONE_GOLD && _highestMedal < CLUB_MEDAL_GOLD) {
          _highestMedal = CLUB_MEDAL_GOLD;
          showMilestoneAnimation(CLUB_MEDAL_GOLD);
        } else if (_correctCount >= CLUB_MILESTONE_SILVER && _highestMedal < CLUB_MEDAL_SILVER) {
          _highestMedal = CLUB_MEDAL_SILVER;
          showMilestoneAnimation(CLUB_MEDAL_SILVER);
        } else if (_correctCount >= CLUB_MILESTONE_BRONZE && _highestMedal < CLUB_MEDAL_BRONZE) {
          _highestMedal = CLUB_MEDAL_BRONZE;
          showMilestoneAnimation(CLUB_MEDAL_BRONZE);
        }
      } else {
        _lives--;
        _wasCorrect = false;
        showWrongFeedback(i);
      }

      // 4. Auto-Weiter nach Pause
      delay(1500);

      if (_lives <= 0 || _correctCount >= CLUB_MILESTONE_GOLD) {
        _gameOver = true;
        showGameOverScreen();
        return 2;
      }

      // Naechste Frage
      generateQuestion();
      draw();
      return 1;
    }
  }

  return -1;
}

void ClubQuiz::showCountdown() {
  // Countdown rechts neben dem Vereinsnamen anzeigen
  int cx = 250;
  int cy = CLUB_NAME_Y + 8;

  for (int n = 3; n >= 1; n--) {
    tft.fillCircle(cx, cy, 15, TFT_BLACK);
    tft.drawCircle(cx, cy, 15, COLOR_ORANGE);

    tft.setTextSize(2);
    tft.setTextColor(COLOR_ORANGE, TFT_BLACK);
    tft.setCursor(cx - 6, cy - 7);
    tft.print(n);

    delay(400);
  }

  // Countdown-Kreis loeschen
  tft.fillCircle(cx, cy, 16, TFT_BLACK);
}

void ClubQuiz::showCorrectFeedback() {
  // Richtigen Button gruen faerben
  answerButtons[_correctAnswer]->setColors(TFT_GREEN, TFT_BLACK);
  answerButtons[_correctAnswer]->draw(&tft);

  // Vereinsname auch gruen
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  const char* name = BUNDESLIGA_CLUBS[_currentClub].name;
  int nameLen = strlen(name);
  int nameX = 140 - (nameLen * 6);
  if (nameX < 10) nameX = 10;
  tft.setCursor(nameX, CLUB_NAME_Y);
  tft.print(name);

  drawProgress();
}

void ClubQuiz::showWrongFeedback(int wrongAnswer) {
  // Falschen Button rot faerben
  answerButtons[wrongAnswer]->setColors(TFT_RED, TFT_WHITE);
  answerButtons[wrongAnswer]->draw(&tft);

  // Richtigen Button gruen (Lerneffekt)
  answerButtons[_correctAnswer]->setColors(TFT_GREEN, TFT_BLACK);
  answerButtons[_correctAnswer]->draw(&tft);

  // Vereinsname rot
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(2);
  const char* name = BUNDESLIGA_CLUBS[_currentClub].name;
  int nameLen = strlen(name);
  int nameX = 140 - (nameLen * 6);
  if (nameX < 10) nameX = 10;
  tft.setCursor(nameX, CLUB_NAME_Y);
  tft.print(name);

  drawLives();
}

void ClubQuiz::drawJubelndeZuschauer(int frame) {
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

void ClubQuiz::showMilestoneAnimation(int medal) {
  String medalText;
  uint16_t medalColor;
  String jubelText;

  switch (medal) {
    case CLUB_MEDAL_BRONZE:
      medalText = "BRONZE!";
      medalColor = 0xB5A0;
      jubelText = "KLASSE!";
      break;
    case CLUB_MEDAL_SILVER:
      medalText = "SILBER!";
      medalColor = TFT_LIGHTGREY;
      jubelText = "SUPER!";
      break;
    case CLUB_MEDAL_GOLD:
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
    if (medal == CLUB_MEDAL_GOLD) {
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
    if (medal == CLUB_MEDAL_GOLD) {
      uint16_t confettiColors[] = {TFT_RED, TFT_BLUE, TFT_GREEN, TFT_YELLOW, TFT_MAGENTA, TFT_CYAN};
      for (int c = 0; c < 10; c++) {
        int cx = random(0, 320);
        int cy = random(35, 170);
        tft.fillRect(cx, cy, random(3, 6), random(3, 6), confettiColors[random(0, 6)]);
      }
    }

    delay(250);
  }

  if (medal < CLUB_MEDAL_GOLD) {
    draw();
  }
}

void ClubQuiz::showConfetti() {
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

void ClubQuiz::showGameOverScreen() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(3);
  if (_correctCount >= CLUB_MILESTONE_GOLD) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(80, 30);
    tft.print("PERFEKT!");
  } else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(50, 30);
    tft.print("GAME OVER");
  }

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(70, 80);
  tft.printf("%d von 15 richtig", _correctCount);

  tft.setTextSize(2);
  tft.setCursor(100, 120);

  if (_highestMedal == CLUB_MEDAL_GOLD) {
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
  } else if (_highestMedal == CLUB_MEDAL_SILVER) {
    tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    tft.print("SILBER");
    // Silber-Medaille gross
    tft.fillCircle(160, 175, 30, TFT_LIGHTGREY);
    tft.drawCircle(160, 175, 30, TFT_WHITE);
    tft.fillRect(155, 205, 10, 15, TFT_RED);
  } else if (_highestMedal == CLUB_MEDAL_BRONZE) {
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

  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(80, 230);
  tft.print("Tippen zum Beenden");
}

int ClubQuiz::getHighestMedal() {
  return _highestMedal;
}

int ClubQuiz::getCorrectCount() {
  return _correctCount;
}
