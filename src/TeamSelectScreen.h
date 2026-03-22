#ifndef SRC_TEAMSELECTSCREEN_H
#define SRC_TEAMSELECTSCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "Teams.h"
#include "Profile.h"

class TeamSelectScreen {
  public:
    TeamSelectScreen();

    void init(TournamentType type, const FavoriteTeam& currentFav);
    void draw();
    int handleTouch(int16_t x, int16_t y);  // -1=nichts, 0=zurück, 1=Team gewählt

    // Gewähltes Team abrufen
    FavoriteTeam getSelectedTeam();

  private:
    TFT_eSPI tft;
    Button* backButton;
    Button* okButton;
    Button* prevButton;
    Button* nextButton;

    TournamentType _type;
    const Team* _teams;
    int _teamCount;

    int _currentPage;
    int _totalPages;
    int _selectedIndex;

    void drawTeamList();
    int getTeamAtY(int16_t y);
};

#endif
