#include "mathe.h"

// Globaler Spielername (wird von main.cpp gesetzt)
static String _playerName = "SPIELER";

void setPlayerName(const String& name) {
  _playerName = name;
  // Auf max 6 Zeichen kürzen für Anzeige
  if (_playerName.length() > 6) {
    _playerName = _playerName.substring(0, 6);
  }
  _playerName.toUpperCase();
}

String getPlayerName() {
  return _playerName;
}

// Farben für das Spielfeld
#define FIELD_GREEN 0x3665       // Hellgrün für Rasen
#define FIELD_GREEN_DARK 0x2442  // Dunkelgrün für überquerte Felder
#define GOAL_WHITE TFT_WHITE
#define PLAYER_BLUE 0x001F
#define BALL_WHITE TFT_WHITE

Mathe::Mathe() {
  // Numpad erstellen: 3x4 Grid
  // Layout: [7][8][9]
  //         [4][5][6]
  //         [1][2][3]
  //         [<-][0][OK]

  int startX = 20;
  int startY = 60;
  int btnWidth = 90;
  int btnHeight = 38;
  int spacing = 6;

  // Reihe 1: 7, 8, 9
  buttons[0] = new Button(startX, startY, btnWidth, btnHeight, "7");
  buttons[1] = new Button(startX + (btnWidth + spacing), startY, btnWidth, btnHeight, "8");
  buttons[2] = new Button(startX + 2*(btnWidth + spacing), startY, btnWidth, btnHeight, "9");

  // Reihe 2: 4, 5, 6
  buttons[3] = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "4");
  buttons[4] = new Button(startX + (btnWidth + spacing), startY + (btnHeight + spacing), btnWidth, btnHeight, "5");
  buttons[5] = new Button(startX + 2*(btnWidth + spacing), startY + (btnHeight + spacing), btnWidth, btnHeight, "6");

  // Reihe 3: 1, 2, 3
  buttons[6] = new Button(startX, startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "1");
  buttons[7] = new Button(startX + (btnWidth + spacing), startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "2");
  buttons[8] = new Button(startX + 2*(btnWidth + spacing), startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "3");

  // Reihe 4: <-, 0, OK
  buttons[9] = new Button(startX, startY + 3*(btnHeight + spacing), btnWidth, btnHeight, "<-");
  buttons[9]->setColors(TFT_ORANGE, TFT_WHITE);  // Backspace = orange

  buttons[10] = new Button(startX + (btnWidth + spacing), startY + 3*(btnHeight + spacing), btnWidth, btnHeight, "0");

  buttons[11] = new Button(startX + 2*(btnWidth + spacing), startY + 3*(btnHeight + spacing), btnWidth, btnHeight, "OK");
  buttons[11]->setColors(TFT_GREEN, TFT_WHITE);  // OK = grün

  // Start-Button (rechts oben)
  homeButton = new Button(290, 5, 25, 25, "S");
  homeButton->setColors(TFT_ORANGE, TFT_WHITE);
}

void Mathe::init(MatheMode mode, unsigned long durationMs, unsigned long answerTimeMs, int stepsForGoal, int nachspielzeitSek) {
  // Display initialisieren
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);  // Backlight an

  tft.setRotation(1); // Landscape (320x240)
  tft.fillScreen(TFT_BLACK);

  // Nachspielzeit speichern (wird am Ende angezeigt)
  _nachspielzeit = nachspielzeitSek * 1000;
  _nachspielzeitShown = false;

  // Modus setzen
  _mode = mode;

  // Schwierigkeit setzen
  _answerTimeLimit = answerTimeMs;
  _stepsForGoal = stepsForGoal;

  // Spiel initialisieren
  _userInput = "";
  _playerPosition = 0;
  _goalsScored = 0;
  _goalsAgainst = 0;

  // Timer starten
  _gameStartTime = millis();
  _gameDuration = durationMs;
  _gameOver = false;
  _answerStartTime = millis();

  // Penalty-Modus aus
  _penaltyMode = false;
  _penaltyAnswered = false;
  _penaltyCorrect = false;

  // Zufallsgenerator mit LDR initialisieren (echtes Rauschen!)
  randomSeed(analogRead(34));  // Pin 34 = LDR Lichtsensor
  Serial.printf("Random seed: %d\n", analogRead(34));

  // Erste Aufgabe generieren
  generateTask();

  // Buttons zeichnen
  drawButtons();

  // Display aktualisieren
  updateDisplay();
}

void Mathe::initPenalty(MatheMode mode, unsigned long answerTimeMs) {
  // Display initialisieren
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Modus setzen
  _mode = mode;
  _answerTimeLimit = answerTimeMs;
  _stepsForGoal = 1;

  // Spiel initialisieren
  _userInput = "";
  _playerPosition = 0;
  _goalsScored = 0;
  _goalsAgainst = 0;

  // Kein Spielzeit-Timer im Penalty-Modus
  _gameStartTime = millis();
  _gameDuration = 999999999;  // Kein Spielzeit-Limit
  _nachspielzeit = 0;
  _nachspielzeitShown = true;
  _gameOver = false;
  _answerStartTime = millis();

  // Penalty-Modus an
  _penaltyMode = true;
  _penaltyAnswered = false;
  _penaltyCorrect = false;

  // Zufallsgenerator
  randomSeed(analogRead(34));

  // Aufgabe generieren
  generateTask();

  // Buttons zeichnen
  drawButtons();

  // Spezielle Penalty-Anzeige
  updateDisplay();
}

void Mathe::drawButtons() {
  for (int i = 0; i < 12; i++) {
    buttons[i]->draw(&tft);
  }
  homeButton->draw(&tft);
}

bool Mathe::handleButtonPress(int16_t x, int16_t y) {
  // Wenn Spiel vorbei, nur Home-Button reagiert
  if (_gameOver) {
    if (homeButton->contains(x, y)) {
      return true;
    }
    return false;
  }

  // Home-Button prüfen
  if (homeButton->contains(x, y)) {
    Serial.println("HOME pressed - returning to menu");
    return true;  // Zurück zum Menü
  }

  // Numpad-Buttons prüfen
  for (int i = 0; i < 12; i++) {
    if (buttons[i]->contains(x, y)) {
      String label = buttons[i]->getLabel();
      Serial.printf("Button pressed: %s\n", label.c_str());

      if (label == "<-") {
        backspace();
      } else if (label == "OK") {
        checkAnswer();
      } else {
        // Ziffer (0-9)
        addDigit(label);
      }

      break;
    }
  }

  return false;  // Kein Home-Button, weiter im Modus
}

void Mathe::update() {
  // Spielzeit-Timer prüfen (reguläre Zeit + Nachspielzeit)
  unsigned long elapsed = millis() - _gameStartTime;
  unsigned long totalTime = _gameDuration + _nachspielzeit;

  // Reguläre Zeit vorbei? Nachspielzeit-Tafel zeigen
  if (!_nachspielzeitShown && elapsed >= _gameDuration && _nachspielzeit > 0) {
    _nachspielzeitShown = true;
    showNachspielzeitTafel(_nachspielzeit / 1000);
  }

  // Gesamtzeit (inkl. Nachspielzeit) vorbei?
  if (!_gameOver && elapsed >= totalTime) {
    _gameOver = true;
    showGameOver();
    return;
  }

  // Antwort-Timer prüfen (Timeout = Gegentor)
  if (!_gameOver && (millis() - _answerStartTime >= _answerTimeLimit)) {
    handleAnswerTimeout();
    return;
  }

  // Timer-Anzeigen aktualisieren
  if (!_gameOver) {
    updateTimerDisplay();
    updateAnswerTimerDisplay();
  }
}

void Mathe::handleAnswerTimeout() {
  // Zeit abgelaufen = Gegentor
  Serial.println("TIMEOUT - Gegentor!");

  // Animation anzeigen (wie bei falscher Antwort)
  showAnimationScreen(false);

  // Gegentor
  _goalsAgainst++;
  _playerPosition = 0;

  // Zurück zur Rechner-Ansicht
  tft.fillScreen(TFT_BLACK);
  _userInput = "";
  generateTask();
  _answerStartTime = millis();  // Timer neu starten
  drawButtons();
  updateDisplay();
}

void Mathe::updateTimerDisplay() {
  // Nur Spielzeit-Bereich neu zeichnen
  int secs = getRemainingSeconds();

  tft.fillRect(70, 0, 70, 20, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(70, 5);

  if (secs < 0) {
    // Nachspielzeit - rot anzeigen
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.printf("+%d", -secs);
  } else {
    int mins = secs / 60;
    secs = secs % 60;
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.printf("%d:%02d", mins, secs);
  }
}

void Mathe::updateAnswerTimerDisplay() {
  // Antwort-Timer als Fortschrittsbalken anzeigen
  unsigned long elapsed = millis() - _answerStartTime;
  int remaining = _answerTimeLimit - elapsed;
  if (remaining < 0) remaining = 0;

  // Balken-Position (unter der Aufgabe)
  int barX = 10;
  int barY = 48;
  int barMaxWidth = 260;
  int barHeight = 6;

  // Prozent berechnen
  float percent = (float)remaining / (float)_answerTimeLimit;
  int barWidth = (int)(barMaxWidth * percent);

  // Farbe basierend auf verbleibender Zeit
  uint16_t barColor;
  if (percent > 0.5) {
    barColor = TFT_GREEN;
  } else if (percent > 0.25) {
    barColor = TFT_YELLOW;
  } else {
    barColor = TFT_RED;
  }

  // Balken zeichnen
  tft.fillRect(barX, barY, barMaxWidth, barHeight, TFT_DARKGREY);
  if (barWidth > 0) {
    tft.fillRect(barX, barY, barWidth, barHeight, barColor);
  }
}

bool Mathe::isGameOver() {
  return _gameOver;
}

int Mathe::getRemainingSeconds() {
  if (_gameOver) return 0;
  unsigned long elapsed = millis() - _gameStartTime;
  unsigned long totalTime = _gameDuration + _nachspielzeit;
  if (elapsed >= totalTime) return 0;

  // In der Nachspielzeit? Zeige negative Werte (für spezielle Anzeige)
  if (elapsed >= _gameDuration) {
    // Rückgabe als negative Zahl für Nachspielzeit-Anzeige
    return -((elapsed - _gameDuration) / 1000 + 1);
  }
  return (_gameDuration - elapsed) / 1000;
}

void Mathe::generateTask() {
  // Bei MATHE_MIXED zufällig zwischen +/- und * wählen
  MatheMode effectiveMode = _mode;
  if (_mode == MATHE_MIXED) {
    effectiveMode = (random(0, 2) == 0) ? MATHE_PLUS_MINUS : MATHE_MULTIPLY;
  }

  if (effectiveMode == MATHE_PLUS_MINUS) {
    // Zufällig Addition oder Subtraktion
    _operation = random(0, 2) == 0 ? '+' : '-';

    if (_operation == '+') {
      // Addition: Beide Zahlen 1-80, Summe <= 120
      _num1 = random(1, 80);
      _num2 = random(1, min(26, 120 - _num1));  // Damit Summe <= 120
      _result = _num1 + _num2;
    } else {
      // Subtraktion: Erste Zahl 10-100, zweite Zahl 1-10, Ergebnis >= 0
      _num2 = random(1, 11);  // Subtrahend max 10
      _num1 = random(_num2, 101);  // Damit Ergebnis >= 0
      _result = _num1 - _num2;
    }
  } else {
    // Multiplikation: Kleines 1x1 (1-10 * 1-10)
    _operation = '*';
    _num1 = random(1, 11);
    _num2 = random(1, 11);
    _result = _num1 * _num2;
  }

  Serial.printf("New task: %d %c %d = %d\n", _num1, _operation, _num2, _result);
}

void Mathe::updateDisplay() {
  // Oberer Bereich clearen
  tft.fillRect(0, 0, 285, 55, TFT_BLACK);

  // Penalty-Modus: Andere Anzeige
  if (_penaltyMode) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(60, 5);
    tft.print("ELFMETER");

    // Aufgabe anzeigen
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 28);
    tft.printf("%d %c %d = ?", _num1, _operation, _num2);

    // Home-Button neu zeichnen
    homeButton->draw(&tft);

    // Eingabe anzeigen
    tft.fillRect(150, 28, 130, 25, TFT_BLACK);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(150, 28);
    tft.print("= ");
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print(_userInput);
    tft.print("_");
    return;
  }

  // Torstand anzeigen (links oben)
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 5);
  tft.printf("%d:%d", _goalsScored, _goalsAgainst);

  // Timer anzeigen (mittig oben)
  int secs = getRemainingSeconds();
  tft.setCursor(70, 5);
  if (secs < 0) {
    // Nachspielzeit - rot
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.printf("+%d", -secs);
  } else {
    int mins = secs / 60;
    secs = secs % 60;
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.printf("%d:%02d", mins, secs);
  }

  // Fortschritt anzeigen (kleine Punkte für Spielerposition)
  tft.setTextSize(1);  // Kleiner für mehr Platz
  tft.setCursor(140, 8);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  for (int i = 0; i < _playerPosition; i++) {
    tft.print("o");
  }
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  for (int i = _playerPosition; i < _stepsForGoal; i++) {
    tft.print(".");
  }
  tft.setTextSize(2);  // Zurück zu normaler Größe

  // Aufgabe anzeigen
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 28);
  tft.printf("%d %c %d = ?", _num1, _operation, _num2);

  // Home-Button neu zeichnen
  homeButton->draw(&tft);

  // Eingabe anzeigen (unter der Aufgabe)
  tft.fillRect(150, 28, 130, 25, TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(150, 28);
  tft.print("= ");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print(_userInput);
  tft.print("_");
}

void Mathe::addDigit(String digit) {
  // Maximal 3 Ziffern (Ergebnis <= 100)
  if (_userInput.length() < 3) {
    _userInput += digit;
    updateDisplay();
  }
}

void Mathe::backspace() {
  if (_userInput.length() > 0) {
    _userInput = _userInput.substring(0, _userInput.length() - 1);
    updateDisplay();
  }
}

void Mathe::checkAnswer() {
  if (_userInput.length() == 0) {
    return;  // Keine Eingabe
  }

  int answer = _userInput.toInt();
  bool correct = (answer == _result);

  // Penalty-Modus: Nur Ergebnis speichern, keine Animation
  if (_penaltyMode) {
    _penaltyAnswered = true;
    _penaltyCorrect = correct;
    Serial.printf("Penalty: %s\n", correct ? "TREFFER" : "DANEBEN");
    return;
  }

  // Animation anzeigen (wechselt zur Animations-Ansicht)
  showAnimationScreen(correct);

  if (correct) {
    _playerPosition++;
    if (_playerPosition >= _stepsForGoal) {
      // TOR!
      _goalsScored++;
      _playerPosition = 0;
      Serial.printf("GOAL! Score: %d:%d\n", _goalsScored, _goalsAgainst);
    }
  } else {
    // Gegentor
    _goalsAgainst++;
    _playerPosition = 0;
    Serial.printf("Against! Score: %d:%d\n", _goalsScored, _goalsAgainst);
  }

  // Zurück zur Rechner-Ansicht
  tft.fillScreen(TFT_BLACK);
  _userInput = "";
  generateTask();
  _answerStartTime = millis();  // Antwort-Timer neu starten
  drawButtons();
  updateDisplay();
}

void Mathe::showFeedback(bool correct) {
  // Wird nicht mehr direkt verwendet, stattdessen showAnimationScreen
}

// ========== SPIELFELD & ANIMATION ==========

void Mathe::drawField(int crossedFields, int totalFields) {
  // Spielfeld in totalFields Streifen unterteilen
  int fieldWidth = 280;  // Breite vor dem Tor
  int stripeWidth = fieldWidth / totalFields;
  int fieldStartX = 0;
  int fieldY = 60;
  int fieldHeight = 180;

  for (int i = 0; i < totalFields; i++) {
    int stripeX = fieldStartX + (i * stripeWidth);

    // Überquerte Felder dunkelgrün, Rest hellgrün
    uint16_t color = (i < crossedFields) ? FIELD_GREEN_DARK : FIELD_GREEN;
    tft.fillRect(stripeX, fieldY, stripeWidth, fieldHeight, color);

    // Trennlinie zwischen Streifen (subtil)
    if (i > 0) {
      tft.drawLine(stripeX, fieldY, stripeX, fieldY + fieldHeight, 0x2C62);
    }
  }

  // Restlicher Bereich (Tor-Bereich) hellgrün
  tft.fillRect(fieldWidth, fieldY, 40, fieldHeight, FIELD_GREEN);

  // Mittellinie
  int midX = fieldWidth / 2;
  tft.drawLine(midX, fieldY, midX, fieldY + fieldHeight, TFT_WHITE);

  // Mittelkreis
  tft.drawCircle(midX, 150, 25, TFT_WHITE);

  // Strafraum rechts
  tft.drawRect(250, 100, 50, 100, TFT_WHITE);

  // Tor rechts
  drawGoal();
}

void Mathe::drawGoal() {
  // Tor (rechts)
  int goalX = 290;
  int goalY = 110;
  int goalW = 25;
  int goalH = 80;

  // Torpfosten
  tft.fillRect(goalX, goalY, 5, goalH, TFT_WHITE);           // Linker Pfosten
  tft.fillRect(goalX, goalY, goalW, 5, TFT_WHITE);           // Latte oben
  tft.fillRect(goalX, goalY + goalH - 5, goalW, 5, TFT_WHITE); // Latte unten

  // Tornetz (Linien)
  for (int i = 0; i < goalH; i += 10) {
    tft.drawLine(goalX + 5, goalY + i, goalX + goalW, goalY + i, TFT_LIGHTGREY);
  }
}

void Mathe::drawGoalkeeper() {
  // Torwart (vor dem Tor)
  int gkX = 270;
  int gkY = 140;

  // Körper (gelb)
  tft.fillRect(gkX, gkY, 12, 20, TFT_YELLOW);
  // Kopf
  tft.fillCircle(gkX + 6, gkY - 5, 6, 0xFE60);  // Hautfarbe
  // Arme ausgestreckt
  tft.fillRect(gkX - 8, gkY + 5, 28, 4, TFT_YELLOW);
}

void Mathe::drawPlayer(int position, bool withBall, int totalSteps) {
  // Spieler-Position berechnen basierend auf totalSteps
  int fieldWidth = 250;  // Nutzbare Breite
  int stepWidth = fieldWidth / totalSteps;
  int playerX = 20 + (position * stepWidth);
  int playerY = 145;

  // Körper (blau)
  tft.fillRect(playerX, playerY, 12, 20, PLAYER_BLUE);
  // Kopf
  tft.fillCircle(playerX + 6, playerY - 5, 6, 0xFE60);  // Hautfarbe
  // Beine (Animation: leicht versetzt)
  tft.fillRect(playerX + 2, playerY + 20, 3, 8, PLAYER_BLUE);
  tft.fillRect(playerX + 7, playerY + 20, 3, 8, PLAYER_BLUE);

  // Ball
  if (withBall) {
    tft.fillCircle(playerX + 18, playerY + 22, 5, TFT_WHITE);
    tft.drawCircle(playerX + 18, playerY + 22, 5, TFT_BLACK);
  }
}

void Mathe::drawScoreboard() {
  // Anzeigetafel oben
  tft.fillRect(0, 0, 320, 55, TFT_DARKGREY);

  // Torstand groß in der Mitte
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setTextSize(4);
  int scoreX = 110;
  if (_goalsScored >= 10) scoreX -= 20;
  tft.setCursor(scoreX, 10);
  tft.printf("%d : %d", _goalsScored, _goalsAgainst);

  // Spielername links, "CPU" rechts
  tft.setTextSize(2);
  tft.setCursor(10, 18);
  tft.print(_playerName);
  tft.setCursor(265, 18);
  tft.print("CPU");
}

void Mathe::showAnimationScreen(bool correct) {
  // === ANIMATIONS-ANSICHT ===
  tft.fillScreen(TFT_BLACK);

  // Anzeigetafel oben
  drawScoreboard();

  if (correct) {
    // Spieler bewegt sich ein Feld weiter
    int newPos = _playerPosition + 1;

    // Spielfeld mit überquerten Feldern zeichnen
    drawField(newPos, _stepsForGoal);

    // Torwart zeichnen
    drawGoalkeeper();

    if (newPos >= _stepsForGoal) {
      // TOR-ANIMATION!
      // Spieler läuft zum Tor
      for (int p = _playerPosition; p <= _stepsForGoal; p++) {
        drawField(p + 1, _stepsForGoal);
        drawGoal();
        drawGoalkeeper();
        drawPlayer(p, true, _stepsForGoal);
        delay(100);
      }

      // Ball fliegt ins Tor
      tft.fillCircle(295, 150, 6, TFT_WHITE);
      tft.drawCircle(295, 150, 6, TFT_BLACK);

      // TOOOOR! Text
      tft.setTextColor(TFT_GREEN, FIELD_GREEN);
      tft.setTextSize(4);
      tft.setCursor(60, 100);
      tft.print("TOOOOR!");

      delay(2000);
    } else {
      // Normaler Schritt - Spieler auf neuer Position
      drawPlayer(newPos, true, _stepsForGoal);

      // "RICHTIG!" Text
      tft.setTextColor(TFT_GREEN, FIELD_GREEN);
      tft.setTextSize(3);
      tft.setCursor(80, 200);
      tft.print("RICHTIG!");

      delay(1500);
    }
  } else {
    // GEGENTOR Animation - alle Felder zurück auf Start
    drawField(0, _stepsForGoal);
    drawGoalkeeper();
    drawPlayer(0, false, _stepsForGoal);  // Spieler ohne Ball am Start

    // Ball fliegt von rechts nach links ins "eigene" Tor
    for (int bx = 280; bx >= 20; bx -= 20) {
      drawField(0, _stepsForGoal);
      drawGoal();
      drawPlayer(0, false, _stepsForGoal);

      // Fliegender Ball
      tft.fillCircle(bx, 150, 6, TFT_WHITE);
      tft.drawCircle(bx, 150, 6, TFT_BLACK);
      delay(50);
    }

    // GEGENTOR Text
    tft.setTextColor(TFT_RED, FIELD_GREEN);
    tft.setTextSize(3);
    tft.setCursor(60, 90);
    tft.print("GEGENTOR!");

    // Richtige Lösung anzeigen
    tft.setTextColor(TFT_WHITE, FIELD_GREEN);
    tft.setTextSize(2);
    tft.setCursor(60, 130);
    tft.printf("%d %c %d = %d", _num1, _operation, _num2, _result);

    delay(3000);
  }
}

void Mathe::showPenaltyResult(bool playerScored, bool cpuScored, int playerTotal, int cpuTotal, int round) {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 10);
  tft.printf("ELFMETER %d", round);

  // Spieler-Ergebnis
  tft.setTextSize(2);
  tft.setCursor(30, 50);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print(_playerName);
  tft.print(": ");
  if (playerScored) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print("TREFFER!");
  } else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("DANEBEN!");
  }

  delay(1000);

  // CPU-Ergebnis
  tft.setCursor(30, 90);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("CPU: ");

  // Kurze Spannung
  delay(800);

  if (cpuScored) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("TREFFER!");
  } else {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print("DANEBEN!");
  }

  // Aktueller Stand
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(100, 140);
  tft.printf("%d : %d", playerTotal, cpuTotal);

  // Namen
  tft.setTextSize(1);
  tft.setCursor(50, 175);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print(_playerName);
  tft.setCursor(200, 175);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.print("CPU");

  delay(1500);
}

void Mathe::showNachspielzeitTafel(int sekunden) {
  // Bildschirm kurz für Tafel übernehmen
  tft.fillScreen(TFT_BLACK);

  // "NACHSPIELZEIT" oben
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 50);
  tft.print("NACHSPIELZEIT");

  // Schwarze Tafel mit roter Zahl
  int tafelX = 110;
  int tafelY = 90;
  int tafelW = 100;
  int tafelH = 60;

  // Tafel zeichnen (schwarz mit weißem Rand)
  tft.fillRect(tafelX, tafelY, tafelW, tafelH, TFT_BLACK);
  tft.drawRect(tafelX, tafelY, tafelW, tafelH, TFT_WHITE);
  tft.drawRect(tafelX + 1, tafelY + 1, tafelW - 2, tafelH - 2, TFT_WHITE);

  // Rote Zahl mit + Symbol
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(tafelX + 10, tafelY + 14);
  tft.printf("+%d", sekunden);

  delay(2000);  // 2 Sekunden anzeigen

  // Zurück zum Spiel-Bildschirm
  tft.fillScreen(TFT_BLACK);
  drawButtons();

  // Antwort-Timer neu starten (grüner Balken voll)
  _answerStartTime = millis();

  updateDisplay();
}

void Mathe::showGameOver() {
  tft.fillScreen(TFT_BLACK);

  // Großer Titel
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(50, 30);
  tft.print("ABPFIFF!");

  // Endstand
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(90, 90);
  tft.printf("%d : %d", _goalsScored, _goalsAgainst);

  // Wer hat gewonnen?
  tft.setTextSize(2);
  tft.setCursor(60, 150);
  if (_goalsScored > _goalsAgainst) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print(_playerName);
    tft.print(" GEWINNT!");
  } else if (_goalsScored < _goalsAgainst) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("CPU GEWINNT!");
  } else {
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.print("UNENTSCHIEDEN!");
  }

  // Hinweis
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(80, 200);
  tft.print("[S] druecken fuer Menue");

  // Home-Button anzeigen
  homeButton->draw(&tft);
}
