#ifndef SRC_WM2026SCHEDULE_H
#define SRC_WM2026SCHEDULE_H

#include <Arduino.h>

// Spieltermin-Struktur
struct WMMatchDate {
  uint8_t day;
  uint8_t month;
  uint16_t year;
  uint8_t hourLocal;    // Lokale Zeit (USA/MEX/CAN)
  uint8_t minuteLocal;
  int8_t timezoneOffset; // Differenz zu deutscher Zeit in Stunden
  const char* weekday;   // Wochentag auf Deutsch
};

// WM 2026: 11. Juni - 19. Juli 2026
// Gruppenphase: 11. Juni - 28. Juni
// K.O.-Phase: 29. Juni - 19. Juli

// Gruppenphasen-Spieltage (3 Spieltage pro Gruppe)
// Gruppe E (Deutschland): Spieltage ca. 13., 18., 23. Juni

// Spieltermine fuer Gruppenphase - je 3 Spiele pro Gruppe
// Index = groupIndex * 3 + matchDay (0-2)
const WMMatchDate WM_GROUP_DATES[] = {
  // Gruppe A - Mexiko, Suedafrika, Korea, Daenemark
  {11, 6, 2026, 17, 0, -7, "Donnerstag"},  // Spieltag 1 - Eroeffnung Azteca
  {16, 6, 2026, 13, 0, -7, "Dienstag"},
  {21, 6, 2026, 19, 0, -7, "Sonntag"},

  // Gruppe B - Kanada, Italien, Katar, Schweiz
  {12, 6, 2026, 14, 0, -5, "Freitag"},
  {17, 6, 2026, 16, 0, -5, "Mittwoch"},
  {22, 6, 2026, 13, 0, -5, "Montag"},

  // Gruppe C - Brasilien, Marokko, Haiti, Schottland
  {12, 6, 2026, 20, 0, -5, "Freitag"},
  {17, 6, 2026, 19, 0, -5, "Mittwoch"},
  {22, 6, 2026, 16, 0, -5, "Montag"},

  // Gruppe D - USA, Paraguay, Australien, Tuerkei
  {13, 6, 2026, 11, 0, -8, "Samstag"},
  {18, 6, 2026, 14, 0, -8, "Donnerstag"},
  {23, 6, 2026, 17, 0, -8, "Dienstag"},

  // Gruppe E - Deutschland, Curacao, Elfenbeinkueste, Ecuador
  {13, 6, 2026, 18, 0, -6, "Samstag"},     // Deutschland vs Curacao - Houston
  {18, 6, 2026, 15, 0, -6, "Donnerstag"},  // Deutschland vs Elfenbeinkueste - Dallas
  {23, 6, 2026, 20, 0, -6, "Dienstag"},    // Deutschland vs Ecuador - Kansas City

  // Gruppe F - Niederlande, Japan, Ukraine, Tunesien
  {14, 6, 2026, 12, 0, -8, "Sonntag"},
  {19, 6, 2026, 15, 0, -8, "Freitag"},
  {24, 6, 2026, 18, 0, -8, "Mittwoch"},

  // Gruppe G - Belgien, Aegypten, Iran, Neuseeland
  {14, 6, 2026, 15, 0, -5, "Sonntag"},
  {19, 6, 2026, 18, 0, -5, "Freitag"},
  {24, 6, 2026, 14, 0, -5, "Mittwoch"},

  // Gruppe H - Spanien, Cabo Verde, Saudi-Arabien, Uruguay
  {14, 6, 2026, 21, 0, -5, "Sonntag"},
  {19, 6, 2026, 21, 0, -5, "Freitag"},
  {24, 6, 2026, 21, 0, -5, "Mittwoch"},

  // Gruppe I - Frankreich, Senegal, Bolivien, Norwegen
  {15, 6, 2026, 14, 0, -5, "Montag"},
  {20, 6, 2026, 17, 0, -5, "Samstag"},
  {25, 6, 2026, 14, 0, -5, "Donnerstag"},

  // Gruppe J - Argentinien, Algerien, Oesterreich, Jordanien
  {15, 6, 2026, 20, 0, -5, "Montag"},
  {20, 6, 2026, 14, 0, -5, "Samstag"},
  {25, 6, 2026, 17, 0, -5, "Donnerstag"},

  // Gruppe K - Portugal, Jamaika, Usbekistan, Kolumbien
  {16, 6, 2026, 11, 0, -8, "Dienstag"},
  {21, 6, 2026, 14, 0, -8, "Sonntag"},
  {26, 6, 2026, 17, 0, -8, "Freitag"},

  // Gruppe L - England, Kroatien, Ghana, Panama
  {16, 6, 2026, 17, 0, -5, "Dienstag"},
  {21, 6, 2026, 20, 0, -5, "Sonntag"},
  {26, 6, 2026, 14, 0, -5, "Freitag"}
};

// K.O.-Runden Termine
const WMMatchDate WM_KNOCKOUT_DATES[] = {
  // Runde der 32 (29. Juni - 2. Juli)
  {29, 6, 2026, 14, 0, -5, "Montag"},
  {30, 6, 2026, 17, 0, -5, "Dienstag"},
  {1, 7, 2026, 20, 0, -5, "Mittwoch"},
  {2, 7, 2026, 14, 0, -5, "Donnerstag"},

  // Achtelfinale (3. - 6. Juli)
  {3, 7, 2026, 17, 0, -5, "Freitag"},
  {4, 7, 2026, 14, 0, -5, "Samstag"},
  {5, 7, 2026, 20, 0, -5, "Sonntag"},
  {6, 7, 2026, 17, 0, -5, "Montag"},

  // Viertelfinale (9. - 10. Juli)
  {9, 7, 2026, 17, 0, -5, "Donnerstag"},
  {10, 7, 2026, 20, 0, -5, "Freitag"},

  // Halbfinale (14. - 15. Juli)
  {14, 7, 2026, 20, 0, -8, "Dienstag"},   // Los Angeles
  {15, 7, 2026, 20, 0, -6, "Mittwoch"},   // Dallas

  // Finale (19. Juli)
  {19, 7, 2026, 16, 0, -5, "Sonntag"}     // New York - 16:00 lokal = 22:00 deutsch
};

// Hilfsfunktion: Deutsche Zeit berechnen
inline void getGermanTime(const WMMatchDate& match, uint8_t& hour, uint8_t& minute) {
  // Deutsche Zeit = Lokalzeit + abs(timezoneOffset) + 1 (Sommerzeit)
  // z.B. -6 Stunden -> +7 Stunden zur deutschen Zeit
  int germanHour = match.hourLocal + (-match.timezoneOffset) + 1;  // +1 fuer Sommerzeit

  // Tagesüberlauf behandeln
  if (germanHour >= 24) {
    germanHour -= 24;
  }

  hour = germanHour;
  minute = match.minuteLocal;
}

// Hilfsfunktion: Spieltermin fuer Gruppenspiel holen
inline WMMatchDate getGroupMatchDate(int groupIndex, int matchDay) {
  int idx = groupIndex * 3 + matchDay;
  if (idx >= 0 && idx < 36) {
    return WM_GROUP_DATES[idx];
  }
  // Fallback
  return {1, 1, 2026, 12, 0, 0, "?"};
}

// Hilfsfunktion: Spieltermin fuer K.O.-Runde holen
inline WMMatchDate getKnockoutMatchDate(int round, int matchIndex) {
  int idx = 0;
  switch (round) {
    case 0: idx = matchIndex % 4; break;      // R32
    case 1: idx = 4 + (matchIndex % 4); break; // R16
    case 2: idx = 8 + (matchIndex % 2); break; // QF
    case 3: idx = 10 + (matchIndex % 2); break; // SF
    case 4: idx = 12; break;                   // Finale
  }
  if (idx >= 0 && idx < 13) {
    return WM_KNOCKOUT_DATES[idx];
  }
  return {1, 1, 2026, 12, 0, 0, "?"};
}

#endif
