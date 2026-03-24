#include "StadionQuiz.h"
#include "Stadien.h"
#include "BundesligaClubs.h"

// Layout-Konstanten
#define STADION_NAME_Y  50   // Stadionname oben

#define BTN_WIDTH   260
#define BTN_HEIGHT  30
#define BTN_START_Y 80
#define BTN_SPACING 4
#define BTN_X       10

// Fortschrittsanzeige rechts
#define PROGRESS_X  295
#define PROGRESS_START_Y  40
#define PROGRESS_SPACING  13

// Orange fuer Animation
#define COLOR_ORANGE 0xFD20

StadionQuiz::StadionQuiz() {
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // 4 Antwort-Buttons untereinander
  for (int i = 0; i < 4; i++) {
    answerButtons[i] = new Button(BTN_X, BTN_START_Y + i * (BTN_HEIGHT + BTN_SPACING), BTN_WIDTH, BTN_HEIGHT, "");
    answerButtons[i]->setColors(TFT_BLUE, TFT_WHITE);
  }
}

void StadionQuiz::init() {
  tft.setRotation(1);

  _lives = 3;
  _correctCount = 0;
  _wasCorrect = false;
  _gameOver = false;
  _highestMedal = STADION_MEDAL_NONE;

  for (int i = 0; i < 60; i++) {
    _usedStadien[i] = false;
  }

  generateQuestion();
  draw();
}

void StadionQuiz::draw() {
  if (_gameOver) {
    showGameOverScreen();
    return;
  }

  tft.fillScreen(TFT_BLACK);

  backButton->draw(&tft);

  // Titel
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 15);
  tft.print("STADION-QUIZ");

  drawLives();
  drawQuestion();

  for (int i = 0; i < 4; i++) {
    answerButtons[i]->draw(&tft);
  }

  drawProgress();
}

void StadionQuiz::drawLives() {
  int startX = 260;
  int y = 12;

  for (int i = 0; i < 3; i++) {
    uint16_t color = (i < _lives) ? TFT_RED : TFT_DARKGREY;
    int x = startX + i * 20;
    tft.fillCircle(x - 4, y, 5, color);
    tft.fillCircle(x + 4, y, 5, color);
    tft.fillTriangle(x - 9, y, x + 9, y, x, y + 10, color);
  }
}

void StadionQuiz::drawMedal(int x, int y, uint16_t color) {
  tft.fillCircle(x, y, 5, color);
  tft.drawCircle(x, y, 5, TFT_WHITE);
  tft.fillRect(x - 2, y + 5, 2, 4, TFT_RED);
  tft.fillRect(x + 1, y + 5, 2, 4, TFT_BLUE);
}

void StadionQuiz::drawProgress() {
  int x = PROGRESS_X;

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

    if (i == STADION_MILESTONE_BRONZE - 1 || i == STADION_MILESTONE_SILVER - 1 || i == STADION_MILESTONE_GOLD - 1) {
      tft.drawCircle(x, y, 6, TFT_WHITE);
    }
  }

  int y5 = PROGRESS_START_Y + (14 - (STADION_MILESTONE_BRONZE - 1)) * PROGRESS_SPACING;
  drawMedal(x - 18, y5, 0xB5A0);

  int y10 = PROGRESS_START_Y + (14 - (STADION_MILESTONE_SILVER - 1)) * PROGRESS_SPACING;
  drawMedal(x - 18, y10, TFT_LIGHTGREY);

  int y15 = PROGRESS_START_Y + (14 - (STADION_MILESTONE_GOLD - 1)) * PROGRESS_SPACING;
  drawMedal(x - 18, y15, TFT_YELLOW);
}

void StadionQuiz::drawQuestion() {
  // Stadionname gross und zentriert
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);

  const char* name = STADIEN[_currentStadion].name;
  int nameLen = strlen(name);
  int nameX = 140 - (nameLen * 6);
  if (nameX < 10) nameX = 10;

  tft.setCursor(nameX, STADION_NAME_Y);
  tft.print(name);
}

void StadionQuiz::generateQuestion() {
  // Zufaelliges Stadion waehlen
  int available[STADION_COUNT];
  int availableCount = 0;

  for (int i = 0; i < STADION_COUNT; i++) {
    if (!_usedStadien[i]) {
      available[availableCount++] = i;
    }
  }

  if (availableCount == 0) {
    for (int i = 0; i < STADION_COUNT; i++) {
      _usedStadien[i] = false;
    }
    availableCount = STADION_COUNT;
    for (int i = 0; i < STADION_COUNT; i++) {
      available[i] = i;
    }
  }

  int randIdx = random(0, availableCount);
  _currentStadion = available[randIdx];
  _usedStadien[_currentStadion] = true;

  // Der richtige Verein
  int correctClub = STADIEN[_currentStadion].clubIndex;

  // Richtige Antwort auf zufaellige Position
  _correctAnswer = random(0, 4);

  // 4 verschiedene Vereine auswaehlen
  bool usedClubs[BUNDESLIGA_CLUB_COUNT] = {false};
  usedClubs[correctClub] = true;

  for (int i = 0; i < 4; i++) {
    if (i == _correctAnswer) {
      _answers[i] = correctClub;
    } else {
      int wrongClub;
      int attempts = 0;
      do {
        wrongClub = random(0, BUNDESLIGA_CLUB_COUNT);
        attempts++;
      } while (usedClubs[wrongClub] && attempts < 50);

      usedClubs[wrongClub] = true;
      _answers[i] = wrongClub;
    }
  }

  // Button-Labels setzen
  for (int i = 0; i < 4; i++) {
    answerButtons[i]->setLabel(BUNDESLIGA_CLUBS[_answers[i]].name);
    answerButtons[i]->setColors(TFT_BLUE, TFT_WHITE);
  }
}

int StadionQuiz::handleTouch(int16_t x, int16_t y) {
  if (_gameOver) {
    return 0;
  }

  if (backButton->contains(x, y)) {
    return 0;
  }

  for (int i = 0; i < 4; i++) {
    if (answerButtons[i]->contains(x, y)) {
      // Orange einloggen
      answerButtons[i]->setColors(COLOR_ORANGE, TFT_BLACK);
      answerButtons[i]->draw(&tft);
      delay(400);

      // Countdown
      showCountdown();

      // Aufloesung
      if (i == _correctAnswer) {
        _correctCount++;
        _wasCorrect = true;
        showCorrectFeedback();

        if (_correctCount >= STADION_MILESTONE_GOLD && _highestMedal < STADION_MEDAL_GOLD) {
          _highestMedal = STADION_MEDAL_GOLD;
          showMilestoneAnimation(STADION_MEDAL_GOLD);
        } else if (_correctCount >= STADION_MILESTONE_SILVER && _highestMedal < STADION_MEDAL_SILVER) {
          _highestMedal = STADION_MEDAL_SILVER;
          showMilestoneAnimation(STADION_MEDAL_SILVER);
        } else if (_correctCount >= STADION_MILESTONE_BRONZE && _highestMedal < STADION_MEDAL_BRONZE) {
          _highestMedal = STADION_MEDAL_BRONZE;
          showMilestoneAnimation(STADION_MEDAL_BRONZE);
        }
      } else {
        _lives--;
        _wasCorrect = false;
        showWrongFeedback(i);
      }

      delay(1500);

      if (_lives <= 0 || _correctCount >= STADION_MILESTONE_GOLD) {
        _gameOver = true;
        showGameOverScreen();
        return 2;
      }

      generateQuestion();
      draw();
      return 1;
    }
  }

  return -1;
}

void StadionQuiz::showCountdown() {
  int cx = 250;
  int cy = STADION_NAME_Y + 8;

  for (int n = 3; n >= 1; n--) {
    tft.fillCircle(cx, cy, 15, TFT_BLACK);
    tft.drawCircle(cx, cy, 15, COLOR_ORANGE);

    tft.setTextSize(2);
    tft.setTextColor(COLOR_ORANGE, TFT_BLACK);
    tft.setCursor(cx - 6, cy - 7);
    tft.print(n);

    delay(400);
  }

  tft.fillCircle(cx, cy, 16, TFT_BLACK);
}

void StadionQuiz::showCorrectFeedback() {
  answerButtons[_correctAnswer]->setColors(TFT_GREEN, TFT_BLACK);
  answerButtons[_correctAnswer]->draw(&tft);

  // Stadionname gruen
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  const char* name = STADIEN[_currentStadion].name;
  int nameLen = strlen(name);
  int nameX = 140 - (nameLen * 6);
  if (nameX < 10) nameX = 10;
  tft.setCursor(nameX, STADION_NAME_Y);
  tft.print(name);

  drawProgress();
}

void StadionQuiz::showWrongFeedback(int wrongAnswer) {
  answerButtons[wrongAnswer]->setColors(TFT_RED, TFT_WHITE);
  answerButtons[wrongAnswer]->draw(&tft);

  answerButtons[_correctAnswer]->setColors(TFT_GREEN, TFT_BLACK);
  answerButtons[_correctAnswer]->draw(&tft);

  // Stadionname rot
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(2);
  const char* name = STADIEN[_currentStadion].name;
  int nameLen = strlen(name);
  int nameX = 140 - (nameLen * 6);
  if (nameX < 10) nameX = 10;
  tft.setCursor(nameX, STADION_NAME_Y);
  tft.print(name);

  drawLives();
}

void StadionQuiz::drawJubelndeZuschauer(int frame) {
  uint16_t colors[] = {TFT_RED, TFT_BLUE, TFT_WHITE, TFT_YELLOW, 0xFBE0, TFT_GREEN, TFT_MAGENTA};

  tft.fillRect(0, 0, 320, 35, 0x4208);

  for (int i = 0; i < 20; i++) {
    int x = 10 + i * 15;
    int baseY = 30;
    uint16_t color = colors[i % 7];

    tft.fillCircle(x, baseY - 12, 4, 0xFED0);
    tft.fillRect(x - 3, baseY - 8, 6, 8, color);

    if ((i + frame) % 2 == 0) {
      tft.drawLine(x - 3, baseY - 6, x - 6, baseY - 16, color);
      tft.drawLine(x + 3, baseY - 6, x + 6, baseY - 16, color);
    } else {
      tft.drawLine(x - 3, baseY - 6, x - 7, baseY - 10, color);
      tft.drawLine(x + 3, baseY - 6, x + 7, baseY - 10, color);
    }
  }
}

void StadionQuiz::showMilestoneAnimation(int medal) {
  String medalText;
  uint16_t medalColor;
  String jubelText;

  switch (medal) {
    case STADION_MEDAL_BRONZE:
      medalText = "BRONZE!";
      medalColor = 0xB5A0;
      jubelText = "KLASSE!";
      break;
    case STADION_MEDAL_SILVER:
      medalText = "SILBER!";
      medalColor = TFT_LIGHTGREY;
      jubelText = "SUPER!";
      break;
    case STADION_MEDAL_GOLD:
      medalText = "GOLD!";
      medalColor = TFT_YELLOW;
      jubelText = "WELTKLASSE!";
      break;
    default:
      medalText = "";
      medalColor = TFT_WHITE;
      jubelText = "";
  }

  for (int frame = 0; frame < 8; frame++) {
    tft.fillScreen(TFT_BLACK);
    drawJubelndeZuschauer(frame);
    tft.fillRect(0, 35, 320, 130, 0x3665);

    int medalY = 100;
    tft.fillCircle(160, medalY, 40, medalColor);
    tft.drawCircle(160, medalY, 40, TFT_WHITE);
    tft.drawCircle(160, medalY, 38, TFT_WHITE);

    tft.fillRect(150, medalY + 40, 8, 20, TFT_RED);
    tft.fillRect(162, medalY + 40, 8, 20, TFT_BLUE);

    if (medal == STADION_MEDAL_GOLD) {
      for (int i = 0; i < 5; i++) {
        float angle = (i * 72 - 90) * PI / 180.0;
        int sx = 160 + cos(angle) * 20;
        int sy = medalY + sin(angle) * 20;
        tft.drawLine(160, medalY, sx, sy, TFT_BLACK);
      }
    }

    if (frame % 2 == 0) {
      tft.setTextSize(2);
      tft.setTextColor(TFT_WHITE, 0x3665);
      int jubelX = 160 - (jubelText.length() * 6);
      tft.setCursor(jubelX, 45);
      tft.print(jubelText);
    }

    tft.setTextSize(3);
    tft.setTextColor(medalColor, TFT_BLACK);
    int textX = 160 - (medalText.length() * 9);
    tft.setCursor(textX, 180);
    tft.print(medalText);

    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(100, 210);
    tft.printf("%d von 15", _correctCount);

    if (medal == STADION_MEDAL_GOLD) {
      uint16_t confettiColors[] = {TFT_RED, TFT_BLUE, TFT_GREEN, TFT_YELLOW, TFT_MAGENTA, TFT_CYAN};
      for (int c = 0; c < 10; c++) {
        int cx = random(0, 320);
        int cy = random(35, 170);
        tft.fillRect(cx, cy, random(3, 6), random(3, 6), confettiColors[random(0, 6)]);
      }
    }

    delay(250);
  }

  if (medal < STADION_MEDAL_GOLD) {
    draw();
  }
}

void StadionQuiz::showConfetti() {
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

void StadionQuiz::showGameOverScreen() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(3);
  if (_correctCount >= STADION_MILESTONE_GOLD) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(80, 30);
    tft.print("PERFEKT!");

    delay(500);
    showConfetti();

    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
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

  if (_highestMedal == STADION_MEDAL_GOLD) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.print("GOLD");
    tft.fillRect(130, 150, 60, 40, TFT_YELLOW);
    tft.fillRect(145, 190, 30, 15, TFT_YELLOW);
    tft.fillRect(135, 205, 50, 10, TFT_YELLOW);
  } else if (_highestMedal == STADION_MEDAL_SILVER) {
    tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    tft.print("SILBER");
    tft.fillCircle(160, 175, 30, TFT_LIGHTGREY);
    tft.drawCircle(160, 175, 30, TFT_WHITE);
  } else if (_highestMedal == STADION_MEDAL_BRONZE) {
    tft.setTextColor(0xB5A0, TFT_BLACK);
    tft.print("BRONZE");
    tft.fillCircle(160, 175, 30, 0xB5A0);
    tft.drawCircle(160, 175, 30, TFT_WHITE);
  } else {
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.print("Keine Medaille");
  }

  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(80, 230);
  tft.print("Tippen zum Beenden");
}

int StadionQuiz::getHighestMedal() {
  return _highestMedal;
}

int StadionQuiz::getCorrectCount() {
  return _correctCount;
}
