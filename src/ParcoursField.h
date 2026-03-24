#ifndef SRC_PARCOURS_FIELD_H
#define SRC_PARCOURS_FIELD_H

#include <Arduino.h>
#include <TFT_eSPI.h>

// Farben für den Parcours
#define SAND_COLOR 0xD69A      // Sandiger Reitplatz-Untergrund
#define SAND_DARK 0xC618       // Dunklerer Sand für Kontrast
#define FENCE_WHITE TFT_WHITE   // Weiße Stangen
#define FENCE_RED 0xF800        // Rote Stangen
#define HORSE_BROWN 0x8200      // Dunkelbraun
#define HORSE_LIGHT 0xA285      // Helleres Braun für Bauch
#define RIDER_BLUE 0x0010       // Dunkelblaue Reitjacke

// Hindernis-Typen
enum ObstacleType {
  OBSTACLE_OXER,       // Breites Hindernis (2 Stangen hintereinander)
  OBSTACLE_VERTICAL,   // Einzelne hohe Stange
  OBSTACLE_WALL,       // Mauer
  OBSTACLE_WATER       // Wassergraben
};

// Hindernis-Status
enum ObstacleStatus {
  OBSTACLE_PENDING,    // Noch nicht gesprungen
  OBSTACLE_CLEARED,    // Erfolgreich übersprungen
  OBSTACLE_KNOCKED     // Abgeworfen (4 Fehler)
};

class ParcoursField {
  public:
    ParcoursField();

    // TFT-Display setzen
    void setDisplay(TFT_eSPI* display);

    // Spieler-Daten
    void setRiderName(const String& name);
    void setHorseName(const String& name);
    void setTournamentName(const String& name);
    void setFaults(int faults);
    void setTotalTime(float seconds);
    void setObstacleCount(int count);
    void setCurrentObstacle(int obstacle);
    void setObstacleStatus(int obstacle, ObstacleStatus status);

    // Parcours zeichnen
    void drawParcours();
    void drawScoreboard();
    void drawHorseAndRider(int position, bool jumping = false);
    void drawObstacle(int index, ObstacleType type, ObstacleStatus status);
    void drawAllObstacles();

    // Animationen
    void showJumpAnimation(int obstacleIndex);       // Erfolgreicher Sprung
    void showKnockdownAnimation(int obstacleIndex);  // Stange fällt
    void showRefusalAnimation(int obstacleIndex);    // Pferd verweigert
    void showRoundComplete(int faults, float totalTime);
    void showStechen(int stechenNum);               // Stechen-Intro
    void showChampionCelebration();                 // Ehrenrunde bei Sieg
    void showJubilatingSpectators(int frame);       // Jubelnde Zuschauer

    // Getter
    int getCurrentObstacle() { return _currentObstacle; }

  private:
    TFT_eSPI* _tft;
    String _riderName;
    String _horseName;
    String _tournamentName;
    int _faults;           // Fehlerpunkte
    float _totalTime;      // Gesamtzeit in Sekunden
    int _obstacleCount;    // Anzahl Hindernisse
    int _currentObstacle;  // Aktuelles Hindernis (0-based)
    ObstacleStatus _obstacleStatus[12];  // Max 12 Hindernisse

    // Hilfsmethoden - Arena
    void drawArenaFence();
    void drawFlowerBoxes();
    void drawSponsorBoard();
    void drawSpectators();
    void drawObstacleProgress();

    // Hilfsmethoden - Pferd & Reiter
    void drawHorse(int x, int y, bool jumping, bool leftFacing = false);
    void drawRider(int x, int y, bool jumping);
    void drawMiniHorse(int x, int y);
    void drawJump(int x, int y, ObstacleType type, ObstacleStatus status);
    void clearObstacleArea(int index);

    // Hilfsmethoden - Animationen
    void showSpectatorReaction(bool positive);
};

#endif
