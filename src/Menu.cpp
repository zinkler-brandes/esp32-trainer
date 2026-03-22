#include "Menu.h"

Menu::Menu() : _playerName("") {
  // Zurück-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Fächer-Buttons
  int btnWidth = 280;
  int btnHeight = 60;
  int startX = 20;
  int startY = 70;
  int spacing = 20;

  // Button 1: Mathe
  menuButtons[0] = new Button(startX, startY, btnWidth, btnHeight, "Mathe");
  menuButtons[0]->setColors(TFT_BLUE, TFT_WHITE);

  // Button 2: Englisch (noch nicht implementiert)
  menuButtons[1] = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "Englisch");
  menuButtons[1]->setColors(TFT_PURPLE, TFT_WHITE);
}

void Menu::init() {
  tft.init();
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

  // Zurück-Button
  backButton->draw(&tft);

  // Spielername (neben dem Zurück-Button)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(60, 12);
  tft.print(_playerName);

  // Fächer-Buttons zeichnen
  for (int i = 0; i < 2; i++) {
    menuButtons[i]->draw(&tft);
  }
}

int Menu::handleTouch(int16_t x, int16_t y) {
  // Zurück-Button
  if (backButton->contains(x, y)) {
    return 0;  // Zurück zur Profilauswahl
  }

  // Fächer-Buttons
  for (int i = 0; i < 2; i++) {
    if (menuButtons[i]->contains(x, y)) {
      return i + 1;  // 1=Mathe, 2=Englisch
    }
  }
  return -1;  // Nichts getroffen
}
