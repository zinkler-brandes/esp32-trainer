#include "ParcoursMenu.h"

ParcoursMenu::ParcoursMenu() {
  // Zurueck-Button
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // 2x2 Grid
  int btnWidth = 130;
  int btnHeight = 70;
  int leftX = 20;
  int rightX = 170;
  int topY = 60;
  int bottomY = 145;

  // +/- (oben links)
  menuButtons[0] = new Button(leftX, topY, btnWidth, btnHeight, "+/-");
  menuButtons[0]->setColors(0xB2A0, TFT_WHITE);  // Hellbraun

  // 1x1 (oben rechts)
  menuButtons[1] = new Button(rightX, topY, btnWidth, btnHeight, "1x1");
  menuButtons[1]->setColors(0xB2A0, TFT_WHITE);

  // 1:1 (unten links)
  menuButtons[2] = new Button(leftX, bottomY, btnWidth, btnHeight, "1:1");
  menuButtons[2]->setColors(0xB2A0, TFT_WHITE);

  // Alle (unten rechts)
  menuButtons[3] = new Button(rightX, bottomY, btnWidth, btnHeight, "Alle");
  menuButtons[3]->setColors(0xFE60, TFT_BLACK);  // Gold
}

void ParcoursMenu::init() {
  tft.init();
  tft.setRotation(1);
}

void ParcoursMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurueck-Button
  backButton->draw(&tft);

  // Ueberschrift
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(80, 15);
  tft.print("Rechenart");

  // Pferd-Icon
  tft.fillEllipse(270, 25, 15, 8, 0x8200);
  tft.fillCircle(282, 18, 6, 0x8200);

  // Menu-Buttons
  for (int i = 0; i < 4; i++) {
    menuButtons[i]->draw(&tft);
  }

  // Erklaerungen unter den Buttons
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(30, 225);
  tft.print("Waehle die Rechenart fuer den Parcours");
}

int ParcoursMenu::handleTouch(int16_t x, int16_t y) {
  if (backButton->contains(x, y)) {
    return 0;  // Zurueck
  }

  for (int i = 0; i < 4; i++) {
    if (menuButtons[i]->contains(x, y)) {
      return i + 1;  // 1=+/-, 2=1x1, 3=1:1, 4=Alle
    }
  }

  return -1;  // Nichts
}
