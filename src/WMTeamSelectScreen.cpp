#include "WMTeamSelectScreen.h"
#include "FlagDrawer.h"

#define ITEMS_PER_PAGE 4
#define FLAG_WIDTH 36
#define FLAG_HEIGHT 24
#define ITEM_HEIGHT 38
#define LIST_Y 45
#define LIST_X 10
#define LIST_WIDTH 300

WMTeamSelectScreen::WMTeamSelectScreen() {
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  okButton = new Button(170, 200, 130, 35, "WEITER");
  okButton->setColors(TFT_GREEN, TFT_WHITE);

  prevButton = new Button(20, 200, 60, 35, "<<");
  prevButton->setColors(TFT_BLUE, TFT_WHITE);

  nextButton = new Button(90, 200, 60, 35, ">>");
  nextButton->setColors(TFT_BLUE, TFT_WHITE);

  _currentPage = 0;
  _selectedIndex = WM_TEAM_GERMANY;  // Deutschland als Standard
}

void WMTeamSelectScreen::init(const FavoriteTeam& currentFav) {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);

  _currentPage = 0;
  _selectedIndex = WM_TEAM_GERMANY;  // Deutschland als Standard
  _totalPages = (WM_TEAM_COUNT + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;

  // Vorauswahl wenn bereits gespeichert
  if (currentFav.isSet()) {
    for (int i = 0; i < WM_TEAM_COUNT; i++) {
      if (String(WM_TEAMS[i].name) == currentFav.name) {
        _selectedIndex = i;
        _currentPage = i / ITEMS_PER_PAGE;
        break;
      }
    }
  }
}

void WMTeamSelectScreen::draw() {
  tft.fillScreen(TFT_BLACK);

  // Header
  backButton->draw(&tft);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 12);
  tft.print("DEIN TEAM (WM)");

  // Team-Liste
  drawTeamList();

  // Navigation Buttons
  if (_currentPage > 0) {
    prevButton->draw(&tft);
  }
  if (_currentPage < _totalPages - 1) {
    nextButton->draw(&tft);
  }

  // OK-Button nur wenn Team gewaehlt
  if (_selectedIndex >= 0) {
    okButton->draw(&tft);
  }
}

void WMTeamSelectScreen::drawTeamList() {
  int y = LIST_Y;
  int startIdx = _currentPage * ITEMS_PER_PAGE;

  // Liste leeren
  tft.fillRect(LIST_X, LIST_Y, LIST_WIDTH, ITEMS_PER_PAGE * ITEM_HEIGHT + 10, TFT_BLACK);

  for (int i = 0; i < ITEMS_PER_PAGE && (startIdx + i) < WM_TEAM_COUNT; i++) {
    int teamIdx = startIdx + i;
    const WMTeam& team = WM_TEAMS[teamIdx];

    // Hintergrund
    uint16_t bgColor = (teamIdx == _selectedIndex) ? team.shirtColor : TFT_DARKGREY;
    // Bei schwarzem/dunklem Hintergrund -> weissen Text
    uint16_t textColor = TFT_WHITE;
    if (teamIdx == _selectedIndex && team.shirtColor > 0x2000) {
      textColor = (team.shirtColor == TFT_WHITE) ? TFT_BLACK : TFT_WHITE;
    }

    tft.fillRoundRect(LIST_X, y, LIST_WIDTH, ITEM_HEIGHT - 4, 4, bgColor);

    // Flagge links
    int flagY = y + (ITEM_HEIGHT - 4 - FLAG_HEIGHT) / 2;
    FlagDrawer::drawWMTeamFlag(&tft, LIST_X + 4, flagY, FLAG_WIDTH, FLAG_HEIGHT, teamIdx);

    // Team-Name (nach Flagge verschoben)
    tft.setTextColor(textColor, bgColor);
    tft.setTextSize(2);
    tft.setCursor(LIST_X + FLAG_WIDTH + 12, y + 8);
    tft.print(team.name);

    // Tier-Indikator (Sterne oder Zeit)
    tft.setTextSize(1);
    tft.setCursor(LIST_X + 220, y + 14);
    switch (team.tier) {
      case TIER_1_ELITE:
        tft.setTextColor(TFT_RED, bgColor);
        tft.print("8 Sek");
        break;
      case TIER_2_STRONG:
        tft.setTextColor(TFT_ORANGE, bgColor);
        tft.print("10 Sek");
        break;
      case TIER_3_MEDIUM:
        tft.setTextColor(TFT_YELLOW, bgColor);
        tft.print("12 Sek");
        break;
      case TIER_4_WEAK:
        tft.setTextColor(TFT_GREEN, bgColor);
        tft.print("14 Sek");
        break;
    }

    y += ITEM_HEIGHT;
  }

  // Button-Bereich leeren (fuer saubere Darstellung)
  tft.fillRect(0, 195, 320, 45, TFT_BLACK);
}

int WMTeamSelectScreen::handleTouch(int16_t x, int16_t y) {
  // Zurueck-Button
  if (backButton->contains(x, y)) {
    return 0;
  }

  // OK-Button
  if (_selectedIndex >= 0 && okButton->contains(x, y)) {
    return 1;
  }

  // Vorherige Seite
  if (_currentPage > 0 && prevButton->contains(x, y)) {
    _currentPage--;
    draw();
    return -1;
  }

  // Naechste Seite
  if (_currentPage < _totalPages - 1 && nextButton->contains(x, y)) {
    _currentPage++;
    draw();
    return -1;
  }

  // Team auswaehlen
  int teamIdx = getTeamAtY(y);
  if (teamIdx >= 0 && teamIdx < WM_TEAM_COUNT) {
    _selectedIndex = teamIdx;
    draw();
  }

  return -1;
}

int WMTeamSelectScreen::getTeamAtY(int16_t touchY) {
  if (touchY < LIST_Y || touchY >= LIST_Y + ITEMS_PER_PAGE * ITEM_HEIGHT) {
    return -1;
  }

  int relY = touchY - LIST_Y;
  int itemIndex = relY / ITEM_HEIGHT;
  int teamIdx = _currentPage * ITEMS_PER_PAGE + itemIndex;

  if (teamIdx >= WM_TEAM_COUNT) {
    return -1;
  }

  return teamIdx;
}

FavoriteTeam WMTeamSelectScreen::getSelectedTeam() {
  if (_selectedIndex < 0 || _selectedIndex >= WM_TEAM_COUNT) {
    return FavoriteTeam();
  }

  const WMTeam& t = WM_TEAMS[_selectedIndex];
  return FavoriteTeam(t.name, t.abbrev, t.shirtColor, t.shortsColor);
}
