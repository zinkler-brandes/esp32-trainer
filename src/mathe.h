#ifndef SRC_MATHE_H
#define SRC_MATHE_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

class Mathe {
  public:
    Mathe();
    void init();
    void drawButtons();
    void handleButtonPress(int16_t x, int16_t y);

  private:
    TFT_eSPI tft;

    // Buttons für Numpad
    Button* buttons[12];  // 0-9, C, OK

    // Spiel-Logik
    int _num1, _num2, _result;
    char _operation;  // '+' oder '-'
    String _userInput;
    int _score;

    // Methoden
    void generateTask();
    void updateDisplay();
    void addDigit(String digit);
    void clearInput();
    void checkAnswer();
    void showFeedback(bool correct);
};

#endif
