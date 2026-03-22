#ifndef SRC_MATHE_H
#define SRC_MATHE_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "SoccerField.h"
#include "Teams.h"

enum MatheMode {
  MATHE_PLUS_MINUS,   // 0: Addition und Subtraktion (+/-)
  MATHE_MULTIPLY,     // 1: Kleines 1x1 (Multiplikation)
  MATHE_DIVIDE,       // 2: Division (1:1)
  MATHE_ALL           // 3: Alle gemischt (+/-, 1x1, 1:1)
};

class Mathe {
  public:
    Mathe();
    void init(MatheMode mode, unsigned long durationMs, unsigned long answerTimeMs, int stepsForGoal, int nachspielzeitSek = 0);
    void drawButtons();
    int handleButtonPress(int16_t x, int16_t y);  // 0=nichts, 1=Home (GameOver), 2=Exit angefragt
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
    void redrawScreen();             // Vollständiges Neuzeichnen (nach Dialog)

    // Turnier-Modus fuer Logos/Flaggen auf Scoreboard
    void setTournamentMode(TournamentType type, int playerTeamIdx = -1, int opponentTeamIdx = -1);
    void clearTournamentMode();

  private:
    TFT_eSPI tft;
    SoccerField _soccerField;  // Spielfeld-Visualisierung

    // Buttons für Numpad
    Button* buttons[12];  // 0-9, <-, OK
    Button* homeButton;   // Zurück zum Menü (bei Game Over)
    Button* exitButton;   // Abbrechen (rotes X, oben rechts)

    // Spiel-Logik
    MatheMode _mode;
    int _num1, _num2, _result;
    char _operation;  // '+', '-', '*' oder '/'
    String _userInput;

    // Fußball-Motivation
    int _playerPosition;  // 0-N (N Schritte bis zum Tor)
    int _goalsScored;     // Spieler-Tore
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
    void showAnimationScreen(bool correct);
    void showGameOver();
};

#endif
