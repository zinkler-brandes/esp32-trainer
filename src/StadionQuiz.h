#ifndef SRC_STADIONQUIZ_H
#define SRC_STADIONQUIZ_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

// Medaillen-Level
#define STADION_MEDAL_NONE    0
#define STADION_MEDAL_BRONZE  1
#define STADION_MEDAL_SILVER  2
#define STADION_MEDAL_GOLD    3

// Meilensteine
#define STADION_MILESTONE_BRONZE  5
#define STADION_MILESTONE_SILVER  10
#define STADION_MILESTONE_GOLD    15

class StadionQuiz {
public:
  StadionQuiz();
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
  int _currentStadion;    // Aktueller Stadion-Index
  int _correctAnswer;     // Index der richtigen Antwort (0-3)
  int _answers[4];        // Club-Indices fuer 4 Buttons
  bool _usedStadien[60];  // Welche Stadien schon dran waren
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
