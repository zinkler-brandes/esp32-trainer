#include "ConfirmDialog.h"

ConfirmDialog::ConfirmDialog() {
  // Buttons erstellen
  int btnWidth = 100;
  int btnHeight = 40;
  int y = 160;

  // "Ja" Button (links)
  btnYes = new Button(50, y, btnWidth, btnHeight, "JA");
  btnYes->setColors(TFT_GREEN, TFT_WHITE);

  // "Nein" Button (rechts)
  btnNo = new Button(170, y, btnWidth, btnHeight, "NEIN");
  btnNo->setColors(TFT_RED, TFT_WHITE);
}

void ConfirmDialog::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void ConfirmDialog::show(const String& title, const String& message) {
  tft.fillScreen(TFT_BLACK);

  // Hintergrund für Dialog
  tft.fillRoundRect(20, 40, 280, 170, 10, TFT_DARKGREY);
  tft.drawRoundRect(20, 40, 280, 170, 10, TFT_WHITE);

  // Titel
  tft.setTextColor(TFT_YELLOW, TFT_DARKGREY);
  tft.setTextSize(2);

  // Titel zentrieren
  int titleX = 160 - (title.length() * 6);
  if (titleX < 30) titleX = 30;
  tft.setCursor(titleX, 55);
  tft.println(title);

  // Nachricht (mit Zeilenumbruch)
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setTextSize(2);

  // Nachricht auf mehrere Zeilen verteilen wenn nötig
  int msgX = 35;
  int msgY = 90;
  int maxCharsPerLine = 20;

  String remaining = message;
  while (remaining.length() > 0 && msgY < 150) {
    String line;
    if ((int)remaining.length() <= maxCharsPerLine) {
      line = remaining;
      remaining = "";
    } else {
      // Am Leerzeichen trennen
      int lastSpace = remaining.lastIndexOf(' ', maxCharsPerLine);
      if (lastSpace > 0) {
        line = remaining.substring(0, lastSpace);
        remaining = remaining.substring(lastSpace + 1);
      } else {
        line = remaining.substring(0, maxCharsPerLine);
        remaining = remaining.substring(maxCharsPerLine);
      }
    }

    tft.setCursor(msgX, msgY);
    tft.println(line);
    msgY += 22;
  }

  // Buttons zeichnen
  btnYes->draw(&tft);
  btnNo->draw(&tft);
}

int ConfirmDialog::handleTouch(int16_t x, int16_t y) {
  if (btnYes->contains(x, y)) {
    return 1;  // Ja
  }
  if (btnNo->contains(x, y)) {
    return 2;  // Nein
  }
  return 0;  // Nichts getroffen
}
