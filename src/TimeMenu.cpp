#include "TimeMenu.h"

TimeMenu::TimeMenu() {
  // Zurück-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Menü-Buttons
  int btnWidth = 280;
  int btnHeight = 50;
  int startX = 20;
  int startY = 55;
  int spacing = 12;

  // Button 1: 1 Minute
  menuButtons[0] = new Button(startX, startY, btnWidth, btnHeight, "1 Minute");
  menuButtons[0]->setColors(TFT_DARKGREEN, TFT_WHITE);

  // Button 2: 2 Minuten
  menuButtons[1] = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "2 Minuten");
  menuButtons[1]->setColors(TFT_DARKGREEN, TFT_WHITE);

  // Button 3: 3 Minuten
  menuButtons[2] = new Button(startX, startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "3 Minuten");
  menuButtons[2]->setColors(TFT_DARKGREEN, TFT_WHITE);
}

void TimeMenu::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void TimeMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurück-Button
  backButton->draw(&tft);

  // Titel (neben dem Zurück-Button)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(60, 12);
  tft.print("SPIELZEIT");

  // Buttons zeichnen
  for (int i = 0; i < 3; i++) {
    menuButtons[i]->draw(&tft);
  }
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
