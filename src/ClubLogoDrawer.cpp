#include "ClubLogoDrawer.h"
#include <string.h>

// Farben
#define LOGO_RED      0xF800
#define LOGO_BLUE     0x001F
#define LOGO_DARKBLUE 0x000F
#define LOGO_GREEN    0x07E0
#define LOGO_DARKGREEN 0x03E0
#define LOGO_YELLOW   0xFE60
#define LOGO_WHITE    0xFFFF
#define LOGO_BLACK    0x0000
#define LOGO_GOLD     0xFE60
#define LOGO_SKYBLUE  0x5D7F
#define LOGO_MAROON   0x8000
#define LOGO_BROWN    0x8200
#define LOGO_GREY     0x8410

// Alle Club-Logos (nach Abkuerzung sortiert fuer schnelle Suche)
const ClubLogo CLUB_LOGOS[] = {
  // DFB Teams
  {"FCB", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_BAYERN},
  {"BVB", LOGO_CUSTOM, LOGO_YELLOW, LOGO_BLACK, LOGO_BLACK, CUSTOM_DORTMUND},
  {"S04", LOGO_CUSTOM, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, CUSTOM_SCHALKE},
  {"SGE", LOGO_CUSTOM, LOGO_BLACK, LOGO_RED, LOGO_WHITE, CUSTOM_FRANKFURT},
  {"SVW", LOGO_CUSTOM, LOGO_GREEN, LOGO_WHITE, LOGO_WHITE, CUSTOM_BREMEN},
  {"BMG", LOGO_CUSTOM, LOGO_BLACK, LOGO_GREEN, LOGO_WHITE, CUSTOM_GLADBACH},
  {"HSV", LOGO_CUSTOM, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, CUSTOM_HAMBURG},
  {"KOE", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_KOELN},
  {"BSC", LOGO_CUSTOM, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, CUSTOM_HERTHA},
  {"B04", LOGO_CUSTOM, LOGO_RED, LOGO_BLACK, LOGO_WHITE, CUSTOM_LEVERKUSEN},
  {"RBL", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_LEIPZIG},
  {"VFB", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_STUTTGART},
  {"STP", LOGO_CUSTOM, LOGO_BROWN, LOGO_WHITE, LOGO_WHITE, CUSTOM_STPAULI},
  {"FCU", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_UNION},
  {"SCF", LOGO_CUSTOM, LOGO_RED, LOGO_BLACK, LOGO_WHITE, CUSTOM_FREIBURG},
  {"WOB", LOGO_CUSTOM, LOGO_GREEN, LOGO_WHITE, LOGO_WHITE, CUSTOM_WOLFSBURG},
  {"FCA", LOGO_CUSTOM, LOGO_RED, LOGO_GREEN, LOGO_WHITE, CUSTOM_AUGSBURG},
  {"M05", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_MAINZ},
  {"M60", LOGO_CUSTOM, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, CUSTOM_1860},
  {"SGD", LOGO_CUSTOM, LOGO_YELLOW, LOGO_BLACK, LOGO_BLACK, CUSTOM_DRESDEN},

  // Generische DFB-Teams (Kreis mit Farben)
  {"ULM", LOGO_CIRCLE, LOGO_BLACK, LOGO_WHITE, LOGO_WHITE, 0},
  {"SCP", LOGO_CIRCLE, LOGO_BLUE, LOGO_BLACK, LOGO_WHITE, 0},   // Paderborn
  {"SCM", LOGO_CIRCLE, LOGO_GREEN, LOGO_WHITE, LOGO_WHITE, 0},  // Preussen Muenster
  {"RWE", LOGO_CIRCLE, LOGO_RED, LOGO_WHITE, LOGO_WHITE, 0},
  {"VIK", LOGO_CIRCLE, LOGO_RED, LOGO_WHITE, LOGO_WHITE, 0},
  {"FCH", LOGO_CIRCLE, LOGO_BLUE, LOGO_RED, LOGO_WHITE, 0},     // Heidenheim
  {"FWK", LOGO_CIRCLE, LOGO_RED, LOGO_WHITE, LOGO_WHITE, 0},
  {"SVM", LOGO_CIRCLE, LOGO_BLUE, LOGO_BLACK, LOGO_WHITE, 0},
  {"FCE", LOGO_CIRCLE, LOGO_RED, LOGO_WHITE, LOGO_WHITE, 0},    // Energie Cottbus
  {"SVE", LOGO_CIRCLE, LOGO_RED, LOGO_WHITE, LOGO_WHITE, 0},
  {"FCS", LOGO_CIRCLE, LOGO_BLUE, LOGO_BLACK, LOGO_WHITE, 0},   // Saarbruecken
  {"SSV", LOGO_CIRCLE, LOGO_RED, LOGO_WHITE, LOGO_WHITE, 0},    // Jahn Regensburg
  {"SCV", LOGO_CIRCLE, LOGO_RED, LOGO_WHITE, LOGO_WHITE, 0},
  {"AUE", LOGO_CIRCLE, 0x7817, LOGO_WHITE, LOGO_WHITE, 0},      // Erzgebirge Aue (Lila)
  {"MSV", LOGO_CIRCLE, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, 0},
  {"DSC", LOGO_CIRCLE, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, 0},   // Arminia Bielefeld
  {"EBS", LOGO_CIRCLE, LOGO_YELLOW, LOGO_BLUE, LOGO_BLUE, 0},   // Braunschweig
  {"SGF", LOGO_CIRCLE, LOGO_GREEN, LOGO_WHITE, LOGO_WHITE, 0},  // Greuther Fuerth
  {"F95", LOGO_CIRCLE, LOGO_RED, LOGO_WHITE, LOGO_WHITE, 0},    // Fortuna Duesseldorf
  {"H96", LOGO_CIRCLE, LOGO_GREEN, LOGO_WHITE, LOGO_WHITE, 0},  // Hannover 96
  {"FCN", LOGO_CIRCLE, LOGO_RED, LOGO_WHITE, LOGO_WHITE, 0},    // 1. FC Nuernberg
  {"KSC", LOGO_CIRCLE, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, 0},   // Karlsruher SC
  {"BOC", LOGO_CIRCLE, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, 0},   // VfL Bochum
  {"KSV", LOGO_CIRCLE, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, 0},   // Holstein Kiel
  {"D98", LOGO_CIRCLE, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, 0},   // Darmstadt 98
  {"TSG", LOGO_CIRCLE, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, 0},   // Hoffenheim
  // Weitere Vereine fuer das Quiz
  {"FCK", LOGO_CIRCLE, LOGO_RED, LOGO_WHITE, LOGO_WHITE, 0},    // 1. FC Kaiserslautern
  {"FCM", LOGO_CIRCLE, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, 0},   // 1. FC Magdeburg
  {"AAC", LOGO_CIRCLE, LOGO_YELLOW, LOGO_BLACK, LOGO_BLACK, 0}, // Alemannia Aachen
  {"FCI", LOGO_CIRCLE, LOGO_RED, LOGO_BLACK, LOGO_WHITE, 0},    // FC Ingolstadt
  {"VFL", LOGO_CIRCLE, 0x7817, LOGO_WHITE, LOGO_WHITE, 0},      // VfL Osnabrueck (Lila)
  {"HRO", LOGO_CIRCLE, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, 0},   // Hansa Rostock
  {"SVS", LOGO_CIRCLE, LOGO_BLACK, LOGO_WHITE, LOGO_WHITE, 0},  // SV Sandhausen
  {"SPU", LOGO_CIRCLE, LOGO_RED, LOGO_BLUE, LOGO_WHITE, 0},     // SpVgg Unterhaching
  {"SWW", LOGO_CIRCLE, LOGO_RED, LOGO_BLACK, LOGO_WHITE, 0},    // Wehen Wiesbaden

  // CL Teams (International)
  {"RMA", LOGO_CUSTOM, LOGO_WHITE, LOGO_GOLD, LOGO_GOLD, CUSTOM_REAL_MADRID},
  {"MCI", LOGO_CUSTOM, LOGO_SKYBLUE, LOGO_WHITE, LOGO_WHITE, CUSTOM_MAN_CITY},
  {"LFC", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_LIVERPOOL},
  {"PSG", LOGO_CUSTOM, LOGO_BLUE, LOGO_RED, LOGO_WHITE, CUSTOM_PSG},
  {"JUV", LOGO_CUSTOM, LOGO_BLACK, LOGO_WHITE, LOGO_WHITE, CUSTOM_JUVENTUS},
  {"ACM", LOGO_CUSTOM, LOGO_RED, LOGO_BLACK, LOGO_WHITE, CUSTOM_MILAN},
  {"INT", LOGO_CUSTOM, LOGO_BLUE, LOGO_BLACK, LOGO_WHITE, CUSTOM_INTER},
  {"ATM", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_ATLETICO},
  {"POR", LOGO_CUSTOM, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, CUSTOM_PORTO},
  {"SLB", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_BENFICA},
  {"AJA", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_AJAX},
  {"CEL", LOGO_CUSTOM, LOGO_GREEN, LOGO_WHITE, LOGO_WHITE, CUSTOM_CELTIC},
  {"MUN", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_MAN_UTD},
  {"CHE", LOGO_CUSTOM, LOGO_BLUE, LOGO_WHITE, LOGO_WHITE, CUSTOM_CHELSEA},
  {"ARS", LOGO_CUSTOM, LOGO_RED, LOGO_WHITE, LOGO_WHITE, CUSTOM_ARSENAL},

  // Barcelona hat gleiche Abkuerzung wie Bayern - unterscheiden durch Kontext
  // Fuer jetzt: verwenden wir FCB fuer Bayern (da DFB Prioritaet hat)
};

const int CLUB_LOGO_COUNT = sizeof(CLUB_LOGOS) / sizeof(ClubLogo);

const ClubLogo* ClubLogoDrawer::findLogo(const char* abbrev) {
  for (int i = 0; i < CLUB_LOGO_COUNT; i++) {
    if (strcmp(CLUB_LOGOS[i].abbrev, abbrev) == 0) {
      return &CLUB_LOGOS[i];
    }
  }
  return nullptr;
}

void ClubLogoDrawer::drawLogo(TFT_eSPI* tft, int x, int y, int size, const char* abbrev) {
  const ClubLogo* logo = findLogo(abbrev);
  if (logo) {
    drawLogoByData(tft, x, y, size, *logo);
  } else {
    // Fallback: Grauer Kreis mit Abkuerzung
    int r = size / 2;
    tft->fillCircle(x + r, y + r, r, LOGO_GREY);
    tft->setTextColor(LOGO_WHITE, LOGO_GREY);
    tft->setTextSize(1);
    if (abbrev && strlen(abbrev) > 0) {
      tft->setCursor(x + r - 6, y + r - 4);
      tft->print(abbrev[0]);
    }
  }
}

void ClubLogoDrawer::drawLogoByData(TFT_eSPI* tft, int x, int y, int size, const ClubLogo& logo) {
  int r = size / 2;
  int cx = x + r;
  int cy = y + r;

  if (logo.type == LOGO_CUSTOM) {
    // Custom Logo zeichnen
    switch (logo.customId) {
      case CUSTOM_BAYERN: drawBayern(tft, cx, cy, r); break;
      case CUSTOM_DORTMUND: drawDortmund(tft, cx, cy, r); break;
      case CUSTOM_SCHALKE: drawSchalke(tft, cx, cy, r); break;
      case CUSTOM_FRANKFURT: drawFrankfurt(tft, cx, cy, r); break;
      case CUSTOM_BREMEN: drawBremen(tft, cx, cy, r); break;
      case CUSTOM_GLADBACH: drawGladbach(tft, cx, cy, r); break;
      case CUSTOM_HAMBURG: drawHamburg(tft, cx, cy, r); break;
      case CUSTOM_KOELN: drawKoeln(tft, cx, cy, r); break;
      case CUSTOM_HERTHA: drawHertha(tft, cx, cy, r); break;
      case CUSTOM_LEVERKUSEN: drawLeverkusen(tft, cx, cy, r); break;
      case CUSTOM_LEIPZIG: drawLeipzig(tft, cx, cy, r); break;
      case CUSTOM_STUTTGART: drawStuttgart(tft, cx, cy, r); break;
      case CUSTOM_STPAULI: drawStPauli(tft, cx, cy, r); break;
      case CUSTOM_UNION: drawUnion(tft, cx, cy, r); break;
      case CUSTOM_FREIBURG: drawFreiburg(tft, cx, cy, r); break;
      case CUSTOM_WOLFSBURG: drawWolfsburg(tft, cx, cy, r); break;
      case CUSTOM_AUGSBURG: drawAugsburg(tft, cx, cy, r); break;
      case CUSTOM_MAINZ: drawMainz(tft, cx, cy, r); break;
      case CUSTOM_1860: draw1860(tft, cx, cy, r); break;
      case CUSTOM_DRESDEN: drawDresden(tft, cx, cy, r); break;
      case CUSTOM_REAL_MADRID: drawRealMadrid(tft, cx, cy, r); break;
      case CUSTOM_BARCELONA: drawBarcelona(tft, cx, cy, r); break;
      case CUSTOM_LIVERPOOL: drawLiverpool(tft, cx, cy, r); break;
      case CUSTOM_MAN_CITY: drawManCity(tft, cx, cy, r); break;
      case CUSTOM_MAN_UTD: drawManUtd(tft, cx, cy, r); break;
      case CUSTOM_JUVENTUS: drawJuventus(tft, cx, cy, r); break;
      case CUSTOM_MILAN: drawMilan(tft, cx, cy, r); break;
      case CUSTOM_INTER: drawInter(tft, cx, cy, r); break;
      case CUSTOM_PSG: drawPSG(tft, cx, cy, r); break;
      case CUSTOM_CHELSEA: drawChelsea(tft, cx, cy, r); break;
      case CUSTOM_ARSENAL: drawArsenal(tft, cx, cy, r); break;
      case CUSTOM_AJAX: drawAjax(tft, cx, cy, r); break;
      case CUSTOM_CELTIC: drawCeltic(tft, cx, cy, r); break;
      case CUSTOM_ATLETICO: drawAtletico(tft, cx, cy, r); break;
      case CUSTOM_PORTO: drawPorto(tft, cx, cy, r); break;
      case CUSTOM_BENFICA: drawBenfica(tft, cx, cy, r); break;
      default:
        drawGenericLogo(tft, cx, cy, r, logo.bgColor, logo.fgColor, logo.abbrev);
        break;
    }
  } else if (logo.type == LOGO_CIRCLE) {
    drawGenericLogo(tft, cx, cy, r, logo.bgColor, logo.fgColor, logo.abbrev);
  } else if (logo.type == LOGO_SHIELD) {
    drawShieldBg(tft, x, y, size, size, logo.bgColor);
  } else if (logo.type == LOGO_DIAMOND) {
    drawDiamondBg(tft, cx, cy, r, logo.bgColor);
  }
}

// === Grundformen ===

void ClubLogoDrawer::drawCircleBg(TFT_eSPI* tft, int cx, int cy, int r, uint16_t color) {
  tft->fillCircle(cx, cy, r, color);
}

void ClubLogoDrawer::drawShieldBg(TFT_eSPI* tft, int x, int y, int w, int h, uint16_t color) {
  // Vereinfachtes Wappen
  tft->fillRect(x, y, w, h * 2 / 3, color);
  tft->fillTriangle(x, y + h * 2 / 3, x + w / 2, y + h, x + w, y + h * 2 / 3, color);
}

void ClubLogoDrawer::drawDiamondBg(TFT_eSPI* tft, int cx, int cy, int size, uint16_t color) {
  tft->fillTriangle(cx, cy - size, cx + size, cy, cx, cy + size, color);
  tft->fillTriangle(cx, cy - size, cx - size, cy, cx, cy + size, color);
}

void ClubLogoDrawer::drawGenericLogo(TFT_eSPI* tft, int cx, int cy, int r, uint16_t bg, uint16_t fg, const char* initials) {
  tft->fillCircle(cx, cy, r, bg);
  tft->drawCircle(cx, cy, r, fg);

  // Initial zeichnen
  if (initials && strlen(initials) > 0) {
    tft->setTextColor(fg, bg);
    tft->setTextSize(1);
    int textW = strlen(initials) <= 2 ? 6 * strlen(initials) : 6;
    tft->setCursor(cx - textW / 2, cy - 3);
    if (strlen(initials) <= 2) {
      tft->print(initials);
    } else {
      tft->print(initials[0]);
    }
  }
}

// === Custom Logos ===

void ClubLogoDrawer::drawBayern(TFT_eSPI* tft, int cx, int cy, int r) {
  // Roter Kreis mit weiss-blauem Rautenmuster
  tft->fillCircle(cx, cy, r, LOGO_RED);
  tft->fillCircle(cx, cy, r - 2, LOGO_WHITE);

  // Bayern-Rauten (vereinfacht als blau-weisses Schachbrett)
  int inner = r - 3;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if ((i + j + 2) % 2 == 0) {
        tft->fillRect(cx + i * (inner/2) - 2, cy + j * (inner/2) - 2, inner/2, inner/2, LOGO_BLUE);
      }
    }
  }
}

void ClubLogoDrawer::drawDortmund(TFT_eSPI* tft, int cx, int cy, int r) {
  // Gelber Kreis mit schwarzem Ring und "09"
  tft->fillCircle(cx, cy, r, LOGO_YELLOW);
  tft->drawCircle(cx, cy, r - 1, LOGO_BLACK);
  tft->drawCircle(cx, cy, r - 2, LOGO_BLACK);

  // "09" in der Mitte
  tft->setTextColor(LOGO_BLACK, LOGO_YELLOW);
  tft->setTextSize(1);
  tft->setCursor(cx - 5, cy - 3);
  tft->print("09");
}

void ClubLogoDrawer::drawSchalke(TFT_eSPI* tft, int cx, int cy, int r) {
  // Blauer Kreis mit "S04"
  tft->fillCircle(cx, cy, r, LOGO_BLUE);
  tft->setTextColor(LOGO_WHITE, LOGO_BLUE);
  tft->setTextSize(1);
  tft->setCursor(cx - 6, cy - 3);
  tft->print("S4");
}

void ClubLogoDrawer::drawFrankfurt(TFT_eSPI* tft, int cx, int cy, int r) {
  // Schwarz mit rotem Adler (vereinfacht)
  tft->fillCircle(cx, cy, r, LOGO_BLACK);
  tft->drawCircle(cx, cy, r - 1, LOGO_RED);

  // Adler-Silhouette (sehr vereinfacht als V)
  tft->drawLine(cx - r/2, cy - r/3, cx, cy + r/3, LOGO_RED);
  tft->drawLine(cx + r/2, cy - r/3, cx, cy + r/3, LOGO_RED);
  tft->drawLine(cx - r/2, cy - r/3, cx, cy - r/2, LOGO_RED);
  tft->drawLine(cx + r/2, cy - r/3, cx, cy - r/2, LOGO_RED);
}

void ClubLogoDrawer::drawBremen(TFT_eSPI* tft, int cx, int cy, int r) {
  // Gruener Kreis mit weisser Raute
  tft->fillCircle(cx, cy, r, LOGO_GREEN);
  // Raute
  int d = r / 2;
  tft->fillTriangle(cx, cy - d, cx + d, cy, cx, cy + d, LOGO_WHITE);
  tft->fillTriangle(cx, cy - d, cx - d, cy, cx, cy + d, LOGO_WHITE);
}

void ClubLogoDrawer::drawGladbach(TFT_eSPI* tft, int cx, int cy, int r) {
  // Schwarz mit gruener Raute
  tft->fillCircle(cx, cy, r, LOGO_BLACK);
  int d = r / 2;
  tft->fillTriangle(cx, cy - d, cx + d, cy, cx, cy + d, LOGO_GREEN);
  tft->fillTriangle(cx, cy - d, cx - d, cy, cx, cy + d, LOGO_GREEN);
}

void ClubLogoDrawer::drawHamburg(TFT_eSPI* tft, int cx, int cy, int r) {
  // Blauer Kreis mit weisser Raute
  tft->fillCircle(cx, cy, r, LOGO_BLUE);
  int d = r * 2 / 3;
  tft->fillTriangle(cx, cy - d, cx + d, cy, cx, cy + d, LOGO_WHITE);
  tft->fillTriangle(cx, cy - d, cx - d, cy, cx, cy + d, LOGO_WHITE);
}

void ClubLogoDrawer::drawKoeln(TFT_eSPI* tft, int cx, int cy, int r) {
  // Weisser Kreis mit rotem Bock (vereinfacht als Dreieck)
  tft->fillCircle(cx, cy, r, LOGO_WHITE);
  tft->drawCircle(cx, cy, r, LOGO_RED);
  tft->fillTriangle(cx - r/3, cy + r/2, cx, cy - r/2, cx + r/3, cy + r/2, LOGO_RED);
}

void ClubLogoDrawer::drawHertha(TFT_eSPI* tft, int cx, int cy, int r) {
  // Blau-weiss horizontal geteilt
  tft->fillCircle(cx, cy, r, LOGO_BLUE);
  tft->fillRect(cx - r, cy, r * 2, r + 1, LOGO_WHITE);
  tft->drawCircle(cx, cy, r, LOGO_BLUE);

  // Flagge
  tft->fillRect(cx - 1, cy - r + 2, 2, r - 2, LOGO_RED);
  tft->fillRect(cx - 2, cy - r + 2, 4, 3, LOGO_RED);
}

void ClubLogoDrawer::drawLeverkusen(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot mit schwarzem Kreuz
  tft->fillCircle(cx, cy, r, LOGO_RED);
  tft->fillRect(cx - 1, cy - r + 2, 2, r * 2 - 4, LOGO_BLACK);
  tft->fillRect(cx - r + 2, cy - 1, r * 2 - 4, 2, LOGO_BLACK);
}

void ClubLogoDrawer::drawLeipzig(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot mit weissem Ball
  tft->fillCircle(cx, cy, r, LOGO_RED);
  tft->fillCircle(cx, cy, r / 2, LOGO_WHITE);

  // "RB" andeuten
  tft->setTextColor(LOGO_RED, LOGO_WHITE);
  tft->setTextSize(1);
  tft->setCursor(cx - 4, cy - 3);
  tft->print("R");
}

void ClubLogoDrawer::drawStuttgart(TFT_eSPI* tft, int cx, int cy, int r) {
  // Weiss mit rotem Ring
  tft->fillCircle(cx, cy, r, LOGO_WHITE);
  tft->drawCircle(cx, cy, r, LOGO_RED);
  tft->drawCircle(cx, cy, r - 1, LOGO_RED);

  // Hirsch (vereinfacht als Y-Form)
  tft->drawLine(cx, cy - r/3, cx, cy + r/2, LOGO_BLACK);
  tft->drawLine(cx, cy - r/3, cx - r/3, cy - r*2/3, LOGO_BLACK);
  tft->drawLine(cx, cy - r/3, cx + r/3, cy - r*2/3, LOGO_BLACK);
}

void ClubLogoDrawer::drawStPauli(TFT_eSPI* tft, int cx, int cy, int r) {
  // Braun mit Totenkopf (vereinfacht)
  tft->fillCircle(cx, cy, r, LOGO_BROWN);

  // Schaedel
  tft->fillCircle(cx, cy - 1, r / 2, LOGO_WHITE);
  // Augen
  tft->fillCircle(cx - 2, cy - 2, 1, LOGO_BLACK);
  tft->fillCircle(cx + 2, cy - 2, 1, LOGO_BLACK);
  // Kreuz darunter
  tft->drawLine(cx - r/3, cy + r/2, cx + r/3, cy + r/2, LOGO_WHITE);
  tft->drawLine(cx, cy + r/3, cx, cy + r - 1, LOGO_WHITE);
}

void ClubLogoDrawer::drawUnion(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot mit gelbem Wappen-Umriss
  tft->fillCircle(cx, cy, r, LOGO_RED);
  tft->drawCircle(cx, cy, r - 2, LOGO_YELLOW);

  // "U" Symbol
  tft->setTextColor(LOGO_WHITE, LOGO_RED);
  tft->setTextSize(1);
  tft->setCursor(cx - 3, cy - 3);
  tft->print("U");
}

void ClubLogoDrawer::drawFreiburg(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot mit schwarzem Kreuz (Freiburg Wappen)
  tft->fillCircle(cx, cy, r, LOGO_RED);
  tft->fillRect(cx - 1, cy - r + 2, 2, r * 2 - 4, LOGO_WHITE);
  tft->fillRect(cx - r + 2, cy - 1, r * 2 - 4, 2, LOGO_WHITE);
}

void ClubLogoDrawer::drawWolfsburg(TFT_eSPI* tft, int cx, int cy, int r) {
  // Gruen mit weissem W
  tft->fillCircle(cx, cy, r, LOGO_GREEN);

  // W zeichnen
  int w = r * 2 / 3;
  tft->drawLine(cx - w/2, cy - r/3, cx - w/4, cy + r/3, LOGO_WHITE);
  tft->drawLine(cx - w/4, cy + r/3, cx, cy - r/4, LOGO_WHITE);
  tft->drawLine(cx, cy - r/4, cx + w/4, cy + r/3, LOGO_WHITE);
  tft->drawLine(cx + w/4, cy + r/3, cx + w/2, cy - r/3, LOGO_WHITE);
}

void ClubLogoDrawer::drawAugsburg(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot/Gruen geteilt mit weissem Zirbelnuss
  tft->fillCircle(cx, cy, r, LOGO_RED);
  // Rechte Haelfte gruen
  tft->fillRect(cx, cy - r, r + 1, r * 2 + 1, LOGO_GREEN);
  tft->drawCircle(cx, cy, r, LOGO_WHITE);

  // Weisses Zentrum
  tft->fillCircle(cx, cy, r / 3, LOGO_WHITE);
}

void ClubLogoDrawer::drawMainz(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot mit weissem Rad
  tft->fillCircle(cx, cy, r, LOGO_RED);
  tft->drawCircle(cx, cy, r / 2, LOGO_WHITE);

  // Speichen
  for (int i = 0; i < 6; i++) {
    float angle = i * PI / 3;
    int x1 = cx + cos(angle) * (r / 2);
    int y1 = cy + sin(angle) * (r / 2);
    tft->drawLine(cx, cy, x1, y1, LOGO_WHITE);
  }
}

void ClubLogoDrawer::draw1860(TFT_eSPI* tft, int cx, int cy, int r) {
  // Blau mit Loewe (vereinfacht)
  tft->fillCircle(cx, cy, r, LOGO_BLUE);

  // "60" zeichnen
  tft->setTextColor(LOGO_WHITE, LOGO_BLUE);
  tft->setTextSize(1);
  tft->setCursor(cx - 5, cy - 3);
  tft->print("60");
}

void ClubLogoDrawer::drawDresden(TFT_eSPI* tft, int cx, int cy, int r) {
  // Gelb mit schwarzem D
  tft->fillCircle(cx, cy, r, LOGO_YELLOW);
  tft->drawCircle(cx, cy, r, LOGO_BLACK);

  tft->setTextColor(LOGO_BLACK, LOGO_YELLOW);
  tft->setTextSize(1);
  tft->setCursor(cx - 3, cy - 3);
  tft->print("D");
}

// === Internationale Clubs ===

void ClubLogoDrawer::drawRealMadrid(TFT_eSPI* tft, int cx, int cy, int r) {
  // Weiss mit goldener Krone
  tft->fillCircle(cx, cy, r, LOGO_WHITE);
  tft->drawCircle(cx, cy, r, LOGO_GOLD);

  // Krone oben
  tft->fillRect(cx - r/2, cy - r/3, r, 2, LOGO_GOLD);
  tft->fillTriangle(cx - r/2, cy - r/3, cx - r/3, cy - r*2/3, cx - r/6, cy - r/3, LOGO_GOLD);
  tft->fillTriangle(cx - r/6, cy - r/3, cx, cy - r*2/3, cx + r/6, cy - r/3, LOGO_GOLD);
  tft->fillTriangle(cx + r/6, cy - r/3, cx + r/3, cy - r*2/3, cx + r/2, cy - r/3, LOGO_GOLD);
}

void ClubLogoDrawer::drawBarcelona(TFT_eSPI* tft, int cx, int cy, int r) {
  // Blau-Rot gestreift
  tft->fillCircle(cx, cy, r, LOGO_BLUE);
  // Rote Streifen
  for (int i = 0; i < 3; i++) {
    int x = cx - r + (i * 2 + 1) * r / 3;
    tft->fillRect(x, cy - r, r / 3, r * 2, LOGO_RED);
  }
  tft->drawCircle(cx, cy, r, LOGO_GOLD);
}

void ClubLogoDrawer::drawLiverpool(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot mit Liverbird (vereinfacht)
  tft->fillCircle(cx, cy, r, LOGO_RED);

  // Vogel-Silhouette (sehr vereinfacht)
  tft->fillTriangle(cx, cy - r/2, cx - r/3, cy + r/3, cx + r/3, cy + r/3, LOGO_GOLD);
  tft->fillCircle(cx, cy - r/3, r/4, LOGO_GOLD);
}

void ClubLogoDrawer::drawManCity(TFT_eSPI* tft, int cx, int cy, int r) {
  // Hellblau mit Adler
  tft->fillCircle(cx, cy, r, LOGO_SKYBLUE);
  tft->drawCircle(cx, cy, r, LOGO_GOLD);

  // Schiff (vereinfacht)
  tft->fillRect(cx - r/2, cy, r, 2, LOGO_GOLD);
  tft->fillTriangle(cx, cy - r/2, cx - r/4, cy, cx + r/4, cy, LOGO_GOLD);
}

void ClubLogoDrawer::drawManUtd(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot mit Teufel (vereinfacht)
  tft->fillCircle(cx, cy, r, LOGO_RED);
  tft->drawCircle(cx, cy, r, LOGO_GOLD);

  // Dreizack
  tft->drawLine(cx, cy - r/2, cx, cy + r/3, LOGO_GOLD);
  tft->drawLine(cx - r/3, cy - r/3, cx, cy - r/2, LOGO_GOLD);
  tft->drawLine(cx + r/3, cy - r/3, cx, cy - r/2, LOGO_GOLD);
}

void ClubLogoDrawer::drawJuventus(TFT_eSPI* tft, int cx, int cy, int r) {
  // Schwarz-weiss gestreift
  tft->fillCircle(cx, cy, r, LOGO_BLACK);
  // Weisse Streifen
  tft->fillRect(cx - r/3, cy - r, r/3 * 2, r * 2, LOGO_WHITE);
  tft->drawCircle(cx, cy, r, LOGO_WHITE);
}

void ClubLogoDrawer::drawMilan(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot-schwarz gestreift
  tft->fillCircle(cx, cy, r, LOGO_RED);
  // Schwarze Streifen
  tft->fillRect(cx - r/2, cy - r, r/4, r * 2, LOGO_BLACK);
  tft->fillRect(cx, cy - r, r/4, r * 2, LOGO_BLACK);
  tft->drawCircle(cx, cy, r, LOGO_WHITE);
}

void ClubLogoDrawer::drawInter(TFT_eSPI* tft, int cx, int cy, int r) {
  // Blau-schwarz gestreift mit goldener Schrift
  tft->fillCircle(cx, cy, r, LOGO_BLUE);
  tft->fillRect(cx - r/2, cy - r, r/4, r * 2, LOGO_BLACK);
  tft->fillRect(cx + r/4, cy - r, r/4, r * 2, LOGO_BLACK);

  // "IM" in der Mitte
  tft->setTextColor(LOGO_GOLD, LOGO_BLUE);
  tft->setTextSize(1);
  tft->setCursor(cx - 4, cy - 3);
  tft->print("I");
}

void ClubLogoDrawer::drawPSG(TFT_eSPI* tft, int cx, int cy, int r) {
  // Blau mit Eiffelturm (vereinfacht)
  tft->fillCircle(cx, cy, r, LOGO_DARKBLUE);
  tft->drawCircle(cx, cy, r, LOGO_RED);

  // Eiffelturm (vereinfacht als Dreieck)
  tft->fillTriangle(cx - r/3, cy + r/2, cx, cy - r/2, cx + r/3, cy + r/2, LOGO_GOLD);
}

void ClubLogoDrawer::drawChelsea(TFT_eSPI* tft, int cx, int cy, int r) {
  // Blau mit Loewe
  tft->fillCircle(cx, cy, r, LOGO_BLUE);
  tft->drawCircle(cx, cy, r, LOGO_WHITE);

  // Loewe (vereinfacht als C)
  tft->setTextColor(LOGO_GOLD, LOGO_BLUE);
  tft->setTextSize(1);
  tft->setCursor(cx - 3, cy - 3);
  tft->print("C");
}

void ClubLogoDrawer::drawArsenal(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot mit Kanone
  tft->fillCircle(cx, cy, r, LOGO_RED);

  // Kanone (vereinfacht als Linie)
  tft->fillRect(cx - r/2, cy - 1, r, 3, LOGO_GOLD);
  tft->fillCircle(cx + r/2 - 1, cy, 2, LOGO_GOLD);
}

void ClubLogoDrawer::drawAjax(TFT_eSPI* tft, int cx, int cy, int r) {
  // Weiss mit rotem Streifen
  tft->fillCircle(cx, cy, r, LOGO_WHITE);
  tft->fillRect(cx - 1, cy - r, 3, r * 2, LOGO_RED);
  tft->drawCircle(cx, cy, r, LOGO_RED);
}

void ClubLogoDrawer::drawCeltic(TFT_eSPI* tft, int cx, int cy, int r) {
  // Gruen-weiss gestreift
  tft->fillCircle(cx, cy, r, LOGO_GREEN);
  tft->fillRect(cx - r/3, cy - r, r/3 * 2, r * 2, LOGO_WHITE);
  tft->drawCircle(cx, cy, r, LOGO_GREEN);
}

void ClubLogoDrawer::drawAtletico(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot-weiss gestreift
  tft->fillCircle(cx, cy, r, LOGO_RED);
  tft->fillRect(cx - r/2, cy - r, r/4, r * 2, LOGO_WHITE);
  tft->fillRect(cx + r/4, cy - r, r/4, r * 2, LOGO_WHITE);
  tft->drawCircle(cx, cy, r, LOGO_BLUE);
}

void ClubLogoDrawer::drawPorto(TFT_eSPI* tft, int cx, int cy, int r) {
  // Blau-weiss
  tft->fillCircle(cx, cy, r, LOGO_BLUE);
  tft->fillRect(cx - r, cy, r * 2, r + 1, LOGO_WHITE);
  tft->drawCircle(cx, cy, r, LOGO_GOLD);
}

void ClubLogoDrawer::drawBenfica(TFT_eSPI* tft, int cx, int cy, int r) {
  // Rot mit Adler
  tft->fillCircle(cx, cy, r, LOGO_RED);

  // Adler (vereinfacht)
  tft->fillTriangle(cx, cy - r/2, cx - r/3, cy + r/3, cx + r/3, cy + r/3, LOGO_WHITE);
  tft->fillCircle(cx, cy - r/3, r/5, LOGO_WHITE);
}
