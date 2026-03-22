#ifndef SRC_WM2026GROUPS_H
#define SRC_WM2026GROUPS_H

#include "WM2026Teams.h"

// Gruppenstruktur
struct WMGroup {
  char letter;           // 'A' - 'L'
  int teamIndices[4];    // Indices in WM_TEAMS Array
};

// ========== DIE 12 WM 2026 GRUPPEN ==========
// Basierend auf der offiziellen FIFA Auslosung

const WMGroup WM_GROUPS[] = {
  // Gruppe A: Mexiko, Suedafrika, Korea, Daenemark
  {'A', {12, 41, 35, 47}},

  // Gruppe B: Kanada, Italien, Katar, Schweiz
  {'B', {24, 21, 25, 14}},

  // Gruppe C: Brasilien, Marokko, Haiti, Schottland
  {'C', {5, 16, 36, 34}},

  // Gruppe D: USA, Paraguay, Australien, Tuerkei
  {'D', {13, 42, 33, 23}},

  // Gruppe E: Deutschland, Curacao, Elfenbeinkueste, Ecuador  <-- Deutschlands Gruppe!
  {'E', {0, 40, 32, 18}},

  // Gruppe F: Niederlande, Japan, Ukraine, Tunesien
  {'F', {7, 15, 22, 30}},

  // Gruppe G: Belgien, Aegypten, Iran, Neuseeland
  {'G', {8, 27, 28, 39}},

  // Gruppe H: Spanien, Cabo Verde, Saudi-Arabien, Uruguay
  {'H', {1, 38, 29, 10}},

  // Gruppe I: Frankreich, Senegal, Bolivien, Norwegen
  {'I', {3, 17, 43, 20}},

  // Gruppe J: Argentinien, Algerien, Oesterreich, Jordanien
  {'J', {4, 26, 19, 45}},

  // Gruppe K: Portugal, Jamaika, Usbekistan, Kolumbien
  {'K', {6, 44, 46, 11}},

  // Gruppe L: England, Kroatien, Ghana, Panama
  {'L', {2, 9, 31, 37}}
};

const int WM_GROUP_COUNT = 12;

// Deutschlands Gruppe
#define WM_GROUP_GERMANY 4  // Index der Gruppe E

// Hilfsfunktion: Finde Gruppe eines Teams
inline int findGroupForTeam(int teamIndex) {
  for (int g = 0; g < WM_GROUP_COUNT; g++) {
    for (int t = 0; t < 4; t++) {
      if (WM_GROUPS[g].teamIndices[t] == teamIndex) {
        return g;
      }
    }
  }
  return -1;  // Team nicht gefunden
}

// Hilfsfunktion: Gruppenbuchstabe
inline char getGroupLetter(int groupIndex) {
  if (groupIndex >= 0 && groupIndex < WM_GROUP_COUNT) {
    return WM_GROUPS[groupIndex].letter;
  }
  return '?';
}

#endif
