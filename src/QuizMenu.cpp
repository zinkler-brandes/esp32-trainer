#include "QuizMenu.h"

QuizMenu::QuizMenu() {
  // Zurueck-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Button-Layout (4 Buttons untereinander)
  int btnWidth = 200;
  int btnHeight = 40;
  int startX = 60;
  int startY = 50;
  int spacing = 10;

  // Flaggen-Quiz
  flaggenButton = new Button(startX, startY, btnWidth, btnHeight, "WM Flaggen");
  flaggenButton->setColors(0xF800, TFT_WHITE);  // Rot

  // Vereine-Quiz
  vereineButton = new Button(startX, startY + btnHeight + spacing, btnWidth, btnHeight, "Vereine");
  vereineButton->setColors(0x001F, TFT_WHITE);  // Blau

  // Stadien-Quiz
  stadienButton = new Button(startX, startY + 2 * (btnHeight + spacing), btnWidth, btnHeight, "Stadien");
  stadienButton->setColors(0x07E0, TFT_BLACK);  // Gruen

  // Kennzeichen-Quiz
  kennzeichenButton = new Button(startX, startY + 3 * (btnHeight + spacing), btnWidth, btnHeight, "Kennzeichen");
  kennzeichenButton->setColors(0xFFE0, TFT_BLACK);  // Gelb
}

void QuizMenu::init() {
  tft.setRotation(1);
}

void QuizMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurueck-Button
  backButton->draw(&tft);

  // Titel
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(100, 12);
  tft.print("QUIZ");

  // Buttons zeichnen
  flaggenButton->draw(&tft);
  vereineButton->draw(&tft);
  stadienButton->draw(&tft);
  kennzeichenButton->draw(&tft);
}

int QuizMenu::handleTouch(int16_t x, int16_t y) {
  if (backButton->contains(x, y)) {
    return 0;  // Zurueck
  }

  if (flaggenButton->contains(x, y)) {
    return 1;  // Flaggen-Quiz
  }

  if (vereineButton->contains(x, y)) {
    return 2;  // Vereine-Quiz
  }

  if (stadienButton->contains(x, y)) {
    return 3;  // Stadien-Quiz
  }

  if (kennzeichenButton->contains(x, y)) {
    return 4;  // Kennzeichen-Quiz
  }

  return -1;
}
