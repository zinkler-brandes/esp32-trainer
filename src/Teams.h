#ifndef SRC_TEAMS_H
#define SRC_TEAMS_H

#include <Arduino.h>

// Turnier-Typen
enum TournamentType {
  TOURNAMENT_DFB_POKAL,
  TOURNAMENT_CHAMPIONS_LEAGUE
};

// Konstanten für Turnier
#define STEPS_FOR_GOAL 3  // Antworten für ein Tor

// Antwortzeiten in Millisekunden
#define ANSWER_TIME_EASY   12000  // 12 Sekunden
#define ANSWER_TIME_MEDIUM  8000  // 8 Sekunden
#define ANSWER_TIME_HARD    5000  // 5 Sekunden

// CPU Elfmeter-Trefferwahrscheinlichkeit (%)
#define CPU_PENALTY_PROB_EASY   50
#define CPU_PENALTY_PROB_MEDIUM 65
#define CPU_PENALTY_PROB_HARD   80

// Rundenanzahl
#define DFB_ROUNDS 6
#define CL_ROUNDS  4

// Rundennamen
const char* const DFB_ROUND_NAMES[] = {
  "1. Runde",
  "2. Runde",
  "Achtelfinale",
  "Viertelfinale",
  "Halbfinale",
  "FINALE"
};

const char* const CL_ROUND_NAMES[] = {
  "Achtelfinale",
  "Viertelfinale",
  "Halbfinale",
  "FINALE"
};

// Team-Struktur für Gegner
struct Team {
  const char* name;
  uint16_t primaryColor;
  uint16_t secondaryColor;
};

// ========== DFB-POKAL Teams ==========

// Cluster für verschiedene Runden (nach Schwierigkeit)
// Runde 1-2: Kleine Vereine (leicht)
const Team DFB_CLUSTER_EARLY[] = {
  {"SSV Ulm", 0x0000, TFT_WHITE},
  {"SV Elversberg", 0xF800, TFT_WHITE},
  {"Preussen Muenster", 0x07E0, TFT_WHITE},
  {"Rot-Weiss Essen", 0xF800, TFT_WHITE},
  {"1. FC Saarbruecken", 0x001F, TFT_WHITE},
  {"Jahn Regensburg", 0xF800, TFT_WHITE}
};

// Runde 3-4: Mittelstarke Teams
const Team DFB_CLUSTER_MID[] = {
  {"1. FC Koeln", 0xF800, TFT_WHITE},
  {"Hertha BSC", 0x001F, TFT_WHITE},
  {"FC Schalke 04", 0x001F, TFT_WHITE},
  {"Hamburger SV", 0x001F, TFT_WHITE},
  {"Fortuna Duesseldorf", 0xF800, TFT_WHITE},
  {"Hannover 96", 0x07E0, TFT_WHITE}
};

// Runde 5-6: Bundesliga-Spitze
const Team DFB_CLUSTER_LATE[] = {
  {"Borussia Dortmund", 0xFE60, 0x0000},
  {"RB Leipzig", 0xF800, TFT_WHITE},
  {"Bayer Leverkusen", 0xF800, 0x0000},
  {"VfB Stuttgart", 0xF800, TFT_WHITE},
  {"Eintracht Frankfurt", 0x0000, 0xF800},
  {"Bayern Muenchen", 0xF800, TFT_WHITE}
};

// Cluster-Array für DFB (pro Runde)
const Team* const DFB_CLUSTERS[] = {
  DFB_CLUSTER_EARLY,  // Runde 0: 1. Runde
  DFB_CLUSTER_EARLY,  // Runde 1: 2. Runde
  DFB_CLUSTER_MID,    // Runde 2: Achtelfinale
  DFB_CLUSTER_MID,    // Runde 3: Viertelfinale
  DFB_CLUSTER_LATE,   // Runde 4: Halbfinale
  DFB_CLUSTER_LATE    // Runde 5: Finale
};

const int DFB_CLUSTER_SIZES[] = {6, 6, 6, 6, 6, 6};

// ========== CHAMPIONS LEAGUE Teams ==========

// CL Cluster (nur starke Teams)
const Team CL_CLUSTER_QUARTER[] = {
  {"FC Porto", 0x001F, TFT_WHITE},
  {"Benfica Lissabon", 0xF800, TFT_WHITE},
  {"Ajax Amsterdam", 0xF800, TFT_WHITE},
  {"Celtic Glasgow", 0x07E0, TFT_WHITE},
  {"AC Milan", 0xF800, 0x0000},
  {"Atletico Madrid", 0xF800, TFT_WHITE}
};

const Team CL_CLUSTER_SEMI[] = {
  {"Real Madrid", TFT_WHITE, 0xFE60},
  {"FC Barcelona", 0x001F, 0xF800},
  {"Manchester City", 0x5D7F, TFT_WHITE},
  {"Liverpool FC", 0xF800, TFT_WHITE},
  {"Paris SG", 0x001F, 0xF800},
  {"Juventus Turin", 0x0000, TFT_WHITE}
};

const Team CL_CLUSTER_FINAL[] = {
  {"Real Madrid", TFT_WHITE, 0xFE60},
  {"FC Barcelona", 0x001F, 0xF800},
  {"Manchester City", 0x5D7F, TFT_WHITE},
  {"Bayern Muenchen", 0xF800, TFT_WHITE}
};

// Cluster-Array für CL (pro Runde)
const Team* const CL_CLUSTERS[] = {
  CL_CLUSTER_QUARTER,  // Runde 0: Achtelfinale
  CL_CLUSTER_QUARTER,  // Runde 1: Viertelfinale
  CL_CLUSTER_SEMI,     // Runde 2: Halbfinale
  CL_CLUSTER_FINAL     // Runde 3: Finale
};

const int CL_CLUSTER_SIZES[] = {6, 6, 6, 4};

#endif
