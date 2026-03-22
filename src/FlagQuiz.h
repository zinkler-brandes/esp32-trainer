#ifndef SRC_FLAGQUIZ_H
#define SRC_FLAGQUIZ_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

// Medaillen-Level
#define MEDAL_NONE    0
#define MEDAL_BRONZE  1
#define MEDAL_SILVER  2
#define MEDAL_GOLD    3

// Meilensteine
#define MILESTONE_BRONZE  5
#define MILESTONE_SILVER  10
#define MILESTONE_GOLD    15

// Anzahl Konfliktgruppen (aehnliche Flaggen)
#define CONFLICT_GROUP_COUNT 6

class FlagQuiz {
public:
  FlagQuiz();
  void init();
  void draw();
  int handleTouch(int16_t x, int16_t y);  // 0=Zurueck, 1=Weiter, 2=GameOver

  // Getter fuer Ergebnis
  int getHighestMedal();  // 0=keine, 1=Bronze, 2=Silber, 3=Gold
  int getCorrectCount();

private:
  TFT_eSPI tft;

  // UI-Elemente
  Button* backButton;
  Button* answerButtons[4];

  // Spielzustand
  int _lives;           // 3 Leben
  int _correctCount;    // Richtige Antworten (0-15)
  int _currentFlag;     // Aktueller Flaggen-Index
  int _correctAnswer;   // Index der richtigen Antwort (0-3)
  int _answers[4];      // Team-Indices fuer 4 Buttons
  bool _usedFlags[48];  // Welche Flaggen schon dran waren
  bool _wasCorrect;     // Letzte Antwort war richtig
  bool _gameOver;
  int _highestMedal;    // Hoechste erreichte Medaille

  // Konfliktgruppen - Flaggen die nie zusammen erscheinen duerfen
  static const int CONFLICT_GROUPS[CONFLICT_GROUP_COUNT][4];

  // Methoden
  void generateQuestion();
  void drawQuestion();
  void drawLives();
  void drawProgress();
  void drawMedal(int x, int y, uint16_t color);
  void drawJubelndeZuschauer(int frame);
  void showCountdown();
  void showCorrectFeedback();
  void showWrongFeedback(int wrongAnswer);
  void showMilestoneAnimation(int medal);
  void showGameOverScreen();
  void showConfetti();
  bool areFlagsInConflict(int flag1, int flag2);
  int getConflictGroup(int flagIndex);
};

#endif
