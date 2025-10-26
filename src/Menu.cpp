#include "Menu.h"

Menu::Menu() {
  // 3 große Buttons für die Menü-Optionen
  int btnWidth = 280;
  int btnHeight = 50;
  int startX = 20;
  int startY = 50;
  int spacing = 20;

  // Button 1: Mathe-Trainer
  menuButtons[0] = new Button(startX, startY, btnWidth, btnHeight, "Mathe-Trainer");
  menuButtons[0]->setColors(TFT_BLUE, TFT_WHITE);

  // Button 2: Fussball-Ergebnisse
  menuButtons[1] = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "Fussball-Ergebnisse");
  menuButtons[1]->setColors(TFT_DARKGREEN, TFT_WHITE);

  // Button 3: Fussball-Tabellen
  menuButtons[2] = new Button(startX, startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "Fussball-Tabellen");
  menuButtons[2]->setColors(TFT_DARKGREEN, TFT_WHITE);
}

void Menu::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void Menu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(40, 10);
  tft.println("Leos Trainer");

  // Menü-Buttons zeichnen
  for (int i = 0; i < 3; i++) {
    menuButtons[i]->draw(&tft);
  }
}

int Menu::handleTouch(int16_t x, int16_t y) {
  for (int i = 0; i < 3; i++) {
    if (menuButtons[i]->contains(x, y)) {
      return i + 1;  // 1=Mathe, 2=Fussball-Ergebnisse, 3=Fussball-Tabellen
    }
  }
  return 0;  // Nichts getroffen
}
