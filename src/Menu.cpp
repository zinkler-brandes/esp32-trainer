#include "Menu.h"

Menu::Menu() : _playerName("") {
  // Zurueck-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Button-Layout (2x2 Grid)
  int btnWidth = 135;
  int btnHeight = 50;
  int leftX = 15;
  int rightX = 170;
  int startY = 55;
  int spacing = 15;

  // Zeile 1: Mathe (links), Rekorde (rechts) - Regenbogenfarben
  matheButton = new Button(leftX, startY, btnWidth, btnHeight, "Mathe");
  matheButton->setColors(0xF800, TFT_WHITE);  // Rot

  recordsButton = new Button(rightX, startY, btnWidth, btnHeight, "Rekorde");
  recordsButton->setColors(0xFD20, TFT_WHITE);  // Orange

  // Zeile 2: Schreiben (links), Quiz (rechts)
  schreibenButton = new Button(leftX, startY + btnHeight + spacing, btnWidth, btnHeight, "Schreiben");
  schreibenButton->setColors(0x07E0, TFT_BLACK);  // Grün

  quizButton = new Button(rightX, startY + btnHeight + spacing, btnWidth, btnHeight, "Quiz");
  quizButton->setColors(0x07FF, TFT_BLACK);  // Cyan
}

void Menu::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void Menu::setPlayerName(const String& name) {
  _playerName = name;
  // Auf max Länge kürzen
  if (_playerName.length() > MAX_PLAYER_NAME_LENGTH) {
    _playerName = _playerName.substring(0, MAX_PLAYER_NAME_LENGTH);
  }
}

void Menu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurueck-Button
  backButton->draw(&tft);

  // Spielername (neben dem Zurueck-Button)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(60, 12);
  tft.print(_playerName);

  // Buttons zeichnen (2x2 Grid)
  matheButton->draw(&tft);
  recordsButton->draw(&tft);
  schreibenButton->draw(&tft);
  quizButton->draw(&tft);
}

int Menu::handleTouch(int16_t x, int16_t y) {
  // Zurueck-Button
  if (backButton->contains(x, y)) {
    return 0;  // Zurueck zur Profilauswahl
  }

  // Mathe-Button
  if (matheButton->contains(x, y)) {
    return 1;  // Mathe
  }

  // Rekorde-Button
  if (recordsButton->contains(x, y)) {
    return 2;  // Rekorde
  }

  // Schreiben-Button
  if (schreibenButton->contains(x, y)) {
    return 3;  // Schreiben
  }

  // Quiz-Button
  if (quizButton->contains(x, y)) {
    return 4;  // Quiz-Menue
  }

  return -1;  // Nichts getroffen
}
