#include "DifficultyMenu.h"

// Schwierigkeitsgrade definieren (Index 1-4)
static const Difficulty difficulties[] = {
  {"", 0, 0},                          // Platzhalter für Index 0
  {"St. Vit", 0, 5},                   // Erlernen: keine Zeitbegrenzung, 5 Schritte
  {"Hannover 96", 15000, 5},           // Leicht: 15 Sek, 5 Schritte
  {"Dortmund", 10000, 5},              // Mittel: 10 Sek, 5 Schritte
  {"FC Bayern", 6000, 5}               // Schwer: 6 Sek, 5 Schritte
};

DifficultyMenu::DifficultyMenu() {
  // Zurück-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Menü-Buttons
  int btnWidth = 280;
  int btnHeight = 38;
  int startX = 20;
  int startY = 50;
  int spacing = 8;

  // Button 1: St. Vit (erlernen) - Hellblau
  menuButtons[0] = new Button(startX, startY, btnWidth, btnHeight, "St. Vit (erlernen)");
  menuButtons[0]->setColors(TFT_CYAN, TFT_BLACK);

  // Button 2: Hannover 96 (leicht) - Grün
  menuButtons[1] = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "Hannover 96 (leicht)");
  menuButtons[1]->setColors(TFT_DARKGREEN, TFT_WHITE);

  // Button 3: Dortmund (mittel) - Gelb
  menuButtons[2] = new Button(startX, startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "Dortmund (mittel)");
  menuButtons[2]->setColors(TFT_YELLOW, TFT_BLACK);

  // Button 4: FC Bayern (schwer) - Rot
  menuButtons[3] = new Button(startX, startY + 3*(btnHeight + spacing), btnWidth, btnHeight, "FC Bayern (schwer)");
  menuButtons[3]->setColors(TFT_RED, TFT_WHITE);
}

void DifficultyMenu::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void DifficultyMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurück-Button
  backButton->draw(&tft);

  // Titel (neben dem Zurück-Button)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(60, 12);
  tft.print("GEGNER");

  // Buttons zeichnen
  for (int i = 0; i < 4; i++) {
    menuButtons[i]->draw(&tft);
  }
}

int DifficultyMenu::handleTouch(int16_t x, int16_t y) {
  // Zurück-Button
  if (backButton->contains(x, y)) {
    return 0;
  }

  // Menü-Buttons
  for (int i = 0; i < 4; i++) {
    if (menuButtons[i]->contains(x, y)) {
      return i + 1;  // 1=St.Vit, 2=Hannover, 3=Dortmund, 4=Bayern
    }
  }

  return -1;  // Nichts getroffen
}

Difficulty DifficultyMenu::getDifficulty(int choice) {
  if (choice >= 1 && choice <= 4) {
    return difficulties[choice];
  }
  return difficulties[2];  // Default: Hannover
}
