#include "ProfileDetailScreen.h"

ProfileDetailScreen::ProfileDetailScreen() {
  int btnWidth = 200;
  int btnHeight = 45;
  int centerX = (320 - btnWidth) / 2;

  // "Spielen" Button
  btnPlay = new Button(centerX, 80, btnWidth, btnHeight, "Spielen");
  btnPlay->setColors(TFT_GREEN, TFT_WHITE);

  // "Löschen" Button
  btnDelete = new Button(centerX, 140, btnWidth, btnHeight, "Profil loeschen");
  btnDelete->setColors(TFT_MAROON, TFT_WHITE);

  // "Zurück" Button
  btnBack = new Button(230, 200, 80, 30, "Zurueck");
  btnBack->setColors(TFT_DARKGREY, TFT_WHITE);
}

void ProfileDetailScreen::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void ProfileDetailScreen::setProfile(const Profile& profile) {
  _profile = profile;
}

void ProfileDetailScreen::draw() {
  tft.fillScreen(TFT_BLACK);

  // Profilname groß anzeigen
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(4);

  // Name zentrieren
  int nameWidth = _profile.name.length() * 24;
  int nameX = (320 - nameWidth) / 2;
  if (nameX < 10) nameX = 10;

  tft.setCursor(nameX, 20);
  tft.println(_profile.name);

  // Buttons zeichnen
  btnPlay->draw(&tft);
  btnDelete->draw(&tft);
  btnBack->draw(&tft);
}

int ProfileDetailScreen::handleTouch(int16_t x, int16_t y) {
  if (btnPlay->contains(x, y)) {
    return 1;  // Spielen
  }
  if (btnDelete->contains(x, y)) {
    return 2;  // Löschen
  }
  if (btnBack->contains(x, y)) {
    return 3;  // Zurück
  }
  return 0;
}

Profile ProfileDetailScreen::getProfile() {
  return _profile;
}
