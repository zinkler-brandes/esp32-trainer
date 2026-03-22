#include "MatheMenu.h"

MatheMenu::MatheMenu() {
  int btnWidth = 280;
  int btnHeight = 45;
  int startX = 20;
  int startY = 45;
  int spacing = 12;

  // Button 1: +/- (Addition/Subtraktion)
  menuButtons[0] = new Button(startX, startY, btnWidth, btnHeight, "+/-");
  menuButtons[0]->setColors(TFT_BLUE, TFT_WHITE);

  // Button 2: 1x1 (Multiplikation)
  menuButtons[1] = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "1x1");
  menuButtons[1]->setColors(TFT_PURPLE, TFT_WHITE);

  // Button 3: Pokal-Modus
  menuButtons[2] = new Button(startX, startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "Pokal-Modus");
  menuButtons[2]->setColors(0xFE60, TFT_BLACK);  // Gold

  // Zurück-Button
  backButton = new Button(startX, 205, btnWidth, 30, "Zurueck");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);
}

void MatheMenu::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void MatheMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(120, 15);
  tft.println("Mathe");

  // Buttons zeichnen
  for (int i = 0; i < 3; i++) {
    menuButtons[i]->draw(&tft);
  }
  backButton->draw(&tft);
}

int MatheMenu::handleTouch(int16_t x, int16_t y) {
  // Zurück-Button
  if (backButton->contains(x, y)) {
    return 0;
  }

  // Menü-Buttons
  for (int i = 0; i < 3; i++) {
    if (menuButtons[i]->contains(x, y)) {
      return i + 1;  // 1=+/-, 2=1x1, 3=Pokal
    }
  }

  return -1;  // Nichts getroffen
}
