#include "FlagDrawer.h"
#include "WM2026Teams.h"

// Farben fuer Flaggen (RGB565)
#define FLAG_RED      0xF800
#define FLAG_BLUE     0x001F
#define FLAG_DARKBLUE 0x000F
#define FLAG_GREEN    0x07E0
#define FLAG_DARKGREEN 0x03E0
#define FLAG_YELLOW   0xFFE0
#define FLAG_ORANGE   0xFD20
#define FLAG_WHITE    0xFFFF
#define FLAG_BLACK    0x0000
#define FLAG_GOLD     0xFE60
#define FLAG_SKYBLUE  0x867D
#define FLAG_MAROON   0x8000

// Flaggen-Daten fuer alle 48 WM-Teams (Index muss mit WM_TEAMS uebereinstimmen)
const FlagData WM_FLAGS[] = {
  // 0: Deutschland - Schwarz-Rot-Gold horizontal
  {FLAG_TRICOLOR_H, FLAG_BLACK, FLAG_RED, FLAG_GOLD, 0, 0},
  // 1: Spanien - Rot-Gelb(breit)-Rot (1:2:1 Verhaeltnis)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_SPAIN},
  // 2: England - Weiss mit rotem Kreuz
  {FLAG_CROSS, FLAG_WHITE, FLAG_RED, 0, FLAG_RED, 0},
  // 3: Frankreich - Blau-Weiss-Rot vertikal
  {FLAG_TRICOLOR_V, FLAG_BLUE, FLAG_WHITE, FLAG_RED, 0, 0},
  // 4: Argentinien - Custom (Hellblau-Weiss-Hellblau mit Sonne)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_ARGENTINA},
  // 5: Brasilien - Custom (Gruen mit gelber Raute)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_BRAZIL},
  // 6: Portugal - Gruen-Rot vertikal mit Wappen
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_PORTUGAL},
  // 7: Niederlande - Rot-Weiss-Blau horizontal
  {FLAG_TRICOLOR_H, FLAG_RED, FLAG_WHITE, FLAG_DARKBLUE, 0, 0},
  // 8: Belgien - Schwarz-Gelb-Rot vertikal
  {FLAG_TRICOLOR_V, FLAG_BLACK, FLAG_YELLOW, FLAG_RED, 0, 0},
  // 9: Kroatien - Custom (Rot-Weiss-Blau mit Schachbrett)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_CROATIA},
  // 10: Uruguay - Custom (Weiss-Blau Streifen mit Sonne)
  {FLAG_STRIPES_H, FLAG_WHITE, FLAG_BLUE, 0, FLAG_GOLD, 9},
  // 11: Kolumbien - Gelb-Blau-Rot horizontal (Gelb doppelt)
  {FLAG_TRICOLOR_H, FLAG_YELLOW, FLAG_DARKBLUE, FLAG_RED, 0, 0},
  // 12: Mexiko - Custom (Gruen-Weiss-Rot mit Adler)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_MEXICO},
  // 13: USA - Custom (Sterne und Streifen)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_USA},
  // 14: Schweiz - Rot mit weissem Kreuz (kompakt in der Mitte)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_SWITZERLAND},
  // 15: Japan - Weiss mit rotem Kreis
  {FLAG_CIRCLE_CENTER, FLAG_WHITE, FLAG_RED, 0, FLAG_RED, 0},
  // 16: Marokko - Rot mit gruenem Stern
  {FLAG_STAR_CENTER, FLAG_RED, FLAG_DARKGREEN, 0, FLAG_DARKGREEN, 0},
  // 17: Senegal - Gruen-Gelb-Rot vertikal mit Stern
  {FLAG_TRICOLOR_V, FLAG_DARKGREEN, FLAG_YELLOW, FLAG_RED, FLAG_DARKGREEN, 0},
  // 18: Ecuador - Gelb-Blau-Rot horizontal (Gelb doppelt)
  {FLAG_TRICOLOR_H, FLAG_YELLOW, FLAG_DARKBLUE, FLAG_RED, 0, 0},
  // 19: Oesterreich - Rot-Weiss-Rot horizontal
  {FLAG_TRICOLOR_H, FLAG_RED, FLAG_WHITE, FLAG_RED, 0, 0},
  // 20: Norwegen - Nordisches Kreuz
  {FLAG_NORDIC_CROSS, FLAG_RED, FLAG_DARKBLUE, FLAG_WHITE, 0, 0},
  // 21: Italien - Gruen-Weiss-Rot vertikal
  {FLAG_TRICOLOR_V, FLAG_DARKGREEN, FLAG_WHITE, FLAG_RED, 0, 0},
  // 22: Ukraine - Blau-Gelb horizontal
  {FLAG_BICOLOR_H, FLAG_BLUE, FLAG_YELLOW, 0, 0, 0},
  // 23: Tuerkei - Rot mit Halbmond
  {FLAG_CRESCENT, FLAG_RED, FLAG_WHITE, 0, FLAG_WHITE, 0},
  // 24: Kanada - Custom (Rot-Weiss-Rot mit Ahornblatt)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_CANADA},
  // 25: Katar - Custom (Maroon-Weiss gezackt)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_QATAR},
  // 26: Algerien - Gruen-Weiss vertikal mit Halbmond
  {FLAG_BICOLOR_V, FLAG_DARKGREEN, FLAG_WHITE, 0, FLAG_RED, 0},
  // 27: Aegypten - Rot-Weiss-Schwarz horizontal
  {FLAG_TRICOLOR_H, FLAG_RED, FLAG_WHITE, FLAG_BLACK, FLAG_GOLD, 0},
  // 28: Iran - Gruen-Weiss-Rot horizontal
  {FLAG_TRICOLOR_H, FLAG_DARKGREEN, FLAG_WHITE, FLAG_RED, 0, 0},
  // 29: Saudi-Arabien - Custom (Gruen mit Schwert)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_SAUDI},
  // 30: Tunesien - Rot mit Halbmond
  {FLAG_CRESCENT, FLAG_RED, FLAG_WHITE, 0, FLAG_WHITE, 0},
  // 31: Ghana - Rot-Gelb-Gruen mit Stern
  {FLAG_TRICOLOR_H, FLAG_RED, FLAG_GOLD, FLAG_DARKGREEN, FLAG_BLACK, 0},
  // 32: Elfenbeinkueste - Orange-Weiss-Gruen vertikal
  {FLAG_TRICOLOR_V, FLAG_ORANGE, FLAG_WHITE, FLAG_DARKGREEN, 0, 0},
  // 33: Australien - Custom (Blau mit Union Jack und Sternen)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_AUSTRALIA},
  // 34: Schottland - Blau mit weissem Diagonalkreuz (St. Andrew's Cross)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_SCOTLAND},
  // 35: Suedkorea - Custom (Weiss mit Yin-Yang)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_SOUTH_KOREA},
  // 36: Haiti - Blau-Rot horizontal
  {FLAG_BICOLOR_H, FLAG_DARKBLUE, FLAG_RED, 0, 0, 0},
  // 37: Panama - Custom (Weiss mit Sternen)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_PANAMA},
  // 38: Cabo Verde - Blau mit Streifen
  {FLAG_STRIPES_H, FLAG_DARKBLUE, FLAG_WHITE, FLAG_RED, 0, 0},
  // 39: Neuseeland - Blau (vereinfacht wie Australien)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_AUSTRALIA},
  // 40: Curacao - Blau mit Streifen
  {FLAG_BICOLOR_H, FLAG_DARKBLUE, FLAG_YELLOW, 0, 0, 0},
  // 41: Suedafrika - Custom (Y-Form)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_SOUTH_AFRICA},
  // 42: Paraguay - Rot-Weiss-Blau horizontal
  {FLAG_TRICOLOR_H, FLAG_RED, FLAG_WHITE, FLAG_DARKBLUE, 0, 0},
  // 43: Bolivien - Rot-Gelb-Gruen horizontal
  {FLAG_TRICOLOR_H, FLAG_RED, FLAG_YELLOW, FLAG_DARKGREEN, 0, 0},
  // 44: Jamaika - Custom (Schwarz-Gruen-Gold diagonal)
  {FLAG_CUSTOM, 0, 0, 0, 0, CUSTOM_JAMAICA},
  // 45: Jordanien - Schwarz-Weiss-Gruen mit rotem Dreieck
  {FLAG_CHEVRON, FLAG_BLACK, FLAG_DARKGREEN, FLAG_RED, FLAG_WHITE, 0},
  // 46: Usbekistan - Blau-Weiss-Gruen mit Streifen
  {FLAG_TRICOLOR_H, FLAG_BLUE, FLAG_WHITE, FLAG_DARKGREEN, 0, 0},
  // 47: Daenemark - Rot mit weissem Kreuz
  {FLAG_NORDIC_CROSS, FLAG_RED, FLAG_WHITE, FLAG_WHITE, 0, 0}
};

void FlagDrawer::drawFlag(TFT_eSPI* tft, int x, int y, int w, int h, const FlagData& flag) {
  switch (flag.type) {
    case FLAG_TRICOLOR_H:
      drawTricolorH(tft, x, y, w, h, flag.color1, flag.color2, flag.color3);
      break;
    case FLAG_TRICOLOR_V:
      drawTricolorV(tft, x, y, w, h, flag.color1, flag.color2, flag.color3);
      break;
    case FLAG_BICOLOR_H:
      drawBicolorH(tft, x, y, w, h, flag.color1, flag.color2);
      break;
    case FLAG_BICOLOR_V:
      tft->fillRect(x, y, w / 2, h, flag.color1);
      tft->fillRect(x + w / 2, y, w / 2, h, flag.color2);
      break;
    case FLAG_NORDIC_CROSS:
      drawNordicCross(tft, x, y, w, h, flag.color1, flag.color2, flag.color3);
      break;
    case FLAG_CROSS:
      drawCross(tft, x, y, w, h, flag.color1, flag.color2);
      break;
    case FLAG_CIRCLE_CENTER:
      drawCircleCenter(tft, x, y, w, h, flag.color1, flag.color2);
      break;
    case FLAG_STAR_CENTER:
      drawStarCenter(tft, x, y, w, h, flag.color1, flag.color2);
      break;
    case FLAG_CRESCENT:
      drawCrescent(tft, x, y, w, h, flag.color1, flag.color2);
      break;
    case FLAG_CHEVRON:
      drawChevron(tft, x, y, w, h, flag.color1, flag.color3, flag.color2);
      break;
    case FLAG_STRIPES_H:
      drawStripesH(tft, x, y, w, h, flag.color1, flag.color2, 5);
      break;
    case FLAG_CUSTOM:
      // Custom-Flaggen
      switch (flag.customId) {
        case CUSTOM_BRAZIL:
          drawBrazil(tft, x, y, w, h);
          break;
        case CUSTOM_USA:
          drawUSA(tft, x, y, w, h);
          break;
        case CUSTOM_ARGENTINA:
          drawArgentina(tft, x, y, w, h);
          break;
        case CUSTOM_AUSTRALIA:
          drawAustralia(tft, x, y, w, h);
          break;
        case CUSTOM_SOUTH_AFRICA:
          drawSouthAfrica(tft, x, y, w, h);
          break;
        case CUSTOM_PANAMA:
          drawPanama(tft, x, y, w, h);
          break;
        case CUSTOM_SOUTH_KOREA:
          drawSouthKorea(tft, x, y, w, h);
          break;
        case CUSTOM_MEXICO:
          drawMexico(tft, x, y, w, h);
          break;
        case CUSTOM_CANADA:
          drawCanada(tft, x, y, w, h);
          break;
        case CUSTOM_JAMAICA:
          drawJamaica(tft, x, y, w, h);
          break;
        case CUSTOM_SAUDI:
          drawSaudi(tft, x, y, w, h);
          break;
        case CUSTOM_QATAR:
          drawQatar(tft, x, y, w, h);
          break;
        case CUSTOM_CROATIA:
          drawCroatia(tft, x, y, w, h);
          break;
        case CUSTOM_SCOTLAND:
          drawScotland(tft, x, y, w, h);
          break;
        case CUSTOM_SWITZERLAND:
          drawSwitzerland(tft, x, y, w, h);
          break;
        case CUSTOM_PORTUGAL:
          drawPortugal(tft, x, y, w, h);
          break;
        case CUSTOM_SPAIN:
          drawSpain(tft, x, y, w, h);
          break;
        default:
          // Fallback: Einfach grau
          tft->fillRect(x, y, w, h, TFT_DARKGREY);
          break;
      }
      break;
    default:
      tft->fillRect(x, y, w, h, TFT_DARKGREY);
      break;
  }

  // Rahmen zeichnen
  tft->drawRect(x, y, w, h, TFT_DARKGREY);
}

void FlagDrawer::drawWMTeamFlag(TFT_eSPI* tft, int x, int y, int w, int h, int teamIndex) {
  if (teamIndex >= 0 && teamIndex < WM_TEAM_COUNT) {
    drawFlag(tft, x, y, w, h, WM_FLAGS[teamIndex]);
  } else {
    tft->fillRect(x, y, w, h, TFT_DARKGREY);
    tft->drawRect(x, y, w, h, TFT_WHITE);
  }
}

// === Grundmuster ===

void FlagDrawer::drawTricolorH(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t c1, uint16_t c2, uint16_t c3) {
  int stripeH = h / 3;
  tft->fillRect(x, y, w, stripeH, c1);
  tft->fillRect(x, y + stripeH, w, stripeH, c2);
  tft->fillRect(x, y + 2 * stripeH, w, h - 2 * stripeH, c3);
}

void FlagDrawer::drawTricolorV(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t c1, uint16_t c2, uint16_t c3) {
  int stripeW = w / 3;
  tft->fillRect(x, y, stripeW, h, c1);
  tft->fillRect(x + stripeW, y, stripeW, h, c2);
  tft->fillRect(x + 2 * stripeW, y, w - 2 * stripeW, h, c3);
}

void FlagDrawer::drawBicolorH(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t c1, uint16_t c2) {
  int halfH = h / 2;
  tft->fillRect(x, y, w, halfH, c1);
  tft->fillRect(x, y + halfH, w, h - halfH, c2);
}

void FlagDrawer::drawNordicCross(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t bg, uint16_t cross, uint16_t inner) {
  // Hintergrund
  tft->fillRect(x, y, w, h, bg);

  // Kreuz (versetzt nach links wie bei nordischen Flaggen)
  int crossX = x + w * 3 / 8;
  int crossW = w / 6;
  int crossH = h / 4;
  int crossY = y + h / 2 - crossH / 2;

  // Aeusseres Kreuz
  tft->fillRect(crossX, y, crossW, h, cross);
  tft->fillRect(x, crossY, w, crossH, cross);

  // Inneres Kreuz (wenn unterschiedlich, z.B. Norwegen)
  if (inner != cross) {
    int innerW = crossW / 2;
    int innerH = crossH / 2;
    tft->fillRect(crossX + crossW / 4, y, innerW, h, inner);
    tft->fillRect(x, crossY + crossH / 4, w, innerH, inner);
  }
}

void FlagDrawer::drawCross(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t bg, uint16_t cross) {
  // Hintergrund
  tft->fillRect(x, y, w, h, bg);

  // Zentriertes Kreuz
  int crossW = w / 5;
  int crossH = h / 3;
  int cx = x + w / 2;
  int cy = y + h / 2;

  tft->fillRect(cx - crossW / 2, y, crossW, h, cross);
  tft->fillRect(x, cy - crossH / 2, w, crossH, cross);
}

void FlagDrawer::drawCircleCenter(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t bg, uint16_t circle) {
  tft->fillRect(x, y, w, h, bg);
  int radius = min(w, h) / 3;
  tft->fillCircle(x + w / 2, y + h / 2, radius, circle);
}

void FlagDrawer::drawStarCenter(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t bg, uint16_t star) {
  tft->fillRect(x, y, w, h, bg);
  int radius = min(w, h) / 3;
  drawStar5(tft, x + w / 2, y + h / 2, radius, star);
}

void FlagDrawer::drawCrescent(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t bg, uint16_t crescent) {
  tft->fillRect(x, y, w, h, bg);

  int cx = x + w / 2;
  int cy = y + h / 2;
  int r1 = min(w, h) / 3;
  int r2 = r1 * 3 / 4;

  // Grosser Kreis
  tft->fillCircle(cx - 2, cy, r1, crescent);
  // Kleinerer Kreis schneidet raus (Halbmond-Effekt)
  tft->fillCircle(cx + 2, cy, r2, bg);

  // Stern neben dem Halbmond
  drawStar5(tft, cx + r1, cy, r1 / 2, crescent);
}

void FlagDrawer::drawChevron(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t c1, uint16_t c2, uint16_t chevron) {
  // Obere Haelfte
  tft->fillRect(x, y, w, h / 2, c1);
  // Untere Haelfte
  tft->fillRect(x, y + h / 2, w, h / 2, c2);
  // Dreieck links
  tft->fillTriangle(x, y, x + w / 3, y + h / 2, x, y + h, chevron);
}

void FlagDrawer::drawStripesH(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t c1, uint16_t c2, int count) {
  int stripeH = h / count;
  for (int i = 0; i < count; i++) {
    uint16_t color = (i % 2 == 0) ? c1 : c2;
    tft->fillRect(x, y + i * stripeH, w, stripeH, color);
  }
}

// === Custom Flaggen ===

void FlagDrawer::drawBrazil(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Gruen
  tft->fillRect(x, y, w, h, FLAG_GREEN);

  // Gelbe Raute
  int cx = x + w / 2;
  int cy = y + h / 2;
  tft->fillTriangle(x + 2, cy, cx, y + 2, cx, y + h - 2, FLAG_YELLOW);
  tft->fillTriangle(x + w - 2, cy, cx, y + 2, cx, y + h - 2, FLAG_YELLOW);

  // Blauer Kreis
  tft->fillCircle(cx, cy, h / 4, FLAG_DARKBLUE);
}

void FlagDrawer::drawUSA(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Rote und weisse Streifen (13 Streifen, vereinfacht auf 7)
  drawStripesH(tft, x, y, w, h, FLAG_RED, FLAG_WHITE, 7);

  // Blaues Feld oben links
  int blueW = w * 2 / 5;
  int blueH = h * 4 / 7;
  tft->fillRect(x, y, blueW, blueH, FLAG_DARKBLUE);

  // Ein paar Sterne (vereinfacht)
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      int sx = x + 4 + col * (blueW / 3);
      int sy = y + 3 + row * (blueH / 3);
      tft->fillCircle(sx, sy, 1, FLAG_WHITE);
    }
  }
}

void FlagDrawer::drawArgentina(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Hellblau-Weiss-Hellblau
  drawTricolorH(tft, x, y, w, h, FLAG_SKYBLUE, FLAG_WHITE, FLAG_SKYBLUE);

  // Sonne in der Mitte
  int cx = x + w / 2;
  int cy = y + h / 2;
  tft->fillCircle(cx, cy, h / 6, FLAG_GOLD);
}

void FlagDrawer::drawAustralia(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Blauer Hintergrund
  tft->fillRect(x, y, w, h, FLAG_DARKBLUE);

  // Union Jack (vereinfacht) oben links
  int ujW = w / 2;
  int ujH = h / 2;
  tft->fillRect(x + ujW / 2 - 1, y, 2, ujH, FLAG_WHITE);
  tft->fillRect(x, y + ujH / 2 - 1, ujW, 2, FLAG_WHITE);
  tft->fillRect(x + ujW / 2, y, 1, ujH, FLAG_RED);
  tft->fillRect(x, y + ujH / 2, ujW, 1, FLAG_RED);

  // Grosse Stern unten links
  tft->fillCircle(x + w / 4, y + h * 3 / 4, 3, FLAG_WHITE);

  // Suedkreuz rechts (vereinfacht als 4 Punkte)
  tft->fillCircle(x + w * 3 / 4, y + h / 4, 2, FLAG_WHITE);
  tft->fillCircle(x + w * 3 / 4, y + h * 3 / 4, 2, FLAG_WHITE);
  tft->fillCircle(x + w * 5 / 8, y + h / 2, 2, FLAG_WHITE);
  tft->fillCircle(x + w * 7 / 8, y + h / 2, 2, FLAG_WHITE);
}

void FlagDrawer::drawSouthAfrica(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Obere Haelfte rot
  tft->fillRect(x, y, w, h / 2, FLAG_RED);
  // Untere Haelfte blau
  tft->fillRect(x, y + h / 2, w, h / 2, FLAG_BLUE);

  // Weisse Trennlinie
  tft->fillRect(x, y + h / 2 - 1, w, 2, FLAG_WHITE);

  // Gruenes Y
  int yW = w / 3;
  tft->fillTriangle(x, y, x + yW, y + h / 2, x, y + h, FLAG_GREEN);
  tft->fillRect(x + yW - 2, y + h / 3, w - yW + 2, h / 3, FLAG_GREEN);

  // Schwarzer Kern im Y
  tft->fillTriangle(x, y + 3, x + yW - 4, y + h / 2, x, y + h - 3, FLAG_BLACK);
}

void FlagDrawer::drawPanama(TFT_eSPI* tft, int x, int y, int w, int h) {
  int halfW = w / 2;
  int halfH = h / 2;

  // 4 Quadranten
  tft->fillRect(x, y, halfW, halfH, FLAG_WHITE);  // oben links - weiss
  tft->fillRect(x + halfW, y, halfW, halfH, FLAG_RED);  // oben rechts - rot
  tft->fillRect(x, y + halfH, halfW, halfH, FLAG_BLUE);  // unten links - blau
  tft->fillRect(x + halfW, y + halfH, halfW, halfH, FLAG_WHITE);  // unten rechts - weiss

  // Sterne
  drawStar5(tft, x + halfW / 2, y + halfH / 2, 4, FLAG_BLUE);
  drawStar5(tft, x + halfW + halfW / 2, y + halfH + halfH / 2, 4, FLAG_RED);
}

void FlagDrawer::drawSouthKorea(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Weisser Hintergrund
  tft->fillRect(x, y, w, h, FLAG_WHITE);

  // Yin-Yang in der Mitte (vereinfacht)
  int cx = x + w / 2;
  int cy = y + h / 2;
  int r = h / 3;

  // Roter oberer Halbkreis
  tft->fillCircle(cx, cy, r, FLAG_RED);
  // Blauer unterer Teil
  tft->fillCircle(cx, cy + r / 2, r / 2, FLAG_BLUE);
  // Roter oberer kleiner Kreis
  tft->fillCircle(cx, cy - r / 2, r / 2, FLAG_RED);
  // Blau ueberdeckt unten
  tft->fillRect(cx - r, cy, r * 2, r, FLAG_BLUE);
  tft->fillCircle(cx, cy + r / 2, r / 2, FLAG_BLUE);
}

void FlagDrawer::drawMexico(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Gruen-Weiss-Rot
  drawTricolorV(tft, x, y, w, h, FLAG_DARKGREEN, FLAG_WHITE, FLAG_RED);

  // Adler (vereinfacht als brauner Kreis)
  int cx = x + w / 2;
  int cy = y + h / 2;
  tft->fillCircle(cx, cy, h / 5, 0x8200);  // Braun
}

void FlagDrawer::drawCanada(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Rot-Weiss-Rot
  int stripeW = w / 4;
  tft->fillRect(x, y, stripeW, h, FLAG_RED);
  tft->fillRect(x + stripeW, y, w - 2 * stripeW, h, FLAG_WHITE);
  tft->fillRect(x + w - stripeW, y, stripeW, h, FLAG_RED);

  // Ahornblatt (vereinfacht als roter Stern/Raute)
  int cx = x + w / 2;
  int cy = y + h / 2;
  tft->fillTriangle(cx, cy - h / 3, cx - h / 6, cy + h / 6, cx + h / 6, cy + h / 6, FLAG_RED);
  tft->fillRect(cx - 2, cy + h / 6, 4, h / 6, FLAG_RED);
}

void FlagDrawer::drawJamaica(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Gruener Hintergrund
  tft->fillRect(x, y, w, h, FLAG_DARKGREEN);

  // Schwarze Dreiecke oben und unten
  tft->fillTriangle(x, y, x + w, y, x + w / 2, y + h / 2, FLAG_BLACK);
  tft->fillTriangle(x, y + h, x + w, y + h, x + w / 2, y + h / 2, FLAG_BLACK);

  // Goldenes X
  tft->drawLine(x, y, x + w, y + h, FLAG_GOLD);
  tft->drawLine(x, y + h, x + w, y, FLAG_GOLD);
  tft->drawLine(x + 1, y, x + w, y + h - 1, FLAG_GOLD);
  tft->drawLine(x, y + h - 1, x + w - 1, y, FLAG_GOLD);
}

void FlagDrawer::drawSaudi(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Gruener Hintergrund
  tft->fillRect(x, y, w, h, FLAG_DARKGREEN);

  // Weisser Text (vereinfacht als Linie)
  tft->fillRect(x + w / 6, y + h / 3, w * 2 / 3, h / 6, FLAG_WHITE);

  // Schwert unten
  tft->fillRect(x + w / 4, y + h * 2 / 3, w / 2, 2, FLAG_WHITE);
}

void FlagDrawer::drawQatar(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Weisser Hintergrund
  tft->fillRect(x, y, w, h, FLAG_WHITE);

  // Maroon rechts mit Zacken
  int maroonX = x + w / 3;
  tft->fillRect(maroonX, y, w - maroonX + x, h, FLAG_MAROON);

  // Zacken (vereinfacht)
  for (int i = 0; i < 5; i++) {
    int ty = y + i * h / 5;
    tft->fillTriangle(maroonX, ty, maroonX, ty + h / 5, maroonX - 6, ty + h / 10, FLAG_WHITE);
  }
}

void FlagDrawer::drawCroatia(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Rot-Weiss-Blau horizontal
  drawTricolorH(tft, x, y, w, h, FLAG_RED, FLAG_WHITE, FLAG_DARKBLUE);

  // Schachbrett in der Mitte (vereinfacht)
  int cx = x + w / 2;
  int cy = y + h / 2;
  int size = h / 4;

  tft->fillRect(cx - size, cy - size / 2, size * 2, size, FLAG_RED);
  tft->fillRect(cx - size / 2, cy - size / 2, size / 2, size / 2, FLAG_WHITE);
  tft->fillRect(cx, cy, size / 2, size / 2, FLAG_WHITE);
}

void FlagDrawer::drawScotland(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Blauer Hintergrund
  tft->fillRect(x, y, w, h, FLAG_DARKBLUE);

  // Weisses Diagonalkreuz (St. Andrew's Cross / Saltire)
  // Dicke Linien von Ecke zu Ecke
  int thickness = max(2, h / 8);

  // Diagonale von oben-links nach unten-rechts
  for (int t = -thickness; t <= thickness; t++) {
    tft->drawLine(x, y + t, x + w - 1, y + h - 1 + t, FLAG_WHITE);
    tft->drawLine(x + t, y, x + w - 1 + t, y + h - 1, FLAG_WHITE);
  }

  // Diagonale von oben-rechts nach unten-links
  for (int t = -thickness; t <= thickness; t++) {
    tft->drawLine(x + w - 1, y + t, x, y + h - 1 + t, FLAG_WHITE);
    tft->drawLine(x + w - 1 + t, y, x + t, y + h - 1, FLAG_WHITE);
  }
}

void FlagDrawer::drawSwitzerland(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Roter Hintergrund
  tft->fillRect(x, y, w, h, FLAG_RED);

  // Weisses Kreuz in der Mitte (kompakt, nicht bis zur Kante)
  int cx = x + w / 2;
  int cy = y + h / 2;

  // Kreuzgroesse: ca. 60% der Flaggenhoehe
  int armLength = h * 3 / 10;  // Laenge eines Arms vom Zentrum
  int armWidth = h / 6;        // Breite des Arms

  // Vertikaler Balken
  tft->fillRect(cx - armWidth / 2, cy - armLength, armWidth, armLength * 2, FLAG_WHITE);

  // Horizontaler Balken
  tft->fillRect(cx - armLength, cy - armWidth / 2, armLength * 2, armWidth, FLAG_WHITE);
}

void FlagDrawer::drawSpain(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Spanische Flagge: Rot-Gelb(breit)-Rot im Verhaeltnis 1:2:1
  int redH = h / 4;
  int yellowH = h / 2;

  // Oberer roter Streifen
  tft->fillRect(x, y, w, redH, FLAG_RED);

  // Mittlerer gelber Streifen (doppelt so breit)
  tft->fillRect(x, y + redH, w, yellowH, FLAG_YELLOW);

  // Unterer roter Streifen
  tft->fillRect(x, y + redH + yellowH, w, h - redH - yellowH, FLAG_RED);
}

void FlagDrawer::drawPortugal(TFT_eSPI* tft, int x, int y, int w, int h) {
  // Gruen links (ca. 40%)
  int greenW = w * 2 / 5;
  tft->fillRect(x, y, greenW, h, FLAG_DARKGREEN);

  // Rot rechts (ca. 60%)
  tft->fillRect(x + greenW, y, w - greenW, h, FLAG_RED);

  // Wappen auf der Trennlinie (vereinfacht als gelber Kreis mit rotem Rand)
  int cx = x + greenW;
  int cy = y + h / 2;
  int r = h / 3;

  // Gelber Kreis (Armillarsphäre)
  tft->fillCircle(cx, cy, r, FLAG_GOLD);
  tft->drawCircle(cx, cy, r, FLAG_RED);

  // Roter Schild in der Mitte
  int shieldR = r * 2 / 3;
  tft->fillCircle(cx, cy, shieldR, FLAG_RED);
  tft->drawCircle(cx, cy, shieldR, FLAG_WHITE);

  // Weisse Punkte im Schild (vereinfacht)
  tft->fillCircle(cx, cy, 2, FLAG_WHITE);
}

// === Hilfsfunktionen ===

void FlagDrawer::drawStar5(TFT_eSPI* tft, int cx, int cy, int r, uint16_t color) {
  // Vereinfachter 5-zackiger Stern
  // Mittelpunkt + 5 Dreiecke
  for (int i = 0; i < 5; i++) {
    float angle1 = (i * 72 - 90) * PI / 180.0;
    float angle2 = ((i + 1) * 72 - 90) * PI / 180.0;
    int x1 = cx + cos(angle1) * r;
    int y1 = cy + sin(angle1) * r;
    int x2 = cx + cos(angle2) * r;
    int y2 = cy + sin(angle2) * r;
    tft->fillTriangle(cx, cy, x1, y1, x2, y2, color);
  }
}
