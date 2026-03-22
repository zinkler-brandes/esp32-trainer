#include "TournamentMenu.h"

// ========== Tournament Menu (DFB/CL Auswahl) ==========

TournamentMenu::TournamentMenu() {
  int btnWidth = 280;
  int btnHeight = 55;
  int startX = 20;
  int startY = 50;
  int spacing = 20;

  // DFB-Pokal Button (Rot/Gold)
  dfbButton = new Button(startX, startY, btnWidth, btnHeight, "DFB-Pokal");
  dfbButton->setColors(0xF800, TFT_WHITE);  // Rot

  // Champions League Button (Blau/Silber)
  clButton = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "Champions League");
  clButton->setColors(0x001F, TFT_WHITE);  // Blau

  // Zurück-Button
  backButton = new Button(startX, 200, btnWidth, 35, "Zurueck");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);
}

void TournamentMenu::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void TournamentMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(70, 15);
  tft.println("Pokal-Modus");

  // Buttons zeichnen
  dfbButton->draw(&tft);
  clButton->draw(&tft);
  backButton->draw(&tft);
}

int TournamentMenu::handleTouch(int16_t x, int16_t y) {
  if (backButton->contains(x, y)) {
    return 0;  // Zurück
  }
  if (dfbButton->contains(x, y)) {
    return 1;  // DFB-Pokal
  }
  if (clButton->contains(x, y)) {
    return 2;  // Champions League
  }
  return -1;
}

// ========== Tournament Mathe Mode Menu (für DFB-Pokal) ==========

TournamentMatheModeMenu::TournamentMatheModeMenu() {
  int btnWidth = 280;
  int btnHeight = 55;
  int startX = 20;
  int startY = 50;
  int spacing = 20;

  // +/- Button
  plusMinusButton = new Button(startX, startY, btnWidth, btnHeight, "+/-");
  plusMinusButton->setColors(TFT_BLUE, TFT_WHITE);

  // 1x1 Button
  multiplyButton = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "1x1");
  multiplyButton->setColors(TFT_PURPLE, TFT_WHITE);

  // Zurück-Button
  backButton = new Button(startX, 200, btnWidth, 35, "Zurueck");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);
}

void TournamentMatheModeMenu::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void TournamentMatheModeMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(50, 15);
  tft.println("Rechenart waehlen");

  // Buttons zeichnen
  plusMinusButton->draw(&tft);
  multiplyButton->draw(&tft);
  backButton->draw(&tft);
}

int TournamentMatheModeMenu::handleTouch(int16_t x, int16_t y) {
  if (backButton->contains(x, y)) {
    return 0;  // Zurück
  }
  if (plusMinusButton->contains(x, y)) {
    return 1;  // +/-
  }
  if (multiplyButton->contains(x, y)) {
    return 2;  // 1x1
  }
  return -1;
}
