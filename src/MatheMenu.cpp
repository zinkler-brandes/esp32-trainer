#include "MatheMenu.h"

MatheMenu::MatheMenu() {
  // Zurück-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Menü-Buttons
  int btnWidth = 280;
  int btnHeight = 60;
  int startX = 20;
  int startY = 70;
  int spacing = 20;

  // Button 1: Einzelspiel
  menuButtons[0] = new Button(startX, startY, btnWidth, btnHeight, "Einzelspiel");
  menuButtons[0]->setColors(TFT_BLUE, TFT_WHITE);

  // Button 2: Turniere
  menuButtons[1] = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "Turniere");
  menuButtons[1]->setColors(0xFE60, TFT_BLACK);  // Gold
}

void MatheMenu::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void MatheMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurück-Button
  backButton->draw(&tft);

  // Titel "MATHE" (neben dem Zurück-Button)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(60, 12);
  tft.print("MATHE");

  // Menü-Buttons zeichnen
  for (int i = 0; i < 2; i++) {
    menuButtons[i]->draw(&tft);
  }
}

int MatheMenu::handleTouch(int16_t x, int16_t y) {
  // Zurück-Button
  if (backButton->contains(x, y)) {
    return 0;
  }

  // Menü-Buttons
  for (int i = 0; i < 2; i++) {
    if (menuButtons[i]->contains(x, y)) {
      return i + 1;  // 1=Einzelspiel, 2=Pokal-Modus
    }
  }

  return -1;  // Nichts getroffen
}
