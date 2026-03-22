#ifndef SRC_WM2026TEAMS_H
#define SRC_WM2026TEAMS_H

#include <Arduino.h>
#include <TFT_eSPI.h>

// Team-Tiers (Antwortzeit basiert auf FIFA-Ranking)
enum TeamTier {
  TIER_1_ELITE,    // 8 Sek - Top 12 Teams
  TIER_2_STRONG,   // 10 Sek - Teams 13-24
  TIER_3_MEDIUM,   // 12 Sek - Teams 25-36
  TIER_4_WEAK      // 14 Sek - Teams 37-48
};

// Antwortzeiten pro Tier in Millisekunden
#define WM_ANSWER_TIME_TIER1  8000   // Elite: 8 Sek
#define WM_ANSWER_TIME_TIER2  10000  // Stark: 10 Sek
#define WM_ANSWER_TIME_TIER3  12000  // Mittel: 12 Sek
#define WM_ANSWER_TIME_TIER4  14000  // Schwach: 14 Sek

// CPU Elfmeter-Trefferwahrscheinlichkeit pro Tier (%)
#define WM_PENALTY_PROB_TIER1  80
#define WM_PENALTY_PROB_TIER2  70
#define WM_PENALTY_PROB_TIER3  60
#define WM_PENALTY_PROB_TIER4  50

struct WMTeam {
  const char* name;
  const char* abbrev;      // 3-stellig (FIFA-Codes)
  uint16_t shirtColor;     // Trikot-Farbe
  uint16_t shortsColor;    // Hosen-Farbe
  uint16_t socksColor;     // Stutzen-Farbe
  TeamTier tier;
};

// Hilfsfunktion: Sichere Textfarbe (weiss wenn Hintergrund zu dunkel)
inline uint16_t getTextColorForBg(uint16_t bgColor) {
  // Wenn Hintergrund schwarz oder sehr dunkel -> weisser Text
  if (bgColor == 0x0000 || bgColor < 0x2000) {
    return TFT_WHITE;
  }
  return bgColor;
}

// ========== ALLE 48 WM 2026 TEAMS ==========
// Sortiert nach FIFA-Ranking / Tier

// Trikot, Hose, Stutzen - Original-Farben der Nationalmannschaften
const WMTeam WM_TEAMS[] = {
  // ===== TIER 1 - ELITE (8 Sekunden) - 12 Teams =====
  // Index 0-11
  //                Name              Abbr   Trikot    Hose      Stutzen   Tier
  {"Deutschland", "GER", TFT_WHITE, 0x0000, TFT_WHITE, TIER_1_ELITE},       // 0 - Weiss/Schwarz/Weiss
  {"Spanien", "ESP", 0xF800, 0x001F, 0xF800, TIER_1_ELITE},                 // 1 - Rot/Dunkelblau/Rot
  {"England", "ENG", TFT_WHITE, TFT_WHITE, TFT_WHITE, TIER_1_ELITE},        // 2 - Weiss/Weiss/Weiss
  {"Frankreich", "FRA", 0x001F, TFT_WHITE, 0xF800, TIER_1_ELITE},           // 3 - Blau/Weiss/Rot
  {"Argentinien", "ARG", 0x5D7F, 0x0000, TFT_WHITE, TIER_1_ELITE},          // 4 - Hellblau/Schwarz/Weiss
  {"Brasilien", "BRA", 0xFE60, 0x001F, TFT_WHITE, TIER_1_ELITE},            // 5 - Gelb/Blau/Weiss
  {"Portugal", "POR", 0xF800, 0x07E0, 0xF800, TIER_1_ELITE},                // 6 - Rot/Gruen/Rot
  {"Niederlande", "NED", 0xFBE0, TFT_WHITE, 0xFBE0, TIER_1_ELITE},          // 7 - Orange/Weiss/Orange
  {"Belgien", "BEL", 0xF800, 0xF800, 0xF800, TIER_1_ELITE},                 // 8 - Rot/Rot/Rot
  {"Kroatien", "CRO", 0xF800, TFT_WHITE, TFT_WHITE, TIER_1_ELITE},          // 9 - Rot-Weiss/Weiss/Weiss
  {"Uruguay", "URU", 0x5D7F, 0x0000, 0x0000, TIER_1_ELITE},                 // 10 - Hellblau/Schwarz/Schwarz
  {"Kolumbien", "COL", 0xFE60, 0x001F, 0xF800, TIER_1_ELITE},               // 11 - Gelb/Blau/Rot

  // ===== TIER 2 - STARK (10 Sekunden) - 12 Teams =====
  // Index 12-23
  {"Mexiko", "MEX", 0x07E0, TFT_WHITE, 0xF800, TIER_2_STRONG},              // 12 - Gruen/Weiss/Rot
  {"USA", "USA", TFT_WHITE, 0x001F, TFT_WHITE, TIER_2_STRONG},              // 13 - Weiss/Dunkelblau/Weiss
  {"Schweiz", "SUI", 0xF800, TFT_WHITE, 0xF800, TIER_2_STRONG},             // 14 - Rot/Weiss/Rot
  {"Japan", "JPN", 0x001F, TFT_WHITE, 0x001F, TIER_2_STRONG},               // 15 - Blau/Weiss/Blau
  {"Marokko", "MAR", 0xF800, 0x07E0, 0xF800, TIER_2_STRONG},                // 16 - Rot/Gruen/Rot
  {"Senegal", "SEN", TFT_WHITE, TFT_WHITE, 0x07E0, TIER_2_STRONG},          // 17 - Weiss/Weiss/Gruen
  {"Ecuador", "ECU", 0xFE60, 0x001F, 0xF800, TIER_2_STRONG},                // 18 - Gelb/Blau/Rot
  {"Oesterreich", "AUT", 0xF800, TFT_WHITE, 0xF800, TIER_2_STRONG},         // 19 - Rot/Weiss/Rot
  {"Norwegen", "NOR", 0xF800, TFT_WHITE, 0xF800, TIER_2_STRONG},            // 20 - Rot/Weiss/Rot
  {"Italien", "ITA", 0x001F, TFT_WHITE, 0x001F, TIER_2_STRONG},             // 21 - Blau/Weiss/Blau
  {"Ukraine", "UKR", 0xFE60, 0x001F, 0xFE60, TIER_2_STRONG},                // 22 - Gelb/Blau/Gelb
  {"Tuerkei", "TUR", 0xF800, TFT_WHITE, 0xF800, TIER_2_STRONG},             // 23 - Rot/Weiss/Rot

  // ===== TIER 3 - MITTEL (12 Sekunden) - 12 Teams =====
  // Index 24-35
  {"Kanada", "CAN", 0xF800, 0xF800, 0xF800, TIER_3_MEDIUM},                 // 24 - Rot/Rot/Rot
  {"Katar", "QAT", 0x8000, TFT_WHITE, 0x8000, TIER_3_MEDIUM},               // 25 - Bordeaux/Weiss/Bordeaux
  {"Algerien", "ALG", TFT_WHITE, TFT_WHITE, TFT_WHITE, TIER_3_MEDIUM},      // 26 - Weiss/Weiss/Weiss
  {"Aegypten", "EGY", 0xF800, TFT_WHITE, 0xF800, TIER_3_MEDIUM},            // 27 - Rot/Weiss/Rot
  {"Iran", "IRN", TFT_WHITE, TFT_WHITE, TFT_WHITE, TIER_3_MEDIUM},          // 28 - Weiss/Weiss/Weiss
  {"Saudi-Arabien", "KSA", 0x07E0, TFT_WHITE, 0x07E0, TIER_3_MEDIUM},       // 29 - Gruen/Weiss/Gruen
  {"Tunesien", "TUN", 0xF800, TFT_WHITE, 0xF800, TIER_3_MEDIUM},            // 30 - Rot/Weiss/Rot
  {"Ghana", "GHA", TFT_WHITE, TFT_WHITE, TFT_WHITE, TIER_3_MEDIUM},         // 31 - Weiss/Weiss/Weiss
  {"Elfenbeinkueste", "CIV", 0xFBE0, TFT_WHITE, 0x07E0, TIER_3_MEDIUM},     // 32 - Orange/Weiss/Gruen
  {"Australien", "AUS", 0xFE60, 0x07E0, 0xFE60, TIER_3_MEDIUM},             // 33 - Gold/Gruen/Gold
  {"Schottland", "SCO", 0x001F, TFT_WHITE, 0xF800, TIER_3_MEDIUM},          // 34 - Blau/Weiss/Rot
  {"Korea", "KOR", 0xF800, 0xF800, 0xF800, TIER_3_MEDIUM},                  // 35 - Rot/Rot/Rot

  // ===== TIER 4 - SCHWACH (14 Sekunden) - 12 Teams =====
  // Index 36-47
  {"Haiti", "HAI", 0x001F, 0xF800, 0x001F, TIER_4_WEAK},                    // 36 - Blau/Rot/Blau
  {"Panama", "PAN", 0xF800, 0xF800, 0xF800, TIER_4_WEAK},                   // 37 - Rot/Rot/Rot
  {"Cabo Verde", "CPV", 0x001F, TFT_WHITE, 0x001F, TIER_4_WEAK},            // 38 - Blau/Weiss/Blau
  {"Neuseeland", "NZL", TFT_WHITE, 0x0000, TFT_WHITE, TIER_4_WEAK},         // 39 - Weiss/Schwarz/Weiss
  {"Curacao", "CUR", 0x001F, 0x001F, 0x001F, TIER_4_WEAK},                  // 40 - Blau/Blau/Blau
  {"Suedafrika", "RSA", 0x07E0, TFT_WHITE, 0xFE60, TIER_4_WEAK},            // 41 - Gruen/Weiss/Gelb
  {"Paraguay", "PAR", 0xF800, 0x001F, 0x001F, TIER_4_WEAK},                 // 42 - Rot/Blau/Blau
  {"Bolivien", "BOL", 0x07E0, TFT_WHITE, 0x07E0, TIER_4_WEAK},              // 43 - Gruen/Weiss/Gruen
  {"Jamaika", "JAM", 0xFE60, 0x0000, 0x07E0, TIER_4_WEAK},                  // 44 - Gelb/Schwarz/Gruen
  {"Jordanien", "JOR", 0xF800, TFT_WHITE, 0xF800, TIER_4_WEAK},             // 45 - Rot/Weiss/Rot
  {"Usbekistan", "UZB", TFT_WHITE, TFT_WHITE, TFT_WHITE, TIER_4_WEAK},      // 46 - Weiss/Weiss/Weiss
  {"Daenemark", "DEN", 0xF800, TFT_WHITE, 0xF800, TIER_4_WEAK}              // 47 - Rot/Weiss/Rot
};

const int WM_TEAM_COUNT = 48;

// Index von Deutschland (Standard-Auswahl)
#define WM_TEAM_GERMANY 0

// Hilfsfunktion: Antwortzeit basierend auf Tier
inline unsigned long getAnswerTimeForTier(TeamTier tier) {
  switch (tier) {
    case TIER_1_ELITE:  return WM_ANSWER_TIME_TIER1;
    case TIER_2_STRONG: return WM_ANSWER_TIME_TIER2;
    case TIER_3_MEDIUM: return WM_ANSWER_TIME_TIER3;
    case TIER_4_WEAK:   return WM_ANSWER_TIME_TIER4;
    default:            return WM_ANSWER_TIME_TIER3;
  }
}

// Hilfsfunktion: Elfmeter-Wahrscheinlichkeit basierend auf Tier
inline int getPenaltyProbForTier(TeamTier tier) {
  switch (tier) {
    case TIER_1_ELITE:  return WM_PENALTY_PROB_TIER1;
    case TIER_2_STRONG: return WM_PENALTY_PROB_TIER2;
    case TIER_3_MEDIUM: return WM_PENALTY_PROB_TIER3;
    case TIER_4_WEAK:   return WM_PENALTY_PROB_TIER4;
    default:            return WM_PENALTY_PROB_TIER3;
  }
}

#endif
