#ifndef SRC_CONFIRMDIALOG_H
#define SRC_CONFIRMDIALOG_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

class ConfirmDialog {
  public:
    ConfirmDialog();

    void init();
    void show(const String& title, const String& message);
    int handleTouch(int16_t x, int16_t y);  // 1=Ja, 2=Nein, 0=nichts

  private:
    TFT_eSPI tft;
    Button* btnYes;
    Button* btnNo;
};

#endif
