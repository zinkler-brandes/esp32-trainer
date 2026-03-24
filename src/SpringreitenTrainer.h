#ifndef SRC_SPRINGREITEN_TRAINER_H
#define SRC_SPRINGREITEN_TRAINER_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "ParcoursField.h"
#include "mathe.h"  // Fuer MatheMode enum

// Schwierigkeitsgrade (Reitabzeichen)
enum Reitabzeichen {
  REITABZEICHEN_BRONZE,   // 15 Sek, 6 Hindernisse, 90 Sek
  REITABZEICHEN_SILBER,   // 10 Sek, 8 Hindernisse, 75 Sek
  REITABZEICHEN_GOLD      // 6 Sek, 10 Hindernisse, 60 Sek
};

struct ReitDifficulty {
  int obstacleCount;       // Anzahl Hindernisse
  unsigned long answerTimeMs;  // Zeit pro Antwort
  unsigned long parcoursTimeMs;  // Gesamt-Parcourszeit
  const char* name;
};

class SpringreitenTrainer {
  public:
    SpringreitenTrainer();

    // Spiel starten
    void init(MatheMode mode, Reitabzeichen difficulty);
    void drawButtons();
    int handleButtonPress(int16_t x, int16_t y);  // 0=nichts, 1=Home, 2=Exit
    void update();  // Timer pruefen
    bool isGameOver();

    // Stechen-Modus (einzelnes Hindernis, Zeit zaehlt)
    void initStechen(MatheMode mode, int stechenNum);
    bool isStechenAnswered() { return _stechenAnswered; }
    bool wasStechenCorrect() { return _stechenCorrect; }
    float getStechenTime() { return _stechenTime; }

    // Getter
    int getFaults() { return _faults; }
    float getTotalTime() { return _totalTime; }
    int getCurrentObstacle() { return _currentObstacle; }

    // Display
    void redrawScreen();

    // Statische Hilfsfunktion
    static ReitDifficulty getDifficulty(Reitabzeichen level);

  private:
    TFT_eSPI tft;
    ParcoursField _parcoursField;

    // Buttons fuer Numpad
    Button* buttons[12];
    Button* homeButton;
    Button* exitButton;

    // Spiel-Logik
    MatheMode _mode;
    int _num1, _num2, _result;
    char _operation;
    String _userInput;

    // Springreiten-Mechanik
    int _faults;           // Fehlerpunkte
    int _obstacleCount;    // Anzahl Hindernisse
    int _currentObstacle;  // Aktuelles Hindernis (0-based)

    // Timer
    unsigned long _gameStartTime;
    unsigned long _parcoursTimeLimit;  // Erlaubte Parcourszeit
    float _totalTime;      // Tatsaechliche Zeit
    bool _gameOver;

    // Antwort-Timer
    unsigned long _answerStartTime;
    unsigned long _answerTimeLimit;

    // Stechen-Modus
    bool _stechenMode;
    bool _stechenAnswered;
    bool _stechenCorrect;
    float _stechenTime;
    int _stechenNum;

    // Aufgaben-History
    static const int TASK_HISTORY_SIZE = 8;
    int _taskHistory[TASK_HISTORY_SIZE];
    int _taskHistoryIdx;

    // Methoden
    void generateTask();
    bool isTaskInHistory(int taskKey);
    void updateDisplay();
    void addDigit(String digit);
    void backspace();
    void checkAnswer();
    void handleAnswerTimeout();
    void showGameOver();
    void updateTimerDisplay();
    void updateAnswerTimerDisplay();
    void calculateTimeFaults();
};

#endif
