#ifndef SRC_WM2026VENUES_H
#define SRC_WM2026VENUES_H

#include <Arduino.h>

struct WMVenue {
  const char* city;
  const char* stadium;
  const char* country;    // "USA", "CAN", "MEX"
};

// ========== 16 WM 2026 AUSTRAGUNGSORTE ==========

const WMVenue WM_VENUES[] = {
  // === USA (11 Staedte) ===
  {"New York", "MetLife Stadium", "USA"},              // 0 - FINALE!
  {"Los Angeles", "SoFi Stadium", "USA"},              // 1
  {"Dallas", "AT&T Stadium", "USA"},                   // 2
  {"Miami", "Hard Rock Stadium", "USA"},               // 3
  {"Atlanta", "Mercedes-Benz Stadium", "USA"},         // 4
  {"Seattle", "Lumen Field", "USA"},                   // 5
  {"San Francisco", "Levi's Stadium", "USA"},          // 6
  {"Houston", "NRG Stadium", "USA"},                   // 7
  {"Boston", "Gillette Stadium", "USA"},               // 8
  {"Philadelphia", "Lincoln Financial Field", "USA"},  // 9
  {"Kansas City", "Arrowhead Stadium", "USA"},         // 10

  // === Kanada (2 Staedte) ===
  {"Toronto", "BMO Field", "CAN"},                     // 11
  {"Vancouver", "BC Place", "CAN"},                    // 12

  // === Mexiko (3 Staedte) ===
  {"Mexico City", "Estadio Azteca", "MEX"},            // 13 - Eroeffnungsspiel!
  {"Guadalajara", "Estadio Akron", "MEX"},             // 14
  {"Monterrey", "Estadio BBVA", "MEX"}                 // 15
};

const int WM_VENUE_COUNT = 16;

// Wichtige Stadien-Indices
#define WM_VENUE_FINALE        0   // MetLife Stadium, New York
#define WM_VENUE_OPENING       13  // Estadio Azteca, Mexico City

// Regionen fuer Gruppenspiele
enum VenueRegion {
  REGION_WEST,     // Vancouver, Seattle, San Francisco, Los Angeles
  REGION_CENTRAL,  // Guadalajara, Mexico City, Monterrey, Houston, Dallas, Kansas City
  REGION_EAST      // Atlanta, Miami, Toronto, Boston, Philadelphia, New York
};

// Stadien pro Region
const int REGION_WEST_VENUES[] = {12, 5, 6, 1};        // Vancouver, Seattle, SF, LA
const int REGION_WEST_COUNT = 4;

const int REGION_CENTRAL_VENUES[] = {14, 13, 15, 7, 2, 10};  // Guadalajara, Mexico City, Monterrey, Houston, Dallas, KC
const int REGION_CENTRAL_COUNT = 6;

const int REGION_EAST_VENUES[] = {4, 3, 11, 8, 9, 0};  // Atlanta, Miami, Toronto, Boston, Philly, NY
const int REGION_EAST_COUNT = 6;

// Hilfsfunktion: Zufaelliges Stadion aus Region
inline int getRandomVenueInRegion(VenueRegion region) {
  switch (region) {
    case REGION_WEST:
      return REGION_WEST_VENUES[random(0, REGION_WEST_COUNT)];
    case REGION_CENTRAL:
      return REGION_CENTRAL_VENUES[random(0, REGION_CENTRAL_COUNT)];
    case REGION_EAST:
      return REGION_EAST_VENUES[random(0, REGION_EAST_COUNT)];
    default:
      return 0;
  }
}

// Hilfsfunktion: Region fuer Gruppe bestimmen
inline VenueRegion getRegionForGroup(int groupIndex) {
  // Gruppen A-D: Zentral/Ost
  // Gruppen E-H: West/Zentral
  // Gruppen I-L: Ost/Kanada
  if (groupIndex < 4) {
    return (random(0, 2) == 0) ? REGION_CENTRAL : REGION_EAST;
  } else if (groupIndex < 8) {
    return (random(0, 2) == 0) ? REGION_WEST : REGION_CENTRAL;
  } else {
    return (random(0, 2) == 0) ? REGION_EAST : REGION_CENTRAL;
  }
}

// Hilfsfunktion: Stadion fuer K.O.-Runde
inline int getVenueForKnockoutRound(int round, int matchIndex) {
  switch (round) {
    case 4:  // Finale
      return WM_VENUE_FINALE;  // MetLife Stadium, New York

    case 3:  // Halbfinale
      return (matchIndex == 0) ? 1 : 2;  // LA oder Dallas

    case 2:  // Viertelfinale
      return 3 + matchIndex;  // Miami, Atlanta, Seattle, SF

    case 1:  // Achtelfinale
      return REGION_EAST_VENUES[matchIndex % REGION_EAST_COUNT];

    case 0:  // Runde der 32
    default:
      return random(0, WM_VENUE_COUNT);
  }
}

#endif
