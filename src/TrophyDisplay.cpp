#include "TrophyDisplay.h"

TrophyDisplay::TrophyDisplay() {
  menuButton = new Button(60, 200, 200, 35, "WEITER");
  menuButton->setColors(TFT_DARKGREY, TFT_WHITE);
}

void TrophyDisplay::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void TrophyDisplay::showTrophy(TournamentType type) {
  tft.fillScreen(TFT_BLACK);

  // Konfetti Effekt
  drawConfetti();

  // Titel
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(20, 10);
  tft.print("POKALSIEGER!");

  // Pokal zeichnen
  if (type == TOURNAMENT_DFB_POKAL) {
    drawDFBPokal();
    tft.setTextSize(2);
    tft.setTextColor(GOLD_COLOR, TFT_BLACK);
    tft.setCursor(80, 170);
    tft.print("DFB-POKAL 2025");
  } else {
    drawHenkelpott();
    tft.setTextSize(2);
    tft.setTextColor(SILVER_COLOR, TFT_BLACK);
    tft.setCursor(50, 170);
    tft.print("CHAMPIONS LEAGUE");
  }

  // Button
  menuButton->draw(&tft);
}

int TrophyDisplay::handleTouch(int16_t x, int16_t y) {
  if (menuButton->contains(x, y)) {
    return 0;
  }
  return -1;
}

void TrophyDisplay::drawDFBPokal() {
  // DFB-Pokal Pixel-Art (zentriert bei x=160)
  int centerX = 160;
  int startY = 50;
  int pixelSize = 6;

  // Pokal-Form (von oben nach unten)
  // Breiter Rand oben
  for (int x = -4; x <= 4; x++) {
    tft.fillRect(centerX + x * pixelSize, startY, pixelSize, pixelSize, GOLD_COLOR);
  }

  // Schale (breiter werdend)
  for (int y = 1; y <= 3; y++) {
    int width = 4 + y;
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? GOLD_DARK : GOLD_COLOR;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Schale nach innen (schmaler werdend)
  for (int y = 4; y <= 6; y++) {
    int width = 7 - (y - 4);
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? GOLD_DARK : GOLD_COLOR;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Stiel
  for (int y = 7; y <= 9; y++) {
    for (int x = -1; x <= 1; x++) {
      uint16_t color = (x == -1 || x == 1) ? GOLD_DARK : GOLD_COLOR;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Fuß (breiter)
  for (int y = 10; y <= 11; y++) {
    int width = (y == 10) ? 2 : 3;
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? GOLD_DARK : GOLD_COLOR;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Glanz-Effekt
  tft.fillRect(centerX - 3 * pixelSize + 2, startY + 2 * pixelSize + 2, pixelSize - 2, pixelSize * 2, TFT_WHITE);
}

void TrophyDisplay::drawHenkelpott() {
  // Champions League Henkelpott (zentriert bei x=160)
  int centerX = 160;
  int startY = 45;
  int pixelSize = 5;

  // Henkel links und rechts (oben)
  for (int y = 0; y <= 3; y++) {
    // Linker Henkel
    tft.fillRect(centerX - 8 * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, SILVER_COLOR);
    // Rechter Henkel
    tft.fillRect(centerX + 8 * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, SILVER_COLOR);
  }

  // Henkel-Bogen
  tft.fillRect(centerX - 8 * pixelSize, startY + 3 * pixelSize, pixelSize * 2, pixelSize, SILVER_COLOR);
  tft.fillRect(centerX + 7 * pixelSize, startY + 3 * pixelSize, pixelSize * 2, pixelSize, SILVER_COLOR);

  // Oberer Rand der Schale
  for (int x = -6; x <= 6; x++) {
    tft.fillRect(centerX + x * pixelSize, startY, pixelSize, pixelSize, SILVER_COLOR);
  }

  // Schale (breiter werdend)
  for (int y = 1; y <= 4; y++) {
    int width = 6 + (y > 2 ? 1 : 0);
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? SILVER_DARK : SILVER_COLOR;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Schale verjüngt sich
  for (int y = 5; y <= 8; y++) {
    int width = 7 - (y - 4);
    if (width < 2) width = 2;
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? SILVER_DARK : SILVER_COLOR;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Stiel
  for (int y = 9; y <= 11; y++) {
    for (int x = -1; x <= 1; x++) {
      uint16_t color = (x == -1 || x == 1) ? SILVER_DARK : SILVER_COLOR;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Fuß
  for (int y = 12; y <= 14; y++) {
    int width = (y == 12) ? 2 : (y == 13) ? 3 : 4;
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? SILVER_DARK : SILVER_COLOR;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Glanz-Effekt
  tft.fillRect(centerX - 4 * pixelSize + 2, startY + 2 * pixelSize + 2, pixelSize - 1, pixelSize * 2, TFT_WHITE);
}

void TrophyDisplay::drawConfetti() {
  // Zufälliges Konfetti
  randomSeed(millis());
  for (int i = 0; i < 50; i++) {
    int x = random(0, 320);
    int y = random(0, 240);

    // Zufällige Farbe
    uint16_t colors[] = {TFT_RED, TFT_YELLOW, TFT_GREEN, TFT_BLUE, TFT_MAGENTA, TFT_CYAN, TFT_WHITE};
    uint16_t color = colors[random(0, 7)];

    // Zufällige Form (Rechteck oder Kreis)
    if (random(0, 2) == 0) {
      tft.fillRect(x, y, random(2, 6), random(2, 6), color);
    } else {
      tft.fillCircle(x, y, random(1, 4), color);
    }
  }
}
