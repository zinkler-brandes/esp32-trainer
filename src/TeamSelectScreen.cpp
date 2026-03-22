#include "TeamSelectScreen.h"

#define ITEMS_PER_PAGE 4
#define ITEM_HEIGHT 38
#define LIST_Y 45
#define LIST_X 10
#define LIST_WIDTH 300

TeamSelectScreen::TeamSelectScreen() {
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  okButton = new Button(170, 200, 130, 35, "WEITER");
  okButton->setColors(TFT_GREEN, TFT_WHITE);

  prevButton = new Button(20, 200, 60, 35, "<<");
  prevButton->setColors(TFT_BLUE, TFT_WHITE);

  nextButton = new Button(90, 200, 60, 35, ">>");
  nextButton->setColors(TFT_BLUE, TFT_WHITE);

  _currentPage = 0;
  _selectedIndex = -1;
}

void TeamSelectScreen::init(TournamentType type, const FavoriteTeam& currentFav) {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);

  _type = type;

  // Teams je nach Turnier-Typ
  if (type == TOURNAMENT_DFB_POKAL) {
    _teams = DFB_SELECTABLE_TEAMS;
    _teamCount = DFB_SELECTABLE_COUNT;
  } else {
    _teams = CL_SELECTABLE_TEAMS;
    _teamCount = CL_SELECTABLE_COUNT;
  }

  _currentPage = 0;
  _selectedIndex = -1;
  _totalPages = (_teamCount + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;

  // Vorauswahl wenn bereits gespeichert
  if (currentFav.isSet()) {
    for (int i = 0; i < _teamCount; i++) {
      if (String(_teams[i].name) == currentFav.name) {
        _selectedIndex = i;
        _currentPage = i / ITEMS_PER_PAGE;
        break;
      }
    }
  }
}

void TeamSelectScreen::draw() {
  tft.fillScreen(TFT_BLACK);

  // Header
  backButton->draw(&tft);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 12);
  if (_type == TOURNAMENT_DFB_POKAL) {
    tft.print("DEIN TEAM (DFB)");
  } else {
    tft.print("DEIN TEAM (CL)");
  }

  // Team-Liste
  drawTeamList();

  // Navigation Buttons
  if (_currentPage > 0) {
    prevButton->draw(&tft);
  }
  if (_currentPage < _totalPages - 1) {
    nextButton->draw(&tft);
  }

  // OK-Button nur wenn Team gewählt
  if (_selectedIndex >= 0) {
    okButton->draw(&tft);
  }
}

void TeamSelectScreen::drawTeamList() {
  int y = LIST_Y;
  int startIdx = _currentPage * ITEMS_PER_PAGE;

  // Liste leeren
  tft.fillRect(LIST_X, LIST_Y, LIST_WIDTH, ITEMS_PER_PAGE * ITEM_HEIGHT + 10, TFT_BLACK);

  for (int i = 0; i < ITEMS_PER_PAGE && (startIdx + i) < _teamCount; i++) {
    int teamIdx = startIdx + i;
    const Team& team = _teams[teamIdx];

    // Hintergrund
    uint16_t bgColor = (teamIdx == _selectedIndex) ? team.primaryColor : TFT_DARKGREY;
    uint16_t textColor = (teamIdx == _selectedIndex) ? team.secondaryColor : TFT_WHITE;

    tft.fillRoundRect(LIST_X, y, LIST_WIDTH, ITEM_HEIGHT - 4, 4, bgColor);

    // Teamfarben-Indikator links
    if (teamIdx != _selectedIndex) {
      tft.fillRect(LIST_X, y, 8, ITEM_HEIGHT - 4, team.primaryColor);
    }

    // Team-Name
    tft.setTextColor(textColor, bgColor);
    tft.setTextSize(2);
    tft.setCursor(LIST_X + 15, y + 10);
    tft.print(team.name);

    y += ITEM_HEIGHT;
  }
}

int TeamSelectScreen::handleTouch(int16_t x, int16_t y) {
  // Zurück-Button
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

  // Nächste Seite
  if (_currentPage < _totalPages - 1 && nextButton->contains(x, y)) {
    _currentPage++;
    draw();
    return -1;
  }

  // Team auswählen
  int teamIdx = getTeamAtY(y);
  if (teamIdx >= 0 && teamIdx < _teamCount) {
    _selectedIndex = teamIdx;
    draw();
  }

  return -1;
}

int TeamSelectScreen::getTeamAtY(int16_t y) {
  if (y < LIST_Y || y >= LIST_Y + ITEMS_PER_PAGE * ITEM_HEIGHT) {
    return -1;
  }

  int relY = y - LIST_Y;
  int itemIndex = relY / ITEM_HEIGHT;
  int teamIdx = _currentPage * ITEMS_PER_PAGE + itemIndex;

  if (teamIdx >= _teamCount) {
    return -1;
  }

  return teamIdx;
}

FavoriteTeam TeamSelectScreen::getSelectedTeam() {
  if (_selectedIndex < 0 || _selectedIndex >= _teamCount) {
    return FavoriteTeam();
  }

  const Team& t = _teams[_selectedIndex];
  return FavoriteTeam(t.name, t.abbrev, t.primaryColor, t.secondaryColor);
}
