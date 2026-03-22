#include "ProfileManageScreen.h"

ProfileManageScreen::ProfileManageScreen() : _profileManager(nullptr), _scrollOffset(0), _visibleCount(0) {
  int btnWidth = 230;
  int btnHeight = 40;
  int startX = 20;
  int startY = 45;
  int spacing = 10;

  // Profil-Buttons (rot für Löschen)
  for (int i = 0; i < MANAGE_VISIBLE_PROFILES; i++) {
    profileButtons[i] = new Button(startX, startY + i * (btnHeight + spacing), btnWidth, btnHeight, "");
    profileButtons[i]->setColors(TFT_MAROON, TFT_WHITE);
  }

  // Scroll-Buttons
  btnScrollUp = new Button(260, startY, 50, 40, "^");
  btnScrollUp->setColors(TFT_DARKGREY, TFT_WHITE);

  btnScrollDown = new Button(260, startY + 2 * (btnHeight + spacing), 50, 40, "v");
  btnScrollDown->setColors(TFT_DARKGREY, TFT_WHITE);

  // Untere Leiste
  int bottomY = 195;
  btnBack = new Button(20, bottomY, 50, 38, "<");
  btnBack->setColors(TFT_DARKGREY, TFT_WHITE);

  btnReset = new Button(80, bottomY, 230, 38, "Alle loeschen");
  btnReset->setColors(TFT_RED, TFT_WHITE);
}

void ProfileManageScreen::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void ProfileManageScreen::setProfileManager(ProfileManager* pm) {
  _profileManager = pm;
  _scrollOffset = 0;
}

void ProfileManageScreen::updateButtons() {
  if (!_profileManager) return;

  int totalProfiles = _profileManager->getProfileCount();
  _visibleCount = min(totalProfiles - _scrollOffset, MANAGE_VISIBLE_PROFILES);

  for (int i = 0; i < MANAGE_VISIBLE_PROFILES; i++) {
    int profileIndex = _scrollOffset + i;
    if (profileIndex < totalProfiles) {
      Profile p = _profileManager->getProfile(profileIndex);
      profileButtons[i]->setLabel("X  " + p.name);
      profileButtons[i]->setColors(TFT_MAROON, TFT_WHITE);
    } else {
      profileButtons[i]->setLabel("");
    }
  }
}

void ProfileManageScreen::draw() {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 12);
  tft.print("Profilverwaltung");

  updateButtons();

  int totalProfiles = _profileManager ? _profileManager->getProfileCount() : 0;

  // Profil-Buttons zeichnen
  for (int i = 0; i < _visibleCount; i++) {
    profileButtons[i]->draw(&tft);
  }

  // Hinweis wenn keine Profile
  if (totalProfiles == 0) {
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(60, 100);
    tft.print("Keine Profile");
  }

  // Scroll-Buttons
  if (totalProfiles > MANAGE_VISIBLE_PROFILES) {
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(270, 100);
    tft.printf("%d/%d", _scrollOffset + 1, totalProfiles - MANAGE_VISIBLE_PROFILES + 1);

    if (_scrollOffset > 0) {
      btnScrollUp->draw(&tft);
    }
    if (_scrollOffset + MANAGE_VISIBLE_PROFILES < totalProfiles) {
      btnScrollDown->draw(&tft);
    }
  }

  // Untere Buttons
  btnBack->draw(&tft);
  btnReset->draw(&tft);
}

int ProfileManageScreen::handleTouch(int16_t x, int16_t y) {
  if (!_profileManager) return 0;

  int totalProfiles = _profileManager->getProfileCount();

  // Profil-Buttons prüfen -> Löschen
  for (int i = 0; i < _visibleCount; i++) {
    if (profileButtons[i]->contains(x, y)) {
      int profileIndex = _scrollOffset + i;
      Profile p = _profileManager->getProfile(profileIndex);
      return p.id;  // Profil-ID zum Löschen
    }
  }

  // "Zurück" Button
  if (btnBack->contains(x, y)) {
    return -2;
  }

  // "Reset" Button
  if (btnReset->contains(x, y)) {
    return -1;
  }

  // Scroll-Buttons
  if (totalProfiles > MANAGE_VISIBLE_PROFILES) {
    if (btnScrollUp->contains(x, y) && _scrollOffset > 0) {
      _scrollOffset--;
      draw();
      return 0;
    }
    if (btnScrollDown->contains(x, y) && _scrollOffset + MANAGE_VISIBLE_PROFILES < totalProfiles) {
      _scrollOffset++;
      draw();
      return 0;
    }
  }

  return 0;
}
