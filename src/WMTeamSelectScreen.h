#ifndef SRC_WMTEAMSELECTSCREEN_H
#define SRC_WMTEAMSELECTSCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "WM2026Teams.h"
#include "Profile.h"

class WMTeamSelectScreen {
  public:
    WMTeamSelectScreen();

    void init(const FavoriteTeam& currentFav);
    void draw();
    int handleTouch(int16_t x, int16_t y);  // -1=nichts, 0=zurueck, 1=Team gewaehlt

    // Gewaehltes Team abrufen
    FavoriteTeam getSelectedTeam();
    int getSelectedTeamIndex() { return _selectedIndex; }

  private:
    TFT_eSPI tft;
    Button* backButton;
    Button* okButton;
    Button* prevButton;
    Button* nextButton;

    int _currentPage;
    int _totalPages;
    int _selectedIndex;

    void drawTeamList();
    int getTeamAtY(int16_t y);
};

#endif
