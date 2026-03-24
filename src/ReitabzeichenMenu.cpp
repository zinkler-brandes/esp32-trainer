#include "ReitabzeichenMenu.h"

ReitabzeichenMenu::ReitabzeichenMenu() {
  // Zurueck-Button
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // 3 Buttons vertikal
  int btnWidth = 200;
  int btnHeight = 50;
  int centerX = 60;
  int startY = 55;
  int spacing = 10;

  // Bronze
  menuButtons[0] = new Button(centerX, startY, btnWidth, btnHeight, "Bronze");
  menuButtons[0]->setColors(0xB346, TFT_WHITE);  // Bronze-Farbe

  // Silber
  menuButtons[1] = new Button(centerX, startY + btnHeight + spacing, btnWidth, btnHeight, "Silber");
  menuButtons[1]->setColors(0xC618, TFT_BLACK);  // Silber-Farbe

  // Gold
  menuButtons[2] = new Button(centerX, startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "Gold");
  menuButtons[2]->setColors(0xFE60, TFT_BLACK);  // Gold-Farbe
}

void ReitabzeichenMenu::init() {
  tft.init();
  tft.setRotation(1);
}

void ReitabzeichenMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurueck-Button
  backButton->draw(&tft);

  // Ueberschrift
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 15);
  tft.print("Reitabzeichen");

  // Menu-Buttons
  for (int i = 0; i < 3; i++) {
    menuButtons[i]->draw(&tft);
  }

  // Beschreibungen rechts neben den Buttons
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);

  // Bronze: 6 Hindernisse, 15 Sek
  tft.setCursor(270, 70);
  tft.print("6 H.");
  tft.setCursor(270, 82);
  tft.print("15s");

  // Silber: 8 Hindernisse, 10 Sek
  tft.setCursor(270, 130);
  tft.print("8 H.");
  tft.setCursor(270, 142);
  tft.print("10s");

  // Gold: 10 Hindernisse, 6 Sek
  tft.setCursor(270, 190);
  tft.print("10 H.");
  tft.setCursor(270, 202);
  tft.print("6s");

  // Hinweis unten
  tft.setCursor(30, 225);
  tft.print("H. = Hindernisse, s = Antwortzeit");
}

int ReitabzeichenMenu::handleTouch(int16_t x, int16_t y) {
  if (backButton->contains(x, y)) {
    return 0;  // Zurueck
  }

  for (int i = 0; i < 3; i++) {
    if (menuButtons[i]->contains(x, y)) {
      return i + 1;  // 1=Bronze, 2=Silber, 3=Gold
    }
  }

  return -1;  // Nichts
}

void ReitabzeichenMenu::drawMedal(int x, int y, uint16_t color, const char* text) {
  // Medaillen-Icon
  tft.fillCircle(x, y, 12, color);
  tft.drawCircle(x, y, 12, TFT_WHITE);
  tft.drawCircle(x, y, 11, TFT_WHITE);

  // Band
  tft.fillTriangle(x - 8, y - 12, x, y - 20, x + 8, y - 12, TFT_RED);
}
