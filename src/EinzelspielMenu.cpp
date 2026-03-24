#include "EinzelspielMenu.h"

EinzelspielMenu::EinzelspielMenu() {
  // Zurück-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // 2x2 Grid Layout
  int btnWidth = 135;
  int btnHeight = 70;
  int startX = 15;
  int startY = 60;
  int spacingX = 15;
  int spacingY = 15;

  // Button 1: +/- (oben links)
  menuButtons[0] = new Button(startX, startY, btnWidth, btnHeight, "+/-");
  menuButtons[0]->setColors(TFT_BLUE, TFT_WHITE);

  // Button 2: 1x1 (oben rechts)
  menuButtons[1] = new Button(startX + btnWidth + spacingX, startY, btnWidth, btnHeight, "1x1");
  menuButtons[1]->setColors(TFT_PURPLE, TFT_WHITE);

  // Button 3: 1:1 (unten links)
  menuButtons[2] = new Button(startX, startY + btnHeight + spacingY, btnWidth, btnHeight, "1:1");
  menuButtons[2]->setColors(TFT_DARKCYAN, TFT_WHITE);

  // Button 4: Alle (unten rechts)
  menuButtons[3] = new Button(startX + btnWidth + spacingX, startY + btnHeight + spacingY, btnWidth, btnHeight, "Alle");
  menuButtons[3]->setColors(TFT_ORANGE, TFT_WHITE);
}

void EinzelspielMenu::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void EinzelspielMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurück-Button
  backButton->draw(&tft);

  // Titel (neben dem Zurück-Button)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 15);
  tft.print("EINZELSPIEL");

  // Menü-Buttons zeichnen (2x2 Grid)
  for (int i = 0; i < 4; i++) {
    menuButtons[i]->draw(&tft);
  }
}

int EinzelspielMenu::handleTouch(int16_t x, int16_t y) {
  // Zurück-Button
  if (backButton->contains(x, y)) {
    return 0;
  }

  // Menü-Buttons
  for (int i = 0; i < 4; i++) {
    if (menuButtons[i]->contains(x, y)) {
      return i + 1;  // 1=+/-, 2=1x1, 3=1:1, 4=Alle
    }
  }

  return -1;  // Nichts getroffen
}
