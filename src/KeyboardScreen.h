#ifndef SRC_KEYBOARDSCREEN_H
#define SRC_KEYBOARDSCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

#define KB_MAX_NAME_LENGTH 8
#define KB_MIN_NAME_LENGTH 2

class KeyboardScreen {
  public:
    KeyboardScreen();

    void init();
    void draw();
    int handleTouch(int16_t x, int16_t y);  // 1=OK (Name fertig), 2=Abbrechen, 0=weiter

    String getName();
    void setName(const String& name);
    void clear();

  private:
    TFT_eSPI tft;

    // Tastatur-Buttons
    Button* keys[30];  // A-Z, Backspace, Space
    Button* btnOk;
    Button* btnCancel;

    String _name;

    void createKeys();
    void updateNameDisplay();
};

#endif
