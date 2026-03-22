#ifndef SRC_CLUBQUIZ_H
#define SRC_CLUBQUIZ_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

// Medaillen-Level (wie bei FlagQuiz)
#define CLUB_MEDAL_NONE    0
#define CLUB_MEDAL_BRONZE  1
#define CLUB_MEDAL_SILVER  2
#define CLUB_MEDAL_GOLD    3

// Meilensteine
#define CLUB_MILESTONE_BRONZE  5
#define CLUB_MILESTONE_SILVER  10
#define CLUB_MILESTONE_GOLD    15

class ClubQuiz {
public:
  ClubQuiz();
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
  int _currentClub;       // Aktueller Club-Index
  int _correctAnswer;     // Index der richtigen Antwort (0-3)
  int _answers[4];        // Bundesland-Indices fuer 4 Buttons
  bool _usedClubs[60];    // Welche Clubs schon dran waren (max 60)
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
