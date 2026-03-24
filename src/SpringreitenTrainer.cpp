#include "SpringreitenTrainer.h"

// Schwierigkeits-Daten
static const ReitDifficulty DIFFICULTIES[] = {
  { 6, 15000, 90000, "Bronze" },   // REITABZEICHEN_BRONZE
  { 8, 10000, 75000, "Silber" },   // REITABZEICHEN_SILBER
  { 10, 6000, 60000, "Gold" }      // REITABZEICHEN_GOLD
};

ReitDifficulty SpringreitenTrainer::getDifficulty(Reitabzeichen level) {
  return DIFFICULTIES[(int)level];
}

SpringreitenTrainer::SpringreitenTrainer() {
  // Numpad erstellen (wie in Mathe)
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
  buttons[9]->setColors(TFT_ORANGE, TFT_WHITE);

  buttons[10] = new Button(startX + (btnWidth + spacing), startY + 3*(btnHeight + spacing), btnWidth, btnHeight, "0");

  buttons[11] = new Button(startX + 2*(btnWidth + spacing), startY + 3*(btnHeight + spacing), btnWidth, btnHeight, "OK");
  buttons[11]->setColors(TFT_GREEN, TFT_WHITE);

  // Home-Button
  homeButton = new Button(10, 5, 40, 30, "<");
  homeButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Exit-Button
  exitButton = new Button(295, 2, 22, 18, "X");
  exitButton->setColors(TFT_RED, TFT_WHITE);

  // History initialisieren
  for (int i = 0; i < TASK_HISTORY_SIZE; i++) {
    _taskHistory[i] = -1;
  }
  _taskHistoryIdx = 0;
}

void SpringreitenTrainer::init(MatheMode mode, Reitabzeichen difficulty) {
  // Display initialisieren
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Schwierigkeit laden
  ReitDifficulty diff = getDifficulty(difficulty);

  // ParcoursField initialisieren
  _parcoursField.setDisplay(&tft);
  _parcoursField.setRiderName("REITER");
  _parcoursField.setObstacleCount(diff.obstacleCount);
  _parcoursField.setFaults(0);
  _parcoursField.setTotalTime(0);

  // Modus setzen
  _mode = mode;
  _answerTimeLimit = diff.answerTimeMs;
  _obstacleCount = diff.obstacleCount;
  _parcoursTimeLimit = diff.parcoursTimeMs;

  // Spiel initialisieren
  _userInput = "";
  _faults = 0;
  _currentObstacle = 0;
  _totalTime = 0;

  // Timer starten
  _gameStartTime = millis();
  _gameOver = false;
  _answerStartTime = millis();

  // Stechen aus
  _stechenMode = false;
  _stechenAnswered = false;
  _stechenCorrect = false;
  _stechenTime = 0;

  // Zufallsgenerator
  randomSeed(analogRead(34));

  // Erste Aufgabe
  generateTask();
  drawButtons();
  updateDisplay();
}

void SpringreitenTrainer::initStechen(MatheMode mode, int stechenNum) {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  _parcoursField.setDisplay(&tft);
  _parcoursField.setRiderName("REITER");
  _parcoursField.showStechen(stechenNum);
  delay(2000);

  tft.fillScreen(TFT_BLACK);

  _mode = mode;
  _answerTimeLimit = 10000;  // 10 Sekunden im Stechen
  _obstacleCount = 1;
  _parcoursTimeLimit = 999999;

  _userInput = "";
  _faults = 0;
  _currentObstacle = 0;

  _gameStartTime = millis();
  _gameOver = false;
  _answerStartTime = millis();

  _stechenMode = true;
  _stechenAnswered = false;
  _stechenCorrect = false;
  _stechenTime = 0;
  _stechenNum = stechenNum;

  randomSeed(analogRead(34));
  generateTask();
  drawButtons();
  updateDisplay();
}

void SpringreitenTrainer::drawButtons() {
  for (int i = 0; i < 12; i++) {
    buttons[i]->draw(&tft);
  }
  if (!_gameOver) {
    exitButton->draw(&tft);
  }
}

int SpringreitenTrainer::handleButtonPress(int16_t x, int16_t y) {
  if (_gameOver) {
    if (homeButton->contains(x, y)) {
      return 1;  // Home
    }
    return 0;
  }

  if (exitButton->contains(x, y)) {
    return 2;  // Exit
  }

  for (int i = 0; i < 12; i++) {
    if (buttons[i]->contains(x, y)) {
      String label = buttons[i]->getLabel();

      if (label == "<-") {
        backspace();
      } else if (label == "OK") {
        checkAnswer();
      } else {
        addDigit(label);
      }
      break;
    }
  }

  return 0;
}

void SpringreitenTrainer::update() {
  if (_gameOver) return;

  // Zeit aktualisieren
  _totalTime = (millis() - _gameStartTime) / 1000.0;

  // Antwort-Timeout
  if (_answerTimeLimit > 0 && (millis() - _answerStartTime >= _answerTimeLimit)) {
    handleAnswerTimeout();
    return;
  }

  updateTimerDisplay();
  updateAnswerTimerDisplay();
}

void SpringreitenTrainer::handleAnswerTimeout() {
  if (_stechenMode) {
    _stechenAnswered = true;
    _stechenCorrect = false;
    _stechenTime = (millis() - _gameStartTime) / 1000.0;
    return;
  }

  // Verweigerung = 4 Fehler
  _faults += 4;
  _parcoursField.setFaults(_faults);
  _parcoursField.setObstacleStatus(_currentObstacle, OBSTACLE_KNOCKED);

  // Animation zeigen
  _parcoursField.showRefusalAnimation(_currentObstacle);

  // Naechstes Hindernis
  _currentObstacle++;
  if (_currentObstacle >= _obstacleCount) {
    calculateTimeFaults();
    _gameOver = true;
    showGameOver();
    return;
  }

  // Weiter
  tft.fillScreen(TFT_BLACK);
  _userInput = "";
  generateTask();
  _answerStartTime = millis();
  drawButtons();
  updateDisplay();
}

void SpringreitenTrainer::updateTimerDisplay() {
  tft.fillRect(180, 5, 100, 20, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(180, 5);

  int mins = (int)(_totalTime / 60);
  int secs = (int)_totalTime % 60;
  int tenths = (int)((_totalTime - (int)_totalTime) * 10);
  tft.printf("%d:%02d.%d", mins, secs, tenths);
}

void SpringreitenTrainer::updateAnswerTimerDisplay() {
  if (_answerTimeLimit == 0) return;

  unsigned long elapsed = millis() - _answerStartTime;
  int remaining = _answerTimeLimit - elapsed;
  if (remaining < 0) remaining = 0;

  int barX = 10, barY = 48, barMaxWidth = 260, barHeight = 6;
  float percent = (float)remaining / (float)_answerTimeLimit;
  int barWidth = (int)(barMaxWidth * percent);

  uint16_t barColor;
  if (percent > 0.5) barColor = TFT_GREEN;
  else if (percent > 0.25) barColor = TFT_YELLOW;
  else barColor = TFT_RED;

  tft.fillRect(barX, barY, barMaxWidth, barHeight, TFT_DARKGREY);
  if (barWidth > 0) {
    tft.fillRect(barX, barY, barWidth, barHeight, barColor);
  }
}

bool SpringreitenTrainer::isGameOver() {
  return _gameOver;
}

void SpringreitenTrainer::generateTask() {
  MatheMode effectiveMode = _mode;
  if (_mode == MATHE_ALL) {
    int r = random(0, 3);
    if (r == 0) effectiveMode = MATHE_PLUS_MINUS;
    else if (r == 1) effectiveMode = MATHE_MULTIPLY;
    else effectiveMode = MATHE_DIVIDE;
  }

  if (effectiveMode == MATHE_PLUS_MINUS) {
    _operation = random(0, 2) == 0 ? '+' : '-';

    if (_operation == '+') {
      _num1 = random(1, 80);
      _num2 = random(1, min(26, 120 - _num1));
      _result = _num1 + _num2;
    } else {
      _num2 = random(1, 11);
      _num1 = random(_num2, 101);
      _result = _num1 - _num2;
    }
  } else if (effectiveMode == MATHE_DIVIDE) {
    _operation = '/';
    int a = random(1, 11);
    int b = random(1, 11);
    _num1 = a * b;
    _num2 = b;
    _result = a;
  } else {
    _operation = '*';

    int attempts = 0;
    int taskKey;
    do {
      _num1 = random(1, 11);
      _num2 = random(1, 11);
      taskKey = _num1 * 11 + _num2;
      attempts++;
    } while (attempts < 20 && isTaskInHistory(taskKey));

    _taskHistory[_taskHistoryIdx] = taskKey;
    _taskHistoryIdx = (_taskHistoryIdx + 1) % TASK_HISTORY_SIZE;

    _result = _num1 * _num2;
  }
}

bool SpringreitenTrainer::isTaskInHistory(int taskKey) {
  for (int i = 0; i < TASK_HISTORY_SIZE; i++) {
    if (_taskHistory[i] == taskKey) {
      return true;
    }
  }
  return false;
}

void SpringreitenTrainer::updateDisplay() {
  tft.fillRect(0, 0, 170, 55, TFT_BLACK);

  if (_stechenMode) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(80, 5);
    tft.printf("STECHEN %d", _stechenNum);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 28);
    tft.printf("%d %c %d = ?", _num1, _operation, _num2);

    tft.fillRect(150, 28, 130, 25, TFT_BLACK);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setCursor(150, 28);
    tft.print("= ");
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print(_userInput);
    tft.print("_");
    return;
  }

  // Fehler (links oben)
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(10, 2);
  tft.print("Fehler");
  tft.setTextSize(2);
  if (_faults == 0) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  } else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  }
  tft.setCursor(10, 12);
  tft.print(_faults);

  // Hindernis-Fortschritt
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(55, 2);
  tft.print("Hindernis");
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(55, 12);
  tft.printf("%d/%d", _currentObstacle + 1, _obstacleCount);

  // Zeit
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(120, 2);
  tft.print("Zeit");
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(120, 12);
  int elapsed = (millis() - _gameStartTime) / 1000;
  tft.printf("%d", elapsed);

  // Aufgabe
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 28);
  tft.printf("%d %c %d = ?", _num1, _operation, _num2);

  // Eingabe
  tft.fillRect(150, 28, 130, 25, TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(150, 28);
  tft.print("= ");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print(_userInput);
  tft.print("_");
}

void SpringreitenTrainer::addDigit(String digit) {
  if (_userInput.length() < 3) {
    _userInput += digit;
    updateDisplay();
  }
}

void SpringreitenTrainer::backspace() {
  if (_userInput.length() > 0) {
    _userInput = _userInput.substring(0, _userInput.length() - 1);
    updateDisplay();
  }
}

void SpringreitenTrainer::checkAnswer() {
  if (_userInput.length() == 0) return;

  int answer = _userInput.toInt();
  bool correct = (answer == _result);

  if (_stechenMode) {
    _stechenAnswered = true;
    _stechenCorrect = correct;
    _stechenTime = (millis() - _gameStartTime) / 1000.0;
    return;
  }

  if (correct) {
    // Hindernis uebersprungen
    _parcoursField.setObstacleStatus(_currentObstacle, OBSTACLE_CLEARED);
    _parcoursField.showJumpAnimation(_currentObstacle);
  } else {
    // Abwurf = 4 Fehler
    _faults += 4;
    _parcoursField.setFaults(_faults);
    _parcoursField.setObstacleStatus(_currentObstacle, OBSTACLE_KNOCKED);
    _parcoursField.showKnockdownAnimation(_currentObstacle);
  }

  _currentObstacle++;

  // Parcours beendet?
  if (_currentObstacle >= _obstacleCount) {
    _totalTime = (millis() - _gameStartTime) / 1000.0;
    calculateTimeFaults();
    _gameOver = true;
    showGameOver();
    return;
  }

  // Naechstes Hindernis
  tft.fillScreen(TFT_BLACK);
  _userInput = "";
  generateTask();
  _answerStartTime = millis();
  drawButtons();
  updateDisplay();
}

void SpringreitenTrainer::calculateTimeFaults() {
  // 1 Fehlerpunkt pro 4 Sekunden ueber Normzeit
  float parcoursTimeSec = _parcoursTimeLimit / 1000.0;
  if (_totalTime > parcoursTimeSec) {
    int overTime = (int)(_totalTime - parcoursTimeSec);
    int timeFaults = overTime / 4;  // 1 Fehler pro 4 Sekunden
    _faults += timeFaults;
    _parcoursField.setFaults(_faults);
  }
}

void SpringreitenTrainer::showGameOver() {
  _parcoursField.setTotalTime(_totalTime);
  _parcoursField.showRoundComplete(_faults, _totalTime);
  homeButton->draw(&tft);
}

void SpringreitenTrainer::redrawScreen() {
  tft.fillScreen(TFT_BLACK);
  drawButtons();
  updateDisplay();
}
