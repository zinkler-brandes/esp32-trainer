#include "SplashScreen.h"

// Regenbogenfarben (RGB565)
#define RAINBOW_RED     0xF800  // Rot
#define RAINBOW_ORANGE  0xFD20  // Orange
#define RAINBOW_YELLOW  0xFFE0  // Gelb
#define RAINBOW_GREEN   0x07E0  // Grün
#define RAINBOW_CYAN    0x07FF  // Cyan
#define RAINBOW_BLUE    0x001F  // Blau
#define RAINBOW_PURPLE  0x781F  // Lila
#define RAINBOW_PINK    0xF81F  // Pink

// Buchstaben-Bitmaps (5x7) für TIKITAKA
const uint8_t char_T[] = { 0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100 };
const uint8_t char_I[] = { 0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b11111 };
const uint8_t char_K[] = { 0b10001, 0b10010, 0b10100, 0b11000, 0b10100, 0b10010, 0b10001 };
const uint8_t char_A[] = { 0b01110, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001 };

const uint8_t* chars[] = { char_T, char_I, char_K, char_I, char_T, char_A, char_K, char_A };

uint16_t colors[] = {
  RAINBOW_RED, RAINBOW_ORANGE, RAINBOW_YELLOW, RAINBOW_GREEN,
  RAINBOW_CYAN, RAINBOW_BLUE, RAINBOW_PURPLE, RAINBOW_PINK
};

// Layout-Konstanten
const int scaleX = 6;
const int scaleY = 8;
const int charWidth = 5 * scaleX;
const int charHeight = 7 * scaleY;
const int spacing = 4;
const int totalWidth = 8 * charWidth + 7 * spacing;
const int startX = (320 - totalWidth) / 2;
const int startY = (240 - charHeight) / 2 - 20;

SplashScreen::SplashScreen() {
}

void SplashScreen::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void SplashScreen::show() {
  // Schwarzer Hintergrund
  tft.fillScreen(TFT_BLACK);

  // Bunte Sterne im Hintergrund
  drawStars();

  // Regenbogen-Linien oben und unten
  drawRainbowBars();

  // Schatten für alle Buchstaben
  for (int i = 0; i < 8; i++) {
    int x = startX + i * (charWidth + spacing);
    drawPixelChar(x + 3, startY + 4, chars[i], 0x2104, scaleX, scaleY);
  }

  // Bunte Buchstaben
  for (int i = 0; i < 8; i++) {
    int x = startX + i * (charWidth + spacing);
    drawPixelChar(x, startY, chars[i], colors[i], scaleX, scaleY);
  }

  // Leuchtende Outline um jeden Buchstaben
  drawGlow();

  // Slogan
  drawSlogan();

  // Sparkles um den Text
  drawSparkles();

  // Kurz stehen lassen
  delay(3000);

  // Schwarz machen für sauberen Übergang
  tft.fillScreen(TFT_BLACK);
}

void SplashScreen::drawStars() {
  // Kleine bunte Sterne/Punkte verstreut
  randomSeed(42);  // Fester Seed für konsistentes Muster

  for (int i = 0; i < 30; i++) {
    int x = random(10, 310);
    int y = random(10, 230);

    // Nicht über dem Text-Bereich
    if (y > startY - 15 && y < startY + charHeight + 50) continue;

    uint16_t starColors[] = {RAINBOW_RED, RAINBOW_ORANGE, RAINBOW_YELLOW, RAINBOW_GREEN, RAINBOW_CYAN};
    uint16_t color = starColors[random(5)];

    int size = random(1, 4);
    if (size == 1) {
      tft.drawPixel(x, y, color);
    } else if (size == 2) {
      // Kleines Kreuz
      tft.drawPixel(x, y, color);
      tft.drawPixel(x-1, y, color);
      tft.drawPixel(x+1, y, color);
      tft.drawPixel(x, y-1, color);
      tft.drawPixel(x, y+1, color);
    } else {
      // 4-zackiger Stern
      tft.drawPixel(x, y, TFT_WHITE);
      tft.drawPixel(x-1, y, color);
      tft.drawPixel(x+1, y, color);
      tft.drawPixel(x, y-1, color);
      tft.drawPixel(x, y+1, color);
      tft.drawPixel(x-2, y, color);
      tft.drawPixel(x+2, y, color);
      tft.drawPixel(x, y-2, color);
      tft.drawPixel(x, y+2, color);
    }
  }
}

void SplashScreen::drawRainbowBars() {
  // Regenbogen-Balken oben
  uint16_t rainbowColors[] = {
    0xF800, 0xFBE0, 0xFFE0, 0x07E0, 0x07FF, 0x001F, 0x781F
  };

  int barY = 8;
  for (int i = 0; i < 7; i++) {
    tft.fillRect(0, barY + i * 3, 320, 2, rainbowColors[i]);
  }

  // Regenbogen-Balken unten
  barY = 210;
  for (int i = 0; i < 7; i++) {
    tft.fillRect(0, barY + i * 3, 320, 2, rainbowColors[6-i]);
  }
}

void SplashScreen::drawGlow() {
  // Leichter Glow/Outline um die Buchstaben in ihrer Farbe (heller)
  for (int i = 0; i < 8; i++) {
    int x = startX + i * (charWidth + spacing);

    // Helle Version der Farbe (mit Weiß gemischt)
    uint16_t glowColor = colors[i] | 0x8410;  // Aufhellen

    // Outline: 1 Pixel außen herum (vereinfacht - nur an den Rändern)
    for (int row = 0; row < 7; row++) {
      uint8_t rowData = chars[i][row];
      for (int col = 0; col < 5; col++) {
        if (rowData & (0x10 >> col)) {
          int px = x + col * scaleX;
          int py = startY + row * scaleY;
          // Glow-Pixel links und rechts
          if (col == 0 || !(rowData & (0x10 >> (col-1)))) {
            tft.drawFastVLine(px - 1, py, scaleY, glowColor);
          }
          if (col == 4 || !(rowData & (0x10 >> (col+1)))) {
            tft.drawFastVLine(px + scaleX, py, scaleY, glowColor);
          }
        }
      }
    }
  }
}

void SplashScreen::drawSlogan() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextFont(1);
  tft.setCursor(76, 175);
  tft.print("Spiel dich schlau.");
}

void SplashScreen::drawSparkles() {
  // Kleine Sparkles neben dem Titel
  uint16_t sparkle = TFT_WHITE;

  // Links vom K
  int sx = startX - 20;
  int sy = startY + charHeight / 2;
  tft.drawPixel(sx, sy, sparkle);
  tft.drawPixel(sx-2, sy, RAINBOW_YELLOW);
  tft.drawPixel(sx+2, sy, RAINBOW_YELLOW);
  tft.drawPixel(sx, sy-2, RAINBOW_YELLOW);
  tft.drawPixel(sx, sy+2, RAINBOW_YELLOW);

  // Rechts vom letzten L
  sx = startX + totalWidth + 15;
  tft.drawPixel(sx, sy, sparkle);
  tft.drawPixel(sx-2, sy, RAINBOW_CYAN);
  tft.drawPixel(sx+2, sy, RAINBOW_CYAN);
  tft.drawPixel(sx, sy-2, RAINBOW_CYAN);
  tft.drawPixel(sx, sy+2, RAINBOW_CYAN);

  // Oben mittig
  sx = 160;
  sy = startY - 12;
  tft.drawPixel(sx, sy, sparkle);
  tft.drawPixel(sx-2, sy, RAINBOW_PINK);
  tft.drawPixel(sx+2, sy, RAINBOW_PINK);
  tft.drawPixel(sx, sy-2, RAINBOW_PINK);
  tft.drawPixel(sx, sy+2, RAINBOW_PINK);
}

void SplashScreen::drawPixelChar(int x, int y, const uint8_t* bitmap, uint16_t color, int scaleX, int scaleY) {
  for (int row = 0; row < 7; row++) {
    uint8_t rowData = bitmap[row];
    for (int col = 0; col < 5; col++) {
      if (rowData & (0x10 >> col)) {
        tft.fillRect(x + col * scaleX, y + row * scaleY, scaleX, scaleY, color);
      }
    }
  }
}

void SplashScreen::drawTitle() {}
void SplashScreen::drawCopperBar() {}
void SplashScreen::drawGrassBackground() {}
void SplashScreen::drawFieldLines() {}
void SplashScreen::drawFootball(int cx, int cy, int r) {}
void SplashScreen::drawLoadingDots(int frame) {}
