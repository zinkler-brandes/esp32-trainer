#ifndef SRC_SPLASHSCREEN_H
#define SRC_SPLASHSCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>

// Rasen-Grün Farben
#define GRASS_DARK    0x1C62   // Dunkles Grün
#define GRASS_LIGHT   0x2DC4   // Helles Grün
#define GRASS_LINE    0xFFFF   // Weiße Linien

class SplashScreen {
  public:
    SplashScreen();
    void init();
    void show();

  private:
    TFT_eSPI tft;
    void drawGrassBackground();
    void drawFieldLines();
    void drawFootball(int x, int y, int size);
    void drawTitle();
    void drawSlogan();
    void drawLoadingDots(int frame);
    void drawPixelChar(int x, int y, const uint8_t* bitmap, uint16_t color, int scaleX, int scaleY);
    void drawCopperBar();
    void drawStars();
    void drawRainbowBars();
    void drawGlow();
    void drawSparkles();
};

#endif
