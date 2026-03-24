#include "ThemeSelectScreen.h"

ThemeSelectScreen::ThemeSelectScreen() : _playerName("") {
  // Zurueck-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Layout: 3 Buttons nebeneinander, kompakt
  int btnWidth = 95;
  int btnHeight = 120;
  int spacing = 10;
  int startX = 10;
  int startY = 55;

  // Fussball-Button (links, gruen)
  fussballButton = new Button(startX, startY, btnWidth, btnHeight, "");
  fussballButton->setColors(0x3665, TFT_WHITE);  // FIELD_GREEN

  // KiTa-Button (mitte, orange)
  kitaButton = new Button(startX + btnWidth + spacing, startY, btnWidth, btnHeight, "");
  kitaButton->setColors(0xFD20, TFT_BLACK);  // Orange

  // Reiten-Button (rechts, braun/sand)
  reitenButton = new Button(startX + 2 * (btnWidth + spacing), startY, btnWidth, btnHeight, "");
  reitenButton->setColors(0xC618, TFT_BLACK);  // Sandfarbe
}

void ThemeSelectScreen::init() {
  tft.init();
  tft.setRotation(1);
}

void ThemeSelectScreen::setPlayerName(const String& name) {
  _playerName = name;
  if (_playerName.length() > 8) {
    _playerName = _playerName.substring(0, 8);
  }
}

void ThemeSelectScreen::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurueck-Button
  backButton->draw(&tft);

  // Spielername (neben dem Zurueck-Button)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(60, 12);
  tft.print(_playerName);

  // Buttons zeichnen
  fussballButton->draw(&tft);
  kitaButton->draw(&tft);
  reitenButton->draw(&tft);

  // Icons auf den Buttons (zentriert)
  drawSoccerIcon(57, 100);
  drawKitaIcon(162, 105);
  drawHorseIcon(267, 100);

  // Beschriftungen (auf den Buttons, klein und zentriert)
  tft.setTextSize(1);

  // Fussball (weiss auf gruen) - "Fussball" = 48px, Button bei X=10, Breite=95 → Mitte=57
  tft.setTextColor(TFT_WHITE, 0x3665);
  tft.setCursor(33, 158);
  tft.print("Fussball");

  // KiTa (schwarz auf orange) - "KiTa" = 24px, Button bei X=115, Breite=95 → Mitte=162
  tft.setTextColor(TFT_BLACK, 0xFD20);
  tft.setCursor(150, 158);
  tft.print("KiTa");

  // Reiten (schwarz auf sand) - "Reiten" = 36px, Button bei X=220, Breite=95 → Mitte=267
  tft.setTextColor(TFT_BLACK, 0xC618);
  tft.setCursor(249, 158);
  tft.print("Reiten");
}

void ThemeSelectScreen::drawSoccerIcon(int x, int y) {
  // Fussball
  int r = 25;
  tft.fillCircle(x, y, r, TFT_WHITE);
  tft.fillCircle(x, y, 8, TFT_BLACK);
  tft.fillCircle(x - 15, y - 10, 6, TFT_BLACK);
  tft.fillCircle(x + 15, y - 10, 6, TFT_BLACK);
  tft.fillCircle(x - 15, y + 10, 6, TFT_BLACK);
  tft.fillCircle(x + 15, y + 10, 6, TFT_BLACK);
  tft.drawCircle(x, y, r, TFT_DARKGREY);
}

void ThemeSelectScreen::drawKitaIcon(int x, int y) {
  // Kind mit Stift (etwas groesser)
  tft.fillCircle(x, y - 15, 12, 0xFE60);  // Kopf
  tft.fillCircle(x, y - 22, 10, 0x4A00);  // Haare
  tft.fillCircle(x - 7, y - 18, 5, 0x4A00);
  tft.fillCircle(x + 7, y - 18, 5, 0x4A00);
  tft.fillCircle(x - 4, y - 16, 2, TFT_BLACK);  // Augen
  tft.fillCircle(x + 4, y - 16, 2, TFT_BLACK);
  tft.drawLine(x - 3, y - 10, x, y - 8, TFT_RED);  // Mund
  tft.drawLine(x, y - 8, x + 3, y - 10, TFT_RED);
  tft.fillRect(x - 10, y, 20, 18, TFT_CYAN);  // Koerper
  tft.fillRect(x - 15, y + 2, 7, 4, 0xFE60);  // Arme
  tft.fillRect(x + 8, y + 2, 12, 4, 0xFE60);
  tft.fillRect(x + 18, y - 5, 4, 15, TFT_YELLOW);  // Stift
  tft.fillTriangle(x + 18, y + 10, x + 22, y + 10, x + 20, y + 16, TFT_DARKGREY);
  tft.fillRect(x - 7, y + 18, 6, 10, TFT_BLUE);  // Beine
  tft.fillRect(x + 1, y + 18, 6, 10, TFT_BLUE);
}

void ThemeSelectScreen::drawHorseIcon(int x, int y) {
  uint16_t brown = 0x8200;
  // Groesseres Pferd
  tft.fillEllipse(x, y + 8, 22, 12, brown);  // Koerper
  tft.fillTriangle(x + 12, y, x + 24, y - 22, x + 14, y + 8, brown);  // Hals
  tft.fillEllipse(x + 26, y - 24, 9, 6, brown);  // Kopf
  tft.fillTriangle(x + 22, y - 34, x + 25, y - 28, x + 19, y - 28, brown);  // Ohren
  tft.fillTriangle(x + 30, y - 34, x + 33, y - 28, x + 27, y - 28, brown);
  tft.fillRect(x - 14, y + 14, 4, 18, brown);  // Beine
  tft.fillRect(x - 5, y + 14, 4, 18, brown);
  tft.fillRect(x + 5, y + 14, 4, 18, brown);
  tft.fillRect(x + 14, y + 14, 4, 18, brown);
  tft.fillTriangle(x - 22, y + 2, x - 32, y + 14, x - 18, y + 12, 0x4100);  // Schweif
}

int ThemeSelectScreen::handleTouch(int16_t x, int16_t y) {
  if (backButton->contains(x, y)) {
    return 0;  // Zurueck
  }
  if (fussballButton->contains(x, y)) {
    return 1;  // Fussball
  }
  if (kitaButton->contains(x, y)) {
    return 2;  // KiTa
  }
  if (reitenButton->contains(x, y)) {
    return 3;  // Reiten
  }
  return -1;
}
