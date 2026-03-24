#ifndef SRC_PROFILE_H
#define SRC_PROFILE_H

#include <Arduino.h>

// Lieblingsteam-Struktur
struct FavoriteTeam {
  String name;
  String abbrev;  // 3-stellige Abkürzung
  uint16_t primaryColor;
  uint16_t secondaryColor;

  FavoriteTeam() : name(""), abbrev(""), primaryColor(0), secondaryColor(0) {}
  FavoriteTeam(const String& n, const String& a, uint16_t pc, uint16_t sc)
    : name(n), abbrev(a), primaryColor(pc), secondaryColor(sc) {}

  bool isSet() const { return name.length() > 0; }
};

// Rekordematrix: 3 Turniere x 4 Rechenarten
// Zeigt welche Kombinationen schon gewonnen wurden
struct RecordsMatrix {
  // [turnier][rechenart]
  // turnier: 0=DFB, 1=CL, 2=WM
  // rechenart: 0=+/-, 1=1x1, 2=1:1, 3=Alle
  bool won[3][4];

  RecordsMatrix() {
    for (int t = 0; t < 3; t++) {
      for (int m = 0; m < 4; m++) {
        won[t][m] = false;
      }
    }
  }

  void setWon(int tournament, int matheMode) {
    if (tournament >= 0 && tournament < 3 && matheMode >= 0 && matheMode < 4) {
      won[tournament][matheMode] = true;
    }
  }

  bool isWon(int tournament, int matheMode) const {
    if (tournament >= 0 && tournament < 3 && matheMode >= 0 && matheMode < 4) {
      return won[tournament][matheMode];
    }
    return false;
  }

  int countWins() const {
    int count = 0;
    for (int t = 0; t < 3; t++) {
      for (int m = 0; m < 4; m++) {
        if (won[t][m]) count++;
      }
    }
    return count;
  }

  bool isComplete() const {
    return countWins() == 12;  // 3x4 = 12
  }
};

// Bit-Konstanten fuer seenIntros
#define INTRO_SCHREIBEN      (1 << 0)
#define INTRO_FLAG_QUIZ      (1 << 1)
#define INTRO_CLUB_QUIZ      (1 << 2)
#define INTRO_STADION_QUIZ   (1 << 3)
#define INTRO_KENNZEICHEN    (1 << 4)
#define INTRO_EINZELSPIEL    (1 << 5)
#define INTRO_DFB_POKAL      (1 << 6)
#define INTRO_CL             (1 << 7)
#define INTRO_WM             (1 << 8)

struct Profile {
  int id;
  String name;
  FavoriteTeam favTeamDFB;  // Lieblingsteam fuer DFB-Pokal
  FavoriteTeam favTeamCL;   // Lieblingsteam fuer Champions League
  FavoriteTeam favTeamWC;   // Lieblingsteam fuer WM 2026
  RecordsMatrix records;    // Rekordematrix
  int flagQuizBestMedal;    // Beste Medaille beim Flaggenraten (0-3)
  int clubQuizBestMedal;    // Beste Medaille beim Vereinsquiz (0-3)
  int stadionQuizBestMedal; // Beste Medaille beim Stadionquiz (0-3)
  int kennzeichenQuizBestMedal; // Beste Medaille beim Kennzeichenquiz (0-3)
  uint16_t seenIntros;      // Bitfeld: welche Intros schon gesehen

  Profile() : id(0), name(""), flagQuizBestMedal(0), clubQuizBestMedal(0),
              stadionQuizBestMedal(0), kennzeichenQuizBestMedal(0), seenIntros(0) {}
  Profile(int _id, const String& _name) : id(_id), name(_name), flagQuizBestMedal(0),
              clubQuizBestMedal(0), stadionQuizBestMedal(0), kennzeichenQuizBestMedal(0), seenIntros(0) {}
};

#endif
