#ifndef SRC_KENNZEICHENQUIZ_H
#define SRC_KENNZEICHENQUIZ_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

// Medaillen-Level
#define KFZ_MEDAL_NONE    0
#define KFZ_MEDAL_BRONZE  1
#define KFZ_MEDAL_SILVER  2
#define KFZ_MEDAL_GOLD    3

// Meilensteine
#define KFZ_MILESTONE_BRONZE  5
#define KFZ_MILESTONE_SILVER  10
#define KFZ_MILESTONE_GOLD    15

class KennzeichenQuiz {
public:
  KennzeichenQuiz();
  void init();
  void draw();
  int handleTouch(int16_t x, int16_t y);  // 0=Zurueck, 1=Weiter, 2=GameOver

  // Getter fuer Ergebnis
  int getHighestMedal();
  int getCorrectCount();

private:
  TFT_eSPI tft;

  // UI-Elemente
  Button* backButton;
  Button* answerButtons[4];

  // Spielzustand
  int _lives;
  int _correctCount;
  int _currentKennzeichen;  // Aktueller Index
  int _correctAnswer;       // Index der richtigen Antwort (0-3)
  int _answers[4];          // Kennzeichen-Indices fuer 4 Buttons
  bool _usedKennzeichen[320];  // Welche schon dran waren
  bool _wasCorrect;
  bool _gameOver;
  int _highestMedal;

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
};

#endif
