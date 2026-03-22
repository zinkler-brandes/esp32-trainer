#ifndef SRC_FLAGDRAWER_H
#define SRC_FLAGDRAWER_H

#include <Arduino.h>
#include <TFT_eSPI.h>

// Flaggen-Typen (Grundmuster)
enum FlagType {
  FLAG_TRICOLOR_H,      // 3 horizontale Streifen (Deutschland, Niederlande)
  FLAG_TRICOLOR_V,      // 3 vertikale Streifen (Frankreich, Italien)
  FLAG_BICOLOR_H,       // 2 horizontale Streifen (Ukraine, Polen)
  FLAG_BICOLOR_V,       // 2 vertikale Streifen
  FLAG_NORDIC_CROSS,    // Nordisches Kreuz (Norwegen, Daenemark)
  FLAG_CROSS,           // Einfaches Kreuz (Schweiz, England)
  FLAG_DIAGONAL,        // Diagonale (Kongo)
  FLAG_STRIPES_H,       // Viele horizontale Streifen (USA, Uruguay)
  FLAG_STAR_CENTER,     // Stern in der Mitte (Marokko)
  FLAG_CIRCLE_CENTER,   // Kreis in der Mitte (Japan, Bangladesch)
  FLAG_CRESCENT,        // Halbmond (Tuerkei, Tunesien)
  FLAG_CHEVRON,         // Dreieck links (Tschechien, Suedafrika)
  FLAG_CUSTOM           // Spezielle Flagge (Brasilien, etc.)
};

// Flaggen-Daten fuer ein Land
struct FlagData {
  FlagType type;
  uint16_t color1;      // Hauptfarbe / oben / links
  uint16_t color2;      // Mitte / rechts
  uint16_t color3;      // unten / Akzent
  uint16_t accent;      // Kreuz/Stern/Kreis Farbe
  uint8_t customId;     // Fuer FLAG_CUSTOM: welche Sonderbehandlung
};

// Custom IDs fuer spezielle Flaggen
#define CUSTOM_BRAZIL     1
#define CUSTOM_USA        2
#define CUSTOM_UK         3
#define CUSTOM_ARGENTINA  4
#define CUSTOM_AUSTRALIA  5
#define CUSTOM_SOUTH_AFRICA 6
#define CUSTOM_PANAMA     7
#define CUSTOM_SOUTH_KOREA 8
#define CUSTOM_MEXICO     9
#define CUSTOM_CANADA     10
#define CUSTOM_JAMAICA    11
#define CUSTOM_SAUDI      12
#define CUSTOM_QATAR      13
#define CUSTOM_CROATIA    14
#define CUSTOM_SERBIA     15
#define CUSTOM_SCOTLAND   16
#define CUSTOM_SWITZERLAND 17
#define CUSTOM_PORTUGAL    18
#define CUSTOM_SPAIN       19

class FlagDrawer {
  public:
    // Zeichnet eine Flagge an Position (x, y) mit Groesse (w, h)
    static void drawFlag(TFT_eSPI* tft, int x, int y, int w, int h, const FlagData& flag);

    // Zeichnet Flagge fuer WM-Team (nach Index)
    static void drawWMTeamFlag(TFT_eSPI* tft, int x, int y, int w, int h, int teamIndex);

  private:
    static void drawTricolorH(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t c1, uint16_t c2, uint16_t c3);
    static void drawTricolorV(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t c1, uint16_t c2, uint16_t c3);
    static void drawBicolorH(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t c1, uint16_t c2);
    static void drawNordicCross(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t bg, uint16_t cross, uint16_t inner);
    static void drawCross(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t bg, uint16_t cross);
    static void drawCircleCenter(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t bg, uint16_t circle);
    static void drawStarCenter(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t bg, uint16_t star);
    static void drawCrescent(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t bg, uint16_t crescent);
    static void drawChevron(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t c1, uint16_t c2, uint16_t chevron);
    static void drawStripesH(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t c1, uint16_t c2, int count);

    // Custom Flaggen
    static void drawBrazil(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawUSA(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawArgentina(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawAustralia(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawSouthAfrica(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawPanama(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawSouthKorea(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawMexico(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawCanada(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawJamaica(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawSaudi(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawQatar(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawCroatia(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawScotland(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawSwitzerland(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawPortugal(TFT_eSPI* tft, int x, int y, int w, int h);
    static void drawSpain(TFT_eSPI* tft, int x, int y, int w, int h);

    // Hilfsfunktionen
    static void drawStar5(TFT_eSPI* tft, int cx, int cy, int r, uint16_t color);
};

#endif
