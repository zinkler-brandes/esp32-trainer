#include "Menu.h"

Menu::Menu() : _playerName("") {
  // Zurueck-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Vertikales Layout: 3 Buttons untereinander
  int btnWidth = 200;
  int btnHeight = 45;
  int spacing = 12;
  int startX = 60;
  int startY = 65;

  // Alle Buttons gruen mit weiss (Fussball-Theme)
  uint16_t btnColor = 0x3665;  // FIELD_GREEN

  // Mathe
  matheButton = new Button(startX, startY, btnWidth, btnHeight, "Mathe");
  matheButton->setColors(btnColor, TFT_WHITE);

  // Rekorde
  recordsButton = new Button(startX, startY + btnHeight + spacing, btnWidth, btnHeight, "Rekorde");
  recordsButton->setColors(btnColor, TFT_WHITE);

  // Quiz
  quizButton = new Button(startX, startY + 2 * (btnHeight + spacing), btnWidth, btnHeight, "Quiz");
  quizButton->setColors(btnColor, TFT_WHITE);
}

void Menu::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void Menu::setPlayerName(const String& name) {
  _playerName = name;
  if (_playerName.length() > MAX_PLAYER_NAME_LENGTH) {
    _playerName = _playerName.substring(0, MAX_PLAYER_NAME_LENGTH);
  }
}

void Menu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurueck-Button
  backButton->draw(&tft);

  // Spielername
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(60, 12);
  tft.print(_playerName);

  // Fussball-Icon rechts oben
  int iconX = 280;
  int iconY = 20;
  tft.fillCircle(iconX, iconY, 14, TFT_WHITE);
  tft.fillCircle(iconX, iconY, 5, TFT_BLACK);
  tft.fillCircle(iconX - 8, iconY - 6, 4, TFT_BLACK);
  tft.fillCircle(iconX + 8, iconY - 6, 4, TFT_BLACK);
  tft.fillCircle(iconX - 8, iconY + 6, 4, TFT_BLACK);
  tft.fillCircle(iconX + 8, iconY + 6, 4, TFT_BLACK);

  // Ueberschrift
  tft.setTextSize(2);
  tft.setTextColor(0x3665, TFT_BLACK);
  tft.setCursor(115, 48);
  tft.print("Fussball");

  // Buttons zeichnen
  matheButton->draw(&tft);
  recordsButton->draw(&tft);
  quizButton->draw(&tft);

  // Icons neben den Button-Texten
  drawMatheIcon(75, 87);
  drawRecordsIcon(75, 87 + 45 + 12);
  drawQuizIcon(75, 87 + 2 * (45 + 12));
}

void Menu::drawMatheIcon(int x, int y) {
  // Plus/Minus Symbol
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, 0x3665);
  tft.setCursor(x - 5, y - 8);
  tft.print("+-");
}

void Menu::drawRecordsIcon(int x, int y) {
  // Pokal
  tft.fillRect(x - 6, y - 8, 12, 10, TFT_YELLOW);
  tft.fillRect(x - 3, y + 2, 6, 4, TFT_YELLOW);
  tft.fillRect(x - 5, y + 6, 10, 3, TFT_YELLOW);
  // Henkel
  tft.drawRect(x - 10, y - 6, 5, 6, TFT_YELLOW);
  tft.drawRect(x + 5, y - 6, 5, 6, TFT_YELLOW);
}

void Menu::drawQuizIcon(int x, int y) {
  // Fragezeichen
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, 0x3665);
  tft.setCursor(x - 3, y - 8);
  tft.print("?");
}

int Menu::handleTouch(int16_t x, int16_t y) {
  if (backButton->contains(x, y)) {
    return 0;
  }
  if (matheButton->contains(x, y)) {
    return 1;
  }
  if (recordsButton->contains(x, y)) {
    return 2;
  }
  if (quizButton->contains(x, y)) {
    return 3;
  }
  return -1;
}
