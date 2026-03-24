#include "ProfileSelectScreen.h"

ProfileSelectScreen::ProfileSelectScreen() : _profileManager(nullptr), _scrollOffset(0), _visibleCount(0) {
  int btnWidth = 230;
  int btnHeight = 40;
  int startX = 20;
  int startY = 45;
  int spacing = 10;

  // Profil-Buttons (werden dynamisch gefüllt)
  for (int i = 0; i < MAX_VISIBLE_PROFILES; i++) {
    profileButtons[i] = new Button(startX, startY + i * (btnHeight + spacing), btnWidth, btnHeight, "");
    profileButtons[i]->setColors(TFT_BLUE, TFT_WHITE);
  }

  // Scroll-Buttons (rechts neben den Profil-Buttons)
  btnScrollUp = new Button(260, startY, 50, 40, "^");
  btnScrollUp->setColors(TFT_DARKGREY, TFT_WHITE);

  btnScrollDown = new Button(260, startY + 2 * (btnHeight + spacing), 50, 40, "v");
  btnScrollDown->setColors(TFT_DARKGREY, TFT_WHITE);

  // Untere Leiste: "Neu" und "Verwalten"
  int bottomY = 195;
  btnNewProfile = new Button(20, bottomY, 145, 38, "+ Neu");
  btnNewProfile->setColors(TFT_DARKGREEN, TFT_WHITE);

  btnManage = new Button(175, bottomY, 135, 38, "Verwalten");
  btnManage->setColors(TFT_DARKGREY, TFT_WHITE);
}

void ProfileSelectScreen::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void ProfileSelectScreen::setProfileManager(ProfileManager* pm) {
  _profileManager = pm;
  _scrollOffset = 0;
}

void ProfileSelectScreen::updateButtons() {
  if (!_profileManager) return;

  int totalProfiles = _profileManager->getProfileCount();
  _visibleCount = min(totalProfiles - _scrollOffset, MAX_VISIBLE_PROFILES);

  // Buttons mit Profilnamen füllen
  for (int i = 0; i < MAX_VISIBLE_PROFILES; i++) {
    int profileIndex = _scrollOffset + i;
    if (profileIndex < totalProfiles) {
      Profile p = _profileManager->getProfile(profileIndex);
      profileButtons[i]->setLabel(p.name);
      profileButtons[i]->setColors(TFT_BLUE, TFT_WHITE);
    } else {
      // Button verstecken (leerer Text)
      profileButtons[i]->setLabel("");
    }
  }
}

void ProfileSelectScreen::draw() {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(40, 12);
  tft.print("Wer spielt heute?");

  updateButtons();

  int totalProfiles = _profileManager ? _profileManager->getProfileCount() : 0;

  // Profil-Buttons zeichnen (nur wenn nicht leer)
  for (int i = 0; i < _visibleCount; i++) {
    profileButtons[i]->draw(&tft);
  }

  // Scroll-Buttons (nur wenn nötig)
  if (totalProfiles > MAX_VISIBLE_PROFILES) {
    // Scroll-Indikator in der Mitte
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(270, 100);
    tft.printf("%d/%d", _scrollOffset + 1, totalProfiles - MAX_VISIBLE_PROFILES + 1);

    if (_scrollOffset > 0) {
      btnScrollUp->draw(&tft);
    }
    if (_scrollOffset + MAX_VISIBLE_PROFILES < totalProfiles) {
      btnScrollDown->draw(&tft);
    }
  }

  // Untere Buttons
  btnNewProfile->draw(&tft);
  btnManage->draw(&tft);
}

int ProfileSelectScreen::handleTouch(int16_t x, int16_t y) {
  if (!_profileManager) return 0;

  int totalProfiles = _profileManager->getProfileCount();

  // Profil-Buttons prüfen -> Direkt spielen!
  for (int i = 0; i < _visibleCount; i++) {
    if (profileButtons[i]->contains(x, y)) {
      int profileIndex = _scrollOffset + i;
      Profile p = _profileManager->getProfile(profileIndex);
      return p.id;  // Profil-ID -> direkt einloggen
    }
  }

  // "Neues Profil" Button
  if (btnNewProfile->contains(x, y)) {
    return -1;
  }

  // "Verwalten" Button
  if (btnManage->contains(x, y)) {
    return -2;
  }

  // Scroll-Buttons
  if (totalProfiles > MAX_VISIBLE_PROFILES) {
    if (btnScrollUp->contains(x, y) && _scrollOffset > 0) {
      _scrollOffset--;
      draw();
      return 0;
    }
    if (btnScrollDown->contains(x, y) && _scrollOffset + MAX_VISIBLE_PROFILES < totalProfiles) {
      _scrollOffset++;
      draw();
      return 0;
    }
  }

  return 0;
}
