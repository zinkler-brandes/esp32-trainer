#ifndef SRC_MATHE_H
#define SRC_MATHE_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

enum MatheMode {
  MATHE_PLUS_MINUS,   // Addition und Subtraktion
  MATHE_MULTIPLY,     // Kleines 1x1
  MATHE_MIXED         // Beides gemischt (für Champions League)
};

class Mathe {
  public:
    Mathe();
    void init(MatheMode mode, unsigned long durationMs, unsigned long answerTimeMs, int stepsForGoal, int nachspielzeitSek = 0);
    void showNachspielzeitTafel(int sekunden);
    void drawButtons();
    bool handleButtonPress(int16_t x, int16_t y);  // Gibt true zurück wenn Home gedrückt
    void update();  // Timer prüfen und anzeigen, in main loop aufrufen
    bool isGameOver();
    int getRemainingSeconds();
    int getGoalsScored() { return _goalsScored; }
    int getGoalsAgainst() { return _goalsAgainst; }

    // Penalty-Modus (einzelne Aufgabe)
    void initPenalty(MatheMode mode, unsigned long answerTimeMs);
    bool isPenaltyAnswered() { return _penaltyAnswered; }
    bool wasPenaltyCorrect() { return _penaltyCorrect; }
    void showPenaltyResult(bool playerScored, bool cpuScored, int playerTotal, int cpuTotal, int round);
    void updateTimerDisplay();       // Spielzeit-Timer neu zeichnen
    void updateAnswerTimerDisplay(); // Antwort-Timer neu zeichnen
    void handleAnswerTimeout();      // Timeout = Gegentor

  private:
    TFT_eSPI tft;

    // Buttons für Numpad
    Button* buttons[12];  // 0-9, C, OK
    Button* homeButton;   // Zurück zum Menü

    // Spiel-Logik
    MatheMode _mode;
    int _num1, _num2, _result;
    char _operation;  // '+', '-' oder '*'
    String _userInput;

    // Fußball-Motivation
    int _playerPosition;  // 0-9 (10 Schritte bis zum Tor)
    int _goalsScored;     // Leo's Tore
    int _goalsAgainst;    // Gegentore

    // Timer (Spielzeit)
    unsigned long _gameStartTime;
    unsigned long _gameDuration;
    unsigned long _nachspielzeit;
    bool _nachspielzeitShown;
    bool _gameOver;

    // Antwort-Timer und Schwierigkeit
    unsigned long _answerStartTime;
    unsigned long _answerTimeLimit;  // Zeit pro Antwort in ms
    int _stepsForGoal;               // Richtige Antworten für ein Tor

    // Penalty-Modus
    bool _penaltyMode;
    bool _penaltyAnswered;
    bool _penaltyCorrect;

    // Methoden
    void generateTask();
    void updateDisplay();
    void addDigit(String digit);
    void backspace();
    void checkAnswer();
    void showFeedback(bool correct);
    void drawField(int crossedFields, int totalFields);
    void drawPlayer(int position, bool withBall, int totalSteps);
    void drawGoalkeeper();
    void drawGoal();
    void drawScoreboard();
    void showAnimationScreen(bool correct);
    void showGameOver();
};

#endif
