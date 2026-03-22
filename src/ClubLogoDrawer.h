#ifndef SRC_CLUBLOGODRAWER_H
#define SRC_CLUBLOGODRAWER_H

#include <Arduino.h>
#include <TFT_eSPI.h>

// Logo-Typen (Grundformen)
enum LogoType {
  LOGO_CIRCLE,           // Runder Hintergrund
  LOGO_SHIELD,           // Wappen-Form
  LOGO_DIAMOND,          // Raute
  LOGO_RECT,             // Rechteck
  LOGO_CUSTOM            // Spezielle Zeichnung
};

// Logo-Daten fuer einen Verein
struct ClubLogo {
  const char* abbrev;    // 3-stellige Abkuerzung (Lookup-Key)
  LogoType type;
  uint16_t bgColor;      // Hintergrundfarbe
  uint16_t fgColor;      // Vordergrund/Akzent
  uint16_t textColor;    // Textfarbe (falls Text gezeichnet wird)
  uint8_t customId;      // Fuer LOGO_CUSTOM
};

// Custom Logo IDs
#define CUSTOM_BAYERN      1
#define CUSTOM_DORTMUND    2
#define CUSTOM_SCHALKE     3
#define CUSTOM_FRANKFURT   4
#define CUSTOM_BREMEN      5
#define CUSTOM_GLADBACH    6
#define CUSTOM_HAMBURG     7
#define CUSTOM_KOELN       8
#define CUSTOM_HERTHA      9
#define CUSTOM_REAL_MADRID 10
#define CUSTOM_BARCELONA   11
#define CUSTOM_LIVERPOOL   12
#define CUSTOM_MAN_CITY    13
#define CUSTOM_MAN_UTD     14
#define CUSTOM_JUVENTUS    15
#define CUSTOM_MILAN       16
#define CUSTOM_INTER       17
#define CUSTOM_PSG         18
#define CUSTOM_CHELSEA     19
#define CUSTOM_ARSENAL     20
#define CUSTOM_AJAX        21
#define CUSTOM_CELTIC      22
#define CUSTOM_STPAULI     23
#define CUSTOM_UNION       24
#define CUSTOM_FREIBURG    25
#define CUSTOM_WOLFSBURG   26
#define CUSTOM_AUGSBURG    27
#define CUSTOM_MAINZ       28
#define CUSTOM_LEVERKUSEN  29
#define CUSTOM_LEIPZIG     30
#define CUSTOM_STUTTGART   31
#define CUSTOM_1860        32
#define CUSTOM_DRESDEN     33
#define CUSTOM_ATLETICO    34
#define CUSTOM_PORTO       35
#define CUSTOM_BENFICA     36

class ClubLogoDrawer {
  public:
    // Zeichnet Logo anhand der Abkuerzung
    static void drawLogo(TFT_eSPI* tft, int x, int y, int size, const char* abbrev);

    // Zeichnet Logo anhand der ClubLogo-Struktur
    static void drawLogoByData(TFT_eSPI* tft, int x, int y, int size, const ClubLogo& logo);

    // Findet Logo-Daten anhand der Abkuerzung
    static const ClubLogo* findLogo(const char* abbrev);

  private:
    // Grundformen
    static void drawCircleBg(TFT_eSPI* tft, int cx, int cy, int r, uint16_t color);
    static void drawShieldBg(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t color);
    static void drawDiamondBg(TFT_eSPI* tft, int cx, int cy, int size, uint16_t color);

    // Custom Logos
    static void drawBayern(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawDortmund(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawSchalke(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawFrankfurt(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawBremen(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawGladbach(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawHamburg(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawKoeln(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawHertha(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawRealMadrid(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawBarcelona(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawLiverpool(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawManCity(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawManUtd(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawJuventus(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawMilan(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawInter(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawPSG(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawChelsea(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawArsenal(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawAjax(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawCeltic(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawStPauli(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawUnion(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawFreiburg(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawWolfsburg(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawAugsburg(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawMainz(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawLeverkusen(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawLeipzig(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawStuttgart(TFT_eSPI* tft, int cx, int cy, int r);
    static void draw1860(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawDresden(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawAtletico(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawPorto(TFT_eSPI* tft, int cx, int cy, int r);
    static void drawBenfica(TFT_eSPI* tft, int cx, int cy, int r);

    // Generisches Logo mit Initialen
    static void drawGenericLogo(TFT_eSPI* tft, int cx, int cy, int r, uint16_t bg, uint16_t fg, const char* initials);
};

#endif
