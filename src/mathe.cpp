#include "mathe.h"

// Globaler Spielername und Teamfarbe (wird von main.cpp gesetzt)
static String _playerName = "SPIELER";
static uint16_t _playerTeamColor = TFT_GREEN;
static String _opponentName = "CPU";
static uint16_t _opponentColor = TFT_WHITE;

void setPlayerName(const String& name) {
  _playerName = name;
  if (_playerName.length() > 14) {
    _playerName = _playerName.substring(0, 14);
  }
  _playerName.toUpperCase();
}

void setPlayerTeamColor(uint16_t color) {
  _playerTeamColor = color;
}

void setOpponentName(const String& name) {
  _opponentName = name;
  if (_opponentName.length() > 10) {
    _opponentName = _opponentName.substring(0, 10);
  }
  _opponentName.toUpperCase();
}

void setOpponentColor(uint16_t color) {
  _opponentColor = color;
}

String getPlayerName() {
  return _playerName;
}

uint16_t getPlayerTeamColor() {
  return _playerTeamColor;
}

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
  buttons[9]->setColors(TFT_ORANGE, TFT_WHITE);

  buttons[10] = new Button(startX + (btnWidth + spacing), startY + 3*(btnHeight + spacing), btnWidth, btnHeight, "0");

  buttons[11] = new Button(startX + 2*(btnWidth + spacing), startY + 3*(btnHeight + spacing), btnWidth, btnHeight, "OK");
  buttons[11]->setColors(TFT_GREEN, TFT_WHITE);

  // Home-Button (links oben, konsistent mit anderen Screens)
  homeButton = new Button(10, 5, 40, 30, "<");
  homeButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Exit-Button (rechts oben, rotes X zum Abbrechen)
  exitButton = new Button(295, 2, 22, 18, "X");
  exitButton->setColors(TFT_RED, TFT_WHITE);

  // Aufgaben-History initialisieren
  for (int i = 0; i < TASK_HISTORY_SIZE; i++) {
    _taskHistory[i] = -1;
  }
  _taskHistoryIdx = 0;
}

void Mathe::init(MatheMode mode, unsigned long durationMs, unsigned long answerTimeMs, int stepsForGoal, int nachspielzeitSek) {
  // Display initialisieren
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // SoccerField initialisieren
  _soccerField.setDisplay(&tft);
  _soccerField.setPlayerName(_playerName);
  _soccerField.setPlayerTeamColor(_playerTeamColor);
  _soccerField.setOpponentName(_opponentName);
  _soccerField.setOpponentColor(_opponentColor);
  _soccerField.setStepsForGoal(stepsForGoal);

  // Nachspielzeit speichern
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

  // Zufallsgenerator
  randomSeed(analogRead(34));

  // Erste Aufgabe generieren
  generateTask();
  drawButtons();
  updateDisplay();
}

void Mathe::initPenalty(MatheMode mode, unsigned long answerTimeMs) {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // SoccerField initialisieren
  _soccerField.setDisplay(&tft);
  _soccerField.setPlayerName(_playerName);
  _soccerField.setPlayerTeamColor(_playerTeamColor);
  _soccerField.setOpponentName(_opponentName);
  _soccerField.setOpponentColor(_opponentColor);
  _soccerField.setStepsForGoal(1);

  _mode = mode;
  _answerTimeLimit = answerTimeMs;
  _stepsForGoal = 1;

  _userInput = "";
  _playerPosition = 0;
  _goalsScored = 0;
  _goalsAgainst = 0;

  _gameStartTime = millis();
  _gameDuration = 999999999;
  _nachspielzeit = 0;
  _nachspielzeitShown = true;
  _gameOver = false;
  _answerStartTime = millis();

  _penaltyMode = true;
  _penaltyAnswered = false;
  _penaltyCorrect = false;

  randomSeed(analogRead(34));
  generateTask();
  drawButtons();
  updateDisplay();
}

void Mathe::drawButtons() {
  for (int i = 0; i < 12; i++) {
    buttons[i]->draw(&tft);
  }
  // Exit-Button wird während des Spiels gezeichnet (nicht bei Game Over)
  if (!_gameOver) {
    exitButton->draw(&tft);
  }
  // Home-Button wird nur bei Spielende gezeichnet
}

int Mathe::handleButtonPress(int16_t x, int16_t y) {
  // Bei Spielende: Home-Button prüfen
  if (_gameOver) {
    if (homeButton->contains(x, y)) {
      return 1;  // Home
    }
    return 0;  // Nichts
  }

  // Exit-Button prüfen (während des Spiels)
  if (exitButton->contains(x, y)) {
    return 2;  // Exit angefragt
  }

  // Numpad-Buttons
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

  return 0;  // Nichts
}

void Mathe::update() {
  unsigned long elapsed = millis() - _gameStartTime;
  unsigned long totalTime = _gameDuration + _nachspielzeit;

  // Nachspielzeit-Tafel zeigen
  if (!_nachspielzeitShown && elapsed >= _gameDuration && _nachspielzeit > 0) {
    _nachspielzeitShown = true;
    _soccerField.showNachspielzeitTafel(_nachspielzeit / 1000);
    tft.fillScreen(TFT_BLACK);
    drawButtons();
    _answerStartTime = millis();
    updateDisplay();
  }

  // Spielende
  if (!_gameOver && elapsed >= totalTime) {
    _gameOver = true;
    showGameOver();
    return;
  }

  // Antwort-Timeout (nicht im Lernmodus)
  if (!_gameOver && _answerTimeLimit > 0 && (millis() - _answerStartTime >= _answerTimeLimit)) {
    handleAnswerTimeout();
    return;
  }

  if (!_gameOver) {
    // Spiel-Timer nur im normalen Modus, nicht im Penalty-Modus
    if (!_penaltyMode) {
      updateTimerDisplay();
    }
    updateAnswerTimerDisplay();
  }
}

void Mathe::handleAnswerTimeout() {
  // Im Penalty-Modus: Timeout = verpasster Elfmeter
  if (_penaltyMode) {
    _penaltyAnswered = true;
    _penaltyCorrect = false;
    return;
  }

  showAnimationScreen(false);
  _goalsAgainst++;
  _playerPosition = 0;

  tft.fillScreen(TFT_BLACK);
  _userInput = "";
  generateTask();
  _answerStartTime = millis();
  drawButtons();
  updateDisplay();
}

void Mathe::updateTimerDisplay() {
  int secs = getRemainingSeconds();

  tft.fillRect(70, 0, 70, 20, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(70, 5);

  if (secs < 0) {
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

bool Mathe::isGameOver() {
  return _gameOver;
}

int Mathe::getRemainingSeconds() {
  if (_gameOver) return 0;
  unsigned long elapsed = millis() - _gameStartTime;
  unsigned long totalTime = _gameDuration + _nachspielzeit;
  if (elapsed >= totalTime) return 0;

  if (elapsed >= _gameDuration) {
    return -((elapsed - _gameDuration) / 1000 + 1);
  }
  return (_gameDuration - elapsed) / 1000;
}

void Mathe::generateTask() {
  MatheMode effectiveMode = _mode;
  if (_mode == MATHE_ALL) {
    // Zufaellig zwischen +/-, 1x1 und 1:1 waehlen
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

    // Multiplikation mit History gegen Wiederholungen
    int attempts = 0;
    int taskKey;
    do {
      _num1 = random(1, 11);
      _num2 = random(1, 11);
      taskKey = _num1 * 11 + _num2;  // Eindeutiger Key fuer diese Kombination
      attempts++;
    } while (attempts < 20 && isTaskInHistory(taskKey));

    // In History speichern
    _taskHistory[_taskHistoryIdx] = taskKey;
    _taskHistoryIdx = (_taskHistoryIdx + 1) % TASK_HISTORY_SIZE;

    _result = _num1 * _num2;
  }
}

bool Mathe::isTaskInHistory(int taskKey) {
  for (int i = 0; i < TASK_HISTORY_SIZE; i++) {
    if (_taskHistory[i] == taskKey) {
      return true;
    }
  }
  return false;
}

void Mathe::updateDisplay() {
  tft.fillRect(0, 0, 250, 55, TFT_BLACK);

  if (_penaltyMode) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(100, 5);
    tft.print("ELFMETER");

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 28);
    tft.printf("%d %c %d = ?", _num1, _operation, _num2);

    tft.fillRect(150, 28, 130, 25, TFT_BLACK);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(150, 28);
    tft.print("= ");
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print(_userInput);
    tft.print("_");

    // Timer-Balken auch im Penalty-Modus anzeigen
    updateAnswerTimerDisplay();
    return;
  }

  // Torstand (links oben)
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 5);
  tft.printf("%d:%d", _goalsScored, _goalsAgainst);

  // Timer
  int secs = getRemainingSeconds();
  tft.setCursor(70, 5);
  if (secs < 0) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.printf("+%d", -secs);
  } else {
    int mins = secs / 60;
    secs = secs % 60;
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.printf("%d:%02d", mins, secs);
  }

  // Fortschritt
  tft.setTextSize(1);
  tft.setCursor(140, 8);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  for (int i = 0; i < _playerPosition; i++) tft.print("o");
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  for (int i = _playerPosition; i < _stepsForGoal; i++) tft.print(".");
  tft.setTextSize(2);

  // Aufgabe
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 28);
  tft.printf("%d %c %d = ?", _num1, _operation, _num2);

  // Eingabe
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
  if (_userInput.length() == 0) return;

  int answer = _userInput.toInt();
  bool correct = (answer == _result);

  if (_penaltyMode) {
    _penaltyAnswered = true;
    _penaltyCorrect = correct;
    return;
  }

  showAnimationScreen(correct);

  if (correct) {
    _playerPosition++;
    if (_playerPosition >= _stepsForGoal) {
      _goalsScored++;
      _playerPosition = 0;
    }
  } else {
    _goalsAgainst++;
    _playerPosition = 0;
  }

  tft.fillScreen(TFT_BLACK);
  _userInput = "";
  generateTask();
  _answerStartTime = millis();
  drawButtons();
  updateDisplay();
}

// ========== ANIMATIONEN (nutzt SoccerField) ==========

void Mathe::showAnimationScreen(bool correct) {
  tft.fillScreen(TFT_BLACK);

  _soccerField.setScore(_goalsScored, _goalsAgainst);
  _soccerField.drawScoreboard();

  if (correct) {
    int newPos = _playerPosition + 1;

    if (newPos >= _stepsForGoal) {
      // Tor-Animation
      _soccerField.showGoalAnimation(_playerPosition);
    } else {
      // Normaler Schritt
      _soccerField.showCorrectStep(newPos);
    }
  } else {
    // Gegentor-Animation
    _soccerField.showCounterGoalAnimation(_num1, _operation, _num2, _result);
  }
}

void Mathe::showGameOver() {
  _soccerField.showGameOver(_goalsScored, _goalsAgainst);
  homeButton->draw(&tft);
}

void Mathe::showPenaltyResult(bool playerScored, bool cpuScored, int playerTotal, int cpuTotal, int round) {
  _soccerField.showPenaltyResult(playerScored, cpuScored, playerTotal, cpuTotal, round);
}

void Mathe::showChampionCelebration() {
  _soccerField.showChampionCelebration();
}

void Mathe::redrawScreen() {
  tft.fillScreen(TFT_BLACK);
  drawButtons();
  updateDisplay();
}

void Mathe::setTournamentMode(TournamentType type, int playerTeamIdx, int opponentTeamIdx) {
  _soccerField.setTournamentMode(type, playerTeamIdx, opponentTeamIdx);
}

void Mathe::clearTournamentMode() {
  _soccerField.clearTournamentMode();
}
