#ifndef SRC_GROUPTABLE_H
#define SRC_GROUPTABLE_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "WM2026Teams.h"
#include "WM2026Groups.h"
#include "WorldCupSave.h"

class GroupTable {
  public:
    GroupTable();

    // Initialisierung mit Gruppenindex
    void init(int groupIndex);

    // Ergebnis hinzufuegen
    void addResult(int team1Index, int team2Index, int goals1, int goals2);

    // Match-Index (0-5) fuer ein Gruppenspiel berechnen
    // (Team1 vs Team2 in einer 4er-Gruppe)
    int getMatchIndex(int team1LocalIndex, int team2LocalIndex);

    // Gruppen-Spielplan: Wer spielt gegen wen?
    // localMatch: 0-5 (6 Spiele pro Gruppe)
    // Gibt die lokalen Indices (0-3) der beiden Teams zurueck
    void getMatchTeams(int localMatch, int& team1Local, int& team2Local);

    // Tabelle berechnen und sortieren
    void calculateStandings();

    // Tabelle zeichnen
    void draw(TFT_eSPI* tft, int playerTeamIndex);

    // Ergebnisse zeichnen (mit Pagination)
    // page 0 = Spieltag 1+2, page 1 = Spieltag 3
    void drawResults(TFT_eSPI* tft, int playerTeamIndex, int page);

    // Platzierung eines Teams abfragen (1-4)
    int getTeamPosition(int teamIndex);

    // Top 2 oder bester Dritter?
    bool isTeamQualified(int teamIndex, bool* asBestThird = nullptr);

    // Getter
    GroupStanding getStanding(int position) { return _standings[position]; }
    int getGroupIndex() { return _groupIndex; }
    char getGroupLetter() { return WM_GROUPS[_groupIndex].letter; }

    // Speichern/Laden Hilfsfunktionen
    bool isComplete();  // Alle 6 Spiele gespielt?
    bool hasMatchResult(int matchIndex);  // Einzelnes Spiel gespielt?
    void getMatchResult(int matchIndex, int& goals1, int& goals2);  // Ergebnis abrufen

  private:
    int _groupIndex;
    GroupStanding _standings[4];
    GroupMatchResult _results[6];  // 6 Spiele pro Gruppe

    // Hilfsfunktion: Team-Index -> lokaler Index (0-3) in Gruppe
    int getLocalIndex(int teamIndex);
};

#endif
