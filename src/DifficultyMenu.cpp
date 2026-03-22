#include "DifficultyMenu.h"

// Schwierigkeitsgrade definieren
static const Difficulty difficulties[] = {
  {"", 0, 0},                      // Platzhalter für Index 0
  {"FC Bayern", 6000, 5},          // Schwer: 6 Sek, 5 Schritte
  {"Dortmund", 10000, 5},          // Mittel: 10 Sek, 5 Schritte
  {"Hannover 96", 15000, 5}        // Leicht: 15 Sek, 5 Schritte
};

DifficultyMenu::DifficultyMenu() {
  int btnWidth = 280;
  int btnHeight = 45;
  int startX = 20;
  int startY = 45;
  int spacing = 10;

  // Button 1: FC Bayern (schwer) - Rot
  menuButtons[0] = new Button(startX, startY, btnWidth, btnHeight, "FC Bayern (schwer)");
  menuButtons[0]->setColors(TFT_RED, TFT_WHITE);

  // Button 2: Dortmund (mittel) - Gelb
  menuButtons[1] = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "Dortmund (mittel)");
  menuButtons[1]->setColors(TFT_YELLOW, TFT_BLACK);

  // Button 3: Hannover 96 (leicht) - Grün
  menuButtons[2] = new Button(startX, startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "Hannover 96 (leicht)");
  menuButtons[2]->setColors(TFT_DARKGREEN, TFT_WHITE);

  // Zurück-Button
  backButton = new Button(startX, 205, btnWidth, 30, "Zurueck");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);
}

void DifficultyMenu::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void DifficultyMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(50, 15);
  tft.println("Schwierigkeit");

  // Buttons zeichnen
  for (int i = 0; i < 3; i++) {
    menuButtons[i]->draw(&tft);
  }
  backButton->draw(&tft);
}

int DifficultyMenu::handleTouch(int16_t x, int16_t y) {
  // Zurück-Button
  if (backButton->contains(x, y)) {
    return 0;
  }

  // Menü-Buttons
  for (int i = 0; i < 3; i++) {
    if (menuButtons[i]->contains(x, y)) {
      return i + 1;  // 1=Bayern, 2=Dortmund, 3=Hannover
    }
  }

  return -1;  // Nichts getroffen
}

Difficulty DifficultyMenu::getDifficulty(int choice) {
  if (choice >= 1 && choice <= 3) {
    return difficulties[choice];
  }
  return difficulties[2];  // Default: Dortmund
}
