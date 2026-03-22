#ifndef SRC_STADIEN_H
#define SRC_STADIEN_H

#include <Arduino.h>

// Stadion-Datenstruktur
struct Stadion {
  const char* name;       // Stadionname
  int clubIndex;          // Index in BUNDESLIGA_CLUBS
};

// Stadien der 1.-3. Liga (passend zu BundesligaClubs.h Indizes)
const Stadion STADIEN[] = {
  // ===== 1. BUNDESLIGA =====
  {"WWK Arena",              0},   // FC Augsburg
  {"Alte Foersterei",        1},   // Union Berlin
  {"Vonovia Ruhrstadion",    2},   // VfL Bochum
  {"Weserstadion",           3},   // Werder Bremen
  {"Signal Iduna Park",      4},   // Borussia Dortmund
  {"Deutsche Bank Park",     5},   // Eintracht Frankfurt
  {"Europa-Park Stadion",    6},   // SC Freiburg
  {"Voith-Arena",            7},   // 1. FC Heidenheim
  {"PreZero Arena",          8},   // TSG Hoffenheim
  {"Holstein-Stadion",       9},   // Holstein Kiel
  {"Red Bull Arena",         10},  // RB Leipzig
  {"BayArena",               11},  // Bayer Leverkusen
  {"Mewa Arena",             12},  // 1. FSV Mainz 05
  {"Borussia-Park",          13},  // Bor. M'gladbach
  {"Allianz Arena",          14},  // FC Bayern Muenchen
  {"Millerntor-Stadion",     15},  // FC St. Pauli
  {"MHP Arena",              16},  // VfB Stuttgart
  {"Volkswagen Arena",       17},  // VfL Wolfsburg

  // ===== 2. BUNDESLIGA =====
  {"Eintracht-Stadion",      18},  // Eintracht Braunschweig
  {"Merck-Stadion",          19},  // SV Darmstadt 98
  {"Merkur Spiel-Arena",     20},  // Fortuna Duesseldorf
  {"Ursapharm-Arena",        21},  // SV Elversberg
  {"Sportpark Ronhof",       22},  // Greuther Fuerth
  {"Volksparkstadion",       23},  // Hamburger SV
  {"Heinz-v.-Heiden-Arena",  24},  // Hannover 96
  {"Olympiastadion",         25},  // Hertha BSC
  {"Fritz-Walter-Stadion",   26},  // 1. FC Kaiserslautern
  {"BBBank Wildpark",        27},  // Karlsruher SC
  {"RheinEnergieStadion",    28},  // 1. FC Koeln
  {"MDCC-Arena",             29},  // 1. FC Magdeburg
  {"Preussenstadion",        30},  // Preussen Muenster
  {"Max-Morlock-Stadion",    31},  // 1. FC Nuernberg
  {"Home Deluxe Arena",      32},  // SC Paderborn 07
  {"Jahnstadion",            33},  // Jahn Regensburg
  {"Veltins-Arena",          34},  // FC Schalke 04
  {"Donaustadion",           35},  // SSV Ulm 1846

  // ===== 3. LIGA =====
  {"Tivoli",                 36},  // Alemannia Aachen
  {"Erzgebirgsstadion",      37},  // Erzgebirge Aue
  {"Schueco Arena",          38},  // Arminia Bielefeld
  {"Stadion der Freundsch.", 39},  // Energie Cottbus
  {"Rudolf-Harbig-Stadion",  40},  // Dynamo Dresden
  {"Schauinsland-Arena",     41},  // MSV Duisburg
  {"Audi Sportpark",         42},  // FC Ingolstadt 04
  {"Sportpark Hoehenberg",   43},  // Viktoria Koeln
  {"Carl-Benz-Stadion",      44},  // Waldhof Mannheim
  {"Gruenwalder Stadion",    45},  // 1860 Muenchen
  {"Bremer Bruecke",         46},  // VfL Osnabrueck
  {"Ostseestadion",          47},  // Hansa Rostock
  {"Stadion Essen",          48},  // Rot-Weiss Essen
  {"Ludwigsparkstadion",     49},  // 1. FC Saarbruecken
  {"BWT-Stadion",            50},  // SV Sandhausen
  {"Alpenbauer Sportpark",   51},  // SpVgg Unterhaching
  {"Sportclub Arena",        52},  // SC Verl
  {"Brita-Arena",            53},  // Wehen Wiesbaden
};

const int STADION_COUNT = sizeof(STADIEN) / sizeof(Stadion);

#endif
