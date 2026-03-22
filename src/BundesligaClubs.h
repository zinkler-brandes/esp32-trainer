#ifndef SRC_BUNDESLIGACLUBS_H
#define SRC_BUNDESLIGACLUBS_H

#include <Arduino.h>

// Bundeslaender-Enum
enum Bundesland {
  BL_BADEN_WUERTTEMBERG = 0,
  BL_BAYERN,
  BL_BERLIN,
  BL_BRANDENBURG,
  BL_BREMEN,
  BL_HAMBURG,
  BL_HESSEN,
  BL_MECKLENBURG_VORPOMMERN,
  BL_NIEDERSACHSEN,
  BL_NRW,
  BL_RHEINLAND_PFALZ,
  BL_SAARLAND,
  BL_SACHSEN,
  BL_SACHSEN_ANHALT,
  BL_SCHLESWIG_HOLSTEIN,
  BL_THUERINGEN,
  BL_COUNT  // 16 Bundeslaender
};

// Bundesland-Namen (kurz fuer Buttons)
const char* const BUNDESLAND_NAMEN[] = {
  "Baden-Wuertt.",
  "Bayern",
  "Berlin",
  "Brandenburg",
  "Bremen",
  "Hamburg",
  "Hessen",
  "Meckl.-Vorp.",
  "Niedersachsen",
  "NRW",
  "Rheinl.-Pfalz",
  "Saarland",
  "Sachsen",
  "Sachsen-Anh.",
  "Schl.-Holstein",
  "Thueringen"
};

// Liga-Typen
enum Liga {
  LIGA_1 = 1,
  LIGA_2 = 2,
  LIGA_3 = 3
};

// Club-Datenstruktur
struct BundesligaClub {
  const char* name;       // Vereinsname
  const char* abbrev;     // 3-stellige Abkuerzung (fuer Logo-Lookup)
  Bundesland bundesland;  // Bundesland
  Liga liga;              // 1., 2. oder 3. Liga
  uint16_t primaryColor;  // Hauptfarbe (fuer generische Logos)
  uint16_t secondaryColor; // Sekundaerfarbe
};

// Farben (RGB565)
#define CLR_RED      0xF800
#define CLR_BLUE     0x001F
#define CLR_DARKBLUE 0x000F
#define CLR_GREEN    0x07E0
#define CLR_YELLOW   0xFE60
#define CLR_WHITE    0xFFFF
#define CLR_BLACK    0x0000
#define CLR_BROWN    0x8200
#define CLR_SKYBLUE  0x5D7F
#define CLR_ORANGE   0xFD20
#define CLR_PURPLE   0x780F

// ========== ALLE VEREINE 1.-3. LIGA (Saison 2024/25) ==========
// Sortiert nach Liga, dann alphabetisch

const BundesligaClub BUNDESLIGA_CLUBS[] = {
  // ===== 1. BUNDESLIGA (18 Vereine) =====
  {"FC Augsburg",          "FCA", BL_BAYERN,            LIGA_1, CLR_RED, CLR_GREEN},
  {"Union Berlin",         "FCU", BL_BERLIN,            LIGA_1, CLR_RED, CLR_WHITE},
  {"VfL Bochum",           "BOC", BL_NRW,               LIGA_1, CLR_BLUE, CLR_WHITE},
  {"Werder Bremen",        "SVW", BL_BREMEN,            LIGA_1, CLR_GREEN, CLR_WHITE},
  {"Borussia Dortmund",    "BVB", BL_NRW,               LIGA_1, CLR_YELLOW, CLR_BLACK},
  {"Eintr. Frankfurt",     "SGE", BL_HESSEN,            LIGA_1, CLR_BLACK, CLR_RED},
  {"SC Freiburg",          "SCF", BL_BADEN_WUERTTEMBERG, LIGA_1, CLR_RED, CLR_BLACK},
  {"1. FC Heidenheim",     "FCH", BL_BADEN_WUERTTEMBERG, LIGA_1, CLR_BLUE, CLR_RED},
  {"TSG Hoffenheim",       "TSG", BL_BADEN_WUERTTEMBERG, LIGA_1, CLR_BLUE, CLR_WHITE},
  {"Holstein Kiel",        "KSV", BL_SCHLESWIG_HOLSTEIN, LIGA_1, CLR_BLUE, CLR_WHITE},
  {"RB Leipzig",           "RBL", BL_SACHSEN,           LIGA_1, CLR_RED, CLR_WHITE},
  {"Bayer Leverkusen",     "B04", BL_NRW,               LIGA_1, CLR_RED, CLR_BLACK},
  {"1. FSV Mainz 05",      "M05", BL_RHEINLAND_PFALZ,   LIGA_1, CLR_RED, CLR_WHITE},
  {"Bor. M'gladbach",      "BMG", BL_NRW,               LIGA_1, CLR_BLACK, CLR_GREEN},
  {"FC Bayern Muenchen",   "FCB", BL_BAYERN,            LIGA_1, CLR_RED, CLR_WHITE},
  {"FC St. Pauli",         "STP", BL_HAMBURG,           LIGA_1, CLR_BROWN, CLR_WHITE},
  {"VfB Stuttgart",        "VFB", BL_BADEN_WUERTTEMBERG, LIGA_1, CLR_RED, CLR_WHITE},
  {"VfL Wolfsburg",        "WOB", BL_NIEDERSACHSEN,     LIGA_1, CLR_GREEN, CLR_WHITE},

  // ===== 2. BUNDESLIGA (18 Vereine) =====
  {"Eintracht Braunschw.", "EBS", BL_NIEDERSACHSEN,     LIGA_2, CLR_YELLOW, CLR_BLUE},
  {"SV Darmstadt 98",      "D98", BL_HESSEN,            LIGA_2, CLR_BLUE, CLR_WHITE},
  {"Fort. Duesseldorf",    "F95", BL_NRW,               LIGA_2, CLR_RED, CLR_WHITE},
  {"SV Elversberg",        "SVE", BL_SAARLAND,          LIGA_2, CLR_RED, CLR_WHITE},
  {"Greuther Fuerth",      "SGF", BL_BAYERN,            LIGA_2, CLR_GREEN, CLR_WHITE},
  {"Hamburger SV",         "HSV", BL_HAMBURG,           LIGA_2, CLR_BLUE, CLR_WHITE},
  {"Hannover 96",          "H96", BL_NIEDERSACHSEN,     LIGA_2, CLR_GREEN, CLR_WHITE},
  {"Hertha BSC",           "BSC", BL_BERLIN,            LIGA_2, CLR_BLUE, CLR_WHITE},
  {"1. FC Kaiserslautern", "FCK", BL_RHEINLAND_PFALZ,   LIGA_2, CLR_RED, CLR_WHITE},
  {"Karlsruher SC",        "KSC", BL_BADEN_WUERTTEMBERG, LIGA_2, CLR_BLUE, CLR_WHITE},
  {"1. FC Koeln",          "KOE", BL_NRW,               LIGA_2, CLR_RED, CLR_WHITE},
  {"1. FC Magdeburg",      "FCM", BL_SACHSEN_ANHALT,    LIGA_2, CLR_BLUE, CLR_WHITE},
  {"Preussen Muenster",    "SCM", BL_NRW,               LIGA_2, CLR_GREEN, CLR_WHITE},
  {"1. FC Nuernberg",      "FCN", BL_BAYERN,            LIGA_2, CLR_RED, CLR_WHITE},
  {"SC Paderborn 07",      "SCP", BL_NRW,               LIGA_2, CLR_BLUE, CLR_BLACK},
  {"Jahn Regensburg",      "SSV", BL_BAYERN,            LIGA_2, CLR_RED, CLR_WHITE},
  {"FC Schalke 04",        "S04", BL_NRW,               LIGA_2, CLR_BLUE, CLR_WHITE},
  {"SSV Ulm 1846",         "ULM", BL_BADEN_WUERTTEMBERG, LIGA_2, CLR_BLACK, CLR_WHITE},

  // ===== 3. LIGA (20 Vereine) =====
  {"Alemannia Aachen",     "AAC", BL_NRW,               LIGA_3, CLR_YELLOW, CLR_BLACK},
  {"Erzgebirge Aue",       "AUE", BL_SACHSEN,           LIGA_3, CLR_PURPLE, CLR_WHITE},
  {"Arminia Bielefeld",    "DSC", BL_NRW,               LIGA_3, CLR_BLUE, CLR_WHITE},
  {"Energie Cottbus",      "FCE", BL_BRANDENBURG,       LIGA_3, CLR_RED, CLR_WHITE},
  {"Dynamo Dresden",       "SGD", BL_SACHSEN,           LIGA_3, CLR_YELLOW, CLR_BLACK},
  {"MSV Duisburg",         "MSV", BL_NRW,               LIGA_3, CLR_BLUE, CLR_WHITE},
  {"FC Ingolstadt 04",     "FCI", BL_BAYERN,            LIGA_3, CLR_RED, CLR_BLACK},
  {"Viktoria Koeln",       "VIK", BL_NRW,               LIGA_3, CLR_RED, CLR_WHITE},
  {"Waldhof Mannheim",     "SVM", BL_BADEN_WUERTTEMBERG, LIGA_3, CLR_BLUE, CLR_BLACK},
  {"1860 Muenchen",        "M60", BL_BAYERN,            LIGA_3, CLR_BLUE, CLR_WHITE},
  {"VfL Osnabrueck",       "VFL", BL_NIEDERSACHSEN,     LIGA_3, CLR_PURPLE, CLR_WHITE},
  {"Hansa Rostock",        "HRO", BL_MECKLENBURG_VORPOMMERN, LIGA_3, CLR_BLUE, CLR_WHITE},
  {"Rot-Weiss Essen",      "RWE", BL_NRW,               LIGA_3, CLR_RED, CLR_WHITE},
  {"1. FC Saarbruecken",   "FCS", BL_SAARLAND,          LIGA_3, CLR_BLUE, CLR_BLACK},
  {"SV Sandhausen",        "SVS", BL_BADEN_WUERTTEMBERG, LIGA_3, CLR_BLACK, CLR_WHITE},
  {"SpVgg Unterhaching",   "SPU", BL_BAYERN,            LIGA_3, CLR_RED, CLR_BLUE},
  {"SC Verl",              "SCV", BL_NRW,               LIGA_3, CLR_RED, CLR_WHITE},
  {"Wehen Wiesbaden",      "SWW", BL_HESSEN,            LIGA_3, CLR_RED, CLR_BLACK},
  // Hinweis: Borussia Dortmund II und VfB Stuttgart II weggelassen (II-Teams)
};

const int BUNDESLIGA_CLUB_COUNT = sizeof(BUNDESLIGA_CLUBS) / sizeof(BundesligaClub);

// Hilfsfunktion: Bundesland-Name holen
inline const char* getBundeslandName(Bundesland bl) {
  if (bl >= 0 && bl < BL_COUNT) {
    return BUNDESLAND_NAMEN[bl];
  }
  return "Unbekannt";
}

#endif
