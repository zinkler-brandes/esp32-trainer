#ifndef SRC_SOCCERFIELD_H
#define SRC_SOCCERFIELD_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Teams.h"

// Farben für das Spielfeld (Bundesliga-Stil mit dezenten Streifen)
#define FIELD_GREEN 0x3665       // Hellgrün für Rasen
#define FIELD_GREEN_ALT 0x2DE4   // Leicht dunkleres Grün (dezenter Kontrast)
#define FIELD_GREEN_LINE 0x3625  // Sehr dezente horizontale Linien
#define GOAL_WHITE TFT_WHITE
#define PLAYER_BLUE 0x001F
#define BALL_WHITE TFT_WHITE

class SoccerField {
  public:
    SoccerField();

    // TFT-Display setzen (muss vor Verwendung aufgerufen werden)
    void setDisplay(TFT_eSPI* display);

    // Spieler-Daten setzen
    void setPlayerName(const String& name);
    void setPlayerTeamColor(uint16_t color);
    void setOpponentName(const String& name);
    void setOpponentColor(uint16_t color);
    void setScore(int goalsScored, int goalsAgainst);
    void setStepsForGoal(int steps);

    // Turnier-Modus fuer Logo-Anzeige
    void setTournamentMode(TournamentType type, int playerTeamIdx = -1, int opponentTeamIdx = -1);
    void clearTournamentMode();  // Deaktiviert Logos

    // Spielfeld zeichnen
    void drawField(int unused = 0);
    void drawGoalLeft();
    void drawGoalRight();
    void drawGoal();  // Zeichnet beide Tore
    void drawGoalkeeper();
    void drawPlayer(int position, bool withBall);
    void drawScoreboard();

    // Animationen
    void showGoalAnimation(int fromPosition);
    void showCounterGoalAnimation(int num1, char operation, int num2, int result);
    void showCorrectStep(int newPosition);

    // Spezial-Screens
    void showNachspielzeitTafel(int sekunden);
    void showGameOver(int goalsScored, int goalsAgainst);
    void showPenaltyResult(bool playerScored, bool cpuScored,
                           int playerTotal, int cpuTotal, int round);

    // Getter
    String getPlayerName() { return _playerName; }

    // Zuschauer
    void drawSpectators(bool standing = false);  // normal sitzend, oder stehend
    void drawJubilatingSpectators(int frame);    // jubelnd (animiert)

  private:
    TFT_eSPI* _tft;
    String _playerName;
    uint16_t _playerTeamColor;
    String _opponentName;
    uint16_t _opponentColor;
    int _goalsScored;
    int _goalsAgainst;
    int _stepsForGoal;

    // Turnier-Modus fuer Logos/Flaggen
    bool _showLogos;
    TournamentType _tournamentType;
    int _playerTeamIndex;    // WM-Team-Index (-1 wenn nicht WM)
    int _opponentTeamIndex;  // WM-Team-Index (-1 wenn nicht WM)
};

#endif
