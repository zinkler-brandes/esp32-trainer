#ifndef SRC_TEAMS_H
#define SRC_TEAMS_H

#include <Arduino.h>
#include <TFT_eSPI.h>  // Für TFT_* Farb-Makros

// Turnier-Typen
enum TournamentType {
  TOURNAMENT_DFB_POKAL,
  TOURNAMENT_CHAMPIONS_LEAGUE,
  TOURNAMENT_WORLD_CUP_2026      // NEU: WM 2026 Modus
};

// Konstanten für Turnier
#define STEPS_FOR_GOAL 5  // Antworten für ein Tor

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
#define WC_KO_ROUNDS  5   // R32, R16, QF, SF, Final

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
  const char* abbrev;  // 3-stellige Abkürzung
  uint16_t primaryColor;
  uint16_t secondaryColor;
};

// ========== DFB-POKAL Teams ==========

// Runde 1: Amateure und Regionalliga (sehr leicht)
const Team DFB_ROUND_1[] = {
  {"SSV Ulm", "ULM", 0x0000, TFT_WHITE},
  {"Preussen Muenster", "SCP", 0x07E0, TFT_WHITE},
  {"Rot-Weiss Essen", "RWE", 0xF800, TFT_WHITE},
  {"Viktoria Koeln", "VIK", 0xF800, TFT_WHITE},
  {"Dynamo Dresden", "SGD", 0xFE60, 0x0000},
  {"Hansa Rostock", "FCH", 0x001F, TFT_WHITE},
  {"1860 Muenchen", "M60", 0x001F, TFT_WHITE},
  {"Wuerzburger Kickers", "FWK", 0xF800, TFT_WHITE},
  {"Waldhof Mannheim", "SVM", 0x001F, 0x0000},
  {"Energie Cottbus", "FCE", 0xF800, TFT_WHITE}
};

// Runde 2: 3. Liga und schwache 2. Liga (leicht)
const Team DFB_ROUND_2[] = {
  {"SV Elversberg", "SVE", 0xF800, TFT_WHITE},
  {"1. FC Saarbruecken", "FCS", 0x001F, TFT_WHITE},
  {"Jahn Regensburg", "SSV", 0xF800, TFT_WHITE},
  {"Wehen Wiesbaden", "SVW", 0xF800, 0x0000},
  {"SC Verl", "SCV", 0xF800, TFT_WHITE},
  {"Erzgebirge Aue", "AUE", 0x7817, TFT_WHITE},
  {"MSV Duisburg", "MSV", 0x001F, TFT_WHITE},
  {"Arminia Bielefeld", "DSC", 0x001F, TFT_WHITE},
  {"Eintracht Braunschweig", "EBS", 0xFE60, 0x001F},
  {"SpVgg Greuther Fuerth", "SGF", 0x07E0, TFT_WHITE}
};

// Achtelfinale: 2. Bundesliga (mittel)
const Team DFB_ROUND_3[] = {
  {"1. FC Koeln", "KOE", 0xF800, TFT_WHITE},
  {"Hertha BSC", "BSC", 0x001F, TFT_WHITE},
  {"FC Schalke 04", "S04", 0x001F, TFT_WHITE},
  {"Hamburger SV", "HSV", 0x001F, TFT_WHITE},
  {"Fortuna Duesseldorf", "F95", 0xF800, TFT_WHITE},
  {"Hannover 96", "H96", 0x07E0, TFT_WHITE},
  {"FC St. Pauli", "STP", 0x8410, TFT_WHITE},
  {"1. FC Nuernberg", "FCN", 0xF800, TFT_WHITE},
  {"Karlsruher SC", "KSC", 0x001F, TFT_WHITE},
  {"SC Paderborn", "SCP", 0x001F, TFT_WHITE}
};

// Viertelfinale: Untere Bundesliga (mittel-schwer)
const Team DFB_ROUND_4[] = {
  {"Werder Bremen", "SVW", 0x07E0, TFT_WHITE},
  {"FC Augsburg", "FCA", 0xF800, 0x07E0},
  {"1. FC Union Berlin", "FCU", 0xF800, TFT_WHITE},
  {"VfL Bochum", "BOC", 0x001F, TFT_WHITE},
  {"FSV Mainz 05", "M05", 0xF800, TFT_WHITE},
  {"1. FC Heidenheim", "FCH", 0xF800, 0x001F},
  {"SV Darmstadt", "D98", 0x001F, TFT_WHITE},
  {"Holstein Kiel", "KSV", 0x001F, TFT_WHITE}
};

// Halbfinale: Obere Bundesliga (schwer)
const Team DFB_ROUND_5[] = {
  {"Borussia Dortmund", "BVB", 0xFE60, 0x0000},
  {"RB Leipzig", "RBL", 0xF800, TFT_WHITE},
  {"Bayer Leverkusen", "B04", 0xF800, 0x0000},
  {"VfB Stuttgart", "VFB", 0xF800, TFT_WHITE},
  {"Eintracht Frankfurt", "SGE", 0x0000, 0xF800},
  {"VfL Wolfsburg", "WOB", 0x07E0, TFT_WHITE},
  {"Bor. Moenchengladbach", "BMG", 0x0000, 0x07E0},
  {"SC Freiburg", "SCF", 0xF800, 0x0000}
};

// Finale: Top-Teams (sehr schwer)
const Team DFB_ROUND_6[] = {
  {"Bayern Muenchen", "FCB", 0xF800, TFT_WHITE},
  {"Borussia Dortmund", "BVB", 0xFE60, 0x0000},
  {"RB Leipzig", "RBL", 0xF800, TFT_WHITE},
  {"Bayer Leverkusen", "B04", 0xF800, 0x0000},
  {"VfB Stuttgart", "VFB", 0xF800, TFT_WHITE},
  {"Eintracht Frankfurt", "SGE", 0x0000, 0xF800}
};

// Cluster-Array für DFB (pro Runde)
const Team* const DFB_CLUSTERS[] = {
  DFB_ROUND_1,  // Runde 0: 1. Runde
  DFB_ROUND_2,  // Runde 1: 2. Runde
  DFB_ROUND_3,  // Runde 2: Achtelfinale
  DFB_ROUND_4,  // Runde 3: Viertelfinale
  DFB_ROUND_5,  // Runde 4: Halbfinale
  DFB_ROUND_6   // Runde 5: Finale
};

const int DFB_CLUSTER_SIZES[] = {10, 10, 10, 8, 8, 6};

// ========== CHAMPIONS LEAGUE Teams ==========

// CL Cluster (nur starke Teams)
const Team CL_CLUSTER_QUARTER[] = {
  {"FC Porto", "POR", 0x001F, TFT_WHITE},
  {"Benfica Lissabon", "SLB", 0xF800, TFT_WHITE},
  {"Ajax Amsterdam", "AJA", 0xF800, TFT_WHITE},
  {"Celtic Glasgow", "CEL", 0x07E0, TFT_WHITE},
  {"AC Milan", "ACM", 0xF800, 0x0000},
  {"Atletico Madrid", "ATM", 0xF800, TFT_WHITE}
};

const Team CL_CLUSTER_SEMI[] = {
  {"Real Madrid", "RMA", TFT_WHITE, 0xFE60},
  {"FC Barcelona", "FCB", 0x001F, 0xF800},
  {"Manchester City", "MCI", 0x5D7F, TFT_WHITE},
  {"Liverpool FC", "LFC", 0xF800, TFT_WHITE},
  {"Paris SG", "PSG", 0x001F, 0xF800},
  {"Juventus Turin", "JUV", 0x0000, TFT_WHITE}
};

const Team CL_CLUSTER_FINAL[] = {
  {"Real Madrid", "RMA", TFT_WHITE, 0xFE60},
  {"FC Barcelona", "FCB", 0x001F, 0xF800},
  {"Manchester City", "MCI", 0x5D7F, TFT_WHITE},
  {"Bayern Muenchen", "FCB", 0xF800, TFT_WHITE}
};

// Cluster-Array für CL (pro Runde)
const Team* const CL_CLUSTERS[] = {
  CL_CLUSTER_QUARTER,  // Runde 0: Achtelfinale
  CL_CLUSTER_QUARTER,  // Runde 1: Viertelfinale
  CL_CLUSTER_SEMI,     // Runde 2: Halbfinale
  CL_CLUSTER_FINAL     // Runde 3: Finale
};

const int CL_CLUSTER_SIZES[] = {6, 6, 6, 4};

// ========== AUSWÄHLBARE TEAMS (für Spieler) ==========

// DFB-Pokal: 1.-3. Liga Teams zur Auswahl
const Team DFB_SELECTABLE_TEAMS[] = {
  // 1. Bundesliga
  {"Bayern Muenchen", "FCB", 0xF800, TFT_WHITE},
  {"Borussia Dortmund", "BVB", 0xFE60, 0x0000},
  {"RB Leipzig", "RBL", 0xF800, TFT_WHITE},
  {"Bayer Leverkusen", "B04", 0xF800, 0x0000},
  {"VfB Stuttgart", "VFB", 0xF800, TFT_WHITE},
  {"Eintracht Frankfurt", "SGE", 0x0000, 0xF800},
  {"VfL Wolfsburg", "WOB", 0x07E0, TFT_WHITE},
  {"Bor. Moenchengladbach", "BMG", 0x0000, 0x07E0},
  {"SC Freiburg", "SCF", 0xF800, 0x0000},
  {"Werder Bremen", "SVW", 0x07E0, TFT_WHITE},
  {"FC Augsburg", "FCA", 0xF800, 0x07E0},
  {"1. FC Union Berlin", "FCU", 0xF800, TFT_WHITE},
  {"VfL Bochum", "BOC", 0x001F, TFT_WHITE},
  {"FSV Mainz 05", "M05", 0xF800, TFT_WHITE},
  {"1. FC Heidenheim", "FCH", 0xF800, 0x001F},
  {"Holstein Kiel", "KSV", 0x001F, TFT_WHITE},
  {"SV Darmstadt", "D98", 0x001F, TFT_WHITE},
  {"TSG Hoffenheim", "TSG", 0x001F, TFT_WHITE},
  // 2. Bundesliga
  {"1. FC Koeln", "KOE", 0xF800, TFT_WHITE},
  {"Hertha BSC", "BSC", 0x001F, TFT_WHITE},
  {"FC Schalke 04", "S04", 0x001F, TFT_WHITE},
  {"Hamburger SV", "HSV", 0x001F, TFT_WHITE},
  {"Fortuna Duesseldorf", "F95", 0xF800, TFT_WHITE},
  {"Hannover 96", "H96", 0x07E0, TFT_WHITE},
  {"FC St. Pauli", "STP", 0x8410, TFT_WHITE},
  {"1. FC Nuernberg", "FCN", 0xF800, TFT_WHITE},
  {"Karlsruher SC", "KSC", 0x001F, TFT_WHITE},
  {"SC Paderborn", "SCP", 0x001F, TFT_WHITE},
  // 3. Liga
  {"SV Elversberg", "SVE", 0xF800, TFT_WHITE},
  {"1. FC Saarbruecken", "FCS", 0x001F, TFT_WHITE},
  {"Jahn Regensburg", "SSV", 0xF800, TFT_WHITE},
  {"Arminia Bielefeld", "DSC", 0x001F, TFT_WHITE},
  {"Eintracht Braunschweig", "EBS", 0xFE60, 0x001F},
  {"1860 Muenchen", "M60", 0x001F, TFT_WHITE},
  {"Dynamo Dresden", "SGD", 0xFE60, 0x0000}
};
const int DFB_SELECTABLE_COUNT = 35;

// Champions League: Alle Teams (deutsche zuerst)
const Team CL_SELECTABLE_TEAMS[] = {
  // Deutsche Teams zuerst
  {"Bayern Muenchen", "FCB", 0xF800, TFT_WHITE},
  {"Borussia Dortmund", "BVB", 0xFE60, 0x0000},
  {"RB Leipzig", "RBL", 0xF800, TFT_WHITE},
  {"Bayer Leverkusen", "B04", 0xF800, 0x0000},
  {"VfB Stuttgart", "VFB", 0xF800, TFT_WHITE},
  {"Eintracht Frankfurt", "SGE", 0x0000, 0xF800},
  // Internationale Top-Teams
  {"Real Madrid", "RMA", TFT_WHITE, 0xFE60},
  {"FC Barcelona", "FCB", 0x001F, 0xF800},
  {"Manchester City", "MCI", 0x5D7F, TFT_WHITE},
  {"Liverpool FC", "LFC", 0xF800, TFT_WHITE},
  {"Paris SG", "PSG", 0x001F, 0xF800},
  {"Juventus Turin", "JUV", 0x0000, TFT_WHITE},
  {"AC Milan", "ACM", 0xF800, 0x0000},
  {"Inter Mailand", "INT", 0x001F, 0x0000},
  {"Atletico Madrid", "ATM", 0xF800, TFT_WHITE},
  {"FC Porto", "POR", 0x001F, TFT_WHITE},
  {"Benfica Lissabon", "SLB", 0xF800, TFT_WHITE},
  {"Ajax Amsterdam", "AJA", 0xF800, TFT_WHITE},
  {"Celtic Glasgow", "CEL", 0x07E0, TFT_WHITE},
  {"Manchester United", "MUN", 0xF800, TFT_WHITE},
  {"FC Chelsea", "CHE", 0x001F, TFT_WHITE},
  {"Arsenal London", "ARS", 0xF800, TFT_WHITE}
};
const int CL_SELECTABLE_COUNT = 22;

#endif
