#include "ReitMenu.h"

ReitMenu::ReitMenu() : _playerName("") {
  // Zurueck-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Button-Layout (vertikal)
  int btnWidth = 200;
  int btnHeight = 45;
  int centerX = 60;
  int startY = 55;
  int spacing = 12;

  // Parcours-Button (braun)
  parcoursButton = new Button(centerX, startY, btnWidth, btnHeight, "Parcours");
  parcoursButton->setColors(0xB2A0, TFT_WHITE);  // Hellbraun

  // Turnier-Button (gold)
  turnierButton = new Button(centerX, startY + btnHeight + spacing, btnWidth, btnHeight, "Turnier");
  turnierButton->setColors(0xFE60, TFT_BLACK);  // Gold

  // Rekorde-Button (silber)
  recordsButton = new Button(centerX, startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "Rekorde");
  recordsButton->setColors(0xC618, TFT_BLACK);  // Silber
}

void ReitMenu::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void ReitMenu::setPlayerName(const String& name) {
  _playerName = name;
  // Auf max Laenge kuerzen
  if (_playerName.length() > MAX_PLAYER_NAME_LENGTH) {
    _playerName = _playerName.substring(0, MAX_PLAYER_NAME_LENGTH);
  }
}

void ReitMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurueck-Button
  backButton->draw(&tft);

  // Spielername (neben dem Zurueck-Button)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(60, 12);
  tft.print(_playerName);

  // Buttons zeichnen
  parcoursButton->draw(&tft);
  turnierButton->draw(&tft);
  recordsButton->draw(&tft);

  // Pferde-Dekoration rechts
  drawHorseIcon(280, 120, 40);
}

void ReitMenu::drawHorseIcon(int x, int y, int size) {
  // Skalierungsfaktor
  float s = size / 40.0;
  uint16_t brown = 0x8200;  // Dunkelbraun

  // Koerper
  tft.fillEllipse(x, y + (int)(5*s), (int)(25*s), (int)(15*s), brown);

  // Hals
  tft.fillTriangle(
    x + (int)(10*s), y - (int)(5*s),
    x + (int)(25*s), y - (int)(25*s),
    x + (int)(15*s), y + (int)(5*s),
    brown
  );

  // Kopf
  tft.fillEllipse(x + (int)(28*s), y - (int)(28*s), (int)(10*s), (int)(8*s), brown);

  // Ohren
  tft.fillTriangle(
    x + (int)(25*s), y - (int)(38*s),
    x + (int)(28*s), y - (int)(32*s),
    x + (int)(22*s), y - (int)(32*s),
    brown
  );
  tft.fillTriangle(
    x + (int)(33*s), y - (int)(38*s),
    x + (int)(36*s), y - (int)(32*s),
    x + (int)(30*s), y - (int)(32*s),
    brown
  );

  // Beine
  int legW = (int)(4*s);
  int legH = (int)(18*s);
  tft.fillRect(x - (int)(15*s), y + (int)(15*s), legW, legH, brown);
  tft.fillRect(x - (int)(5*s), y + (int)(15*s), legW, legH, brown);
  tft.fillRect(x + (int)(5*s), y + (int)(15*s), legW, legH, brown);
  tft.fillRect(x + (int)(15*s), y + (int)(15*s), legW, legH, brown);

  // Schweif
  tft.fillTriangle(
    x - (int)(25*s), y,
    x - (int)(35*s), y + (int)(15*s),
    x - (int)(20*s), y + (int)(10*s),
    brown
  );
}

int ReitMenu::handleTouch(int16_t x, int16_t y) {
  // Zurueck-Button
  if (backButton->contains(x, y)) {
    return 0;  // Zurueck zur Themenauswahl
  }

  // Parcours-Button
  if (parcoursButton->contains(x, y)) {
    return 1;  // Parcours (Einzelspiel)
  }

  // Turnier-Button
  if (turnierButton->contains(x, y)) {
    return 2;  // Turnier
  }

  // Rekorde-Button
  if (recordsButton->contains(x, y)) {
    return 3;  // Rekorde
  }

  return -1;  // Nichts getroffen
}
