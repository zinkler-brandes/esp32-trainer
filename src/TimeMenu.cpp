#include "TimeMenu.h"

TimeMenu::TimeMenu() {
  int btnWidth = 280;
  int btnHeight = 45;
  int startX = 20;
  int startY = 45;
  int spacing = 12;

  // Button 1: 1 Minute
  menuButtons[0] = new Button(startX, startY, btnWidth, btnHeight, "1 Minute");
  menuButtons[0]->setColors(TFT_DARKGREEN, TFT_WHITE);

  // Button 2: 3 Minuten
  menuButtons[1] = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "3 Minuten");
  menuButtons[1]->setColors(TFT_DARKGREEN, TFT_WHITE);

  // Button 3: 5 Minuten
  menuButtons[2] = new Button(startX, startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "5 Minuten");
  menuButtons[2]->setColors(TFT_DARKGREEN, TFT_WHITE);

  // Zurück-Button
  backButton = new Button(startX, 205, btnWidth, 30, "Zurueck");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);
}

void TimeMenu::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void TimeMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(80, 15);
  tft.println("Spielzeit");

  // Buttons zeichnen
  for (int i = 0; i < 3; i++) {
    menuButtons[i]->draw(&tft);
  }
  backButton->draw(&tft);
}

int TimeMenu::handleTouch(int16_t x, int16_t y) {
  // Zurück-Button
  if (backButton->contains(x, y)) {
    return 0;
  }

  // Menü-Buttons
  for (int i = 0; i < 3; i++) {
    if (menuButtons[i]->contains(x, y)) {
      return i + 1;  // 1=1Min, 2=3Min, 3=5Min
    }
  }

  return -1;  // Nichts getroffen
}
