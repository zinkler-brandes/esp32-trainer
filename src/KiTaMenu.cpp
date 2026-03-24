#include "KiTaMenu.h"

KiTaMenu::KiTaMenu() : _playerName("") {
  // Zurueck-Button (oben links)
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Schreiben-Button (gross, mittig)
  schreibenButton = new Button(60, 70, 200, 80, "Schreiben");
  schreibenButton->setColors(0xFD20, TFT_BLACK);  // Orange (wie KiTa-Thema)
}

void KiTaMenu::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void KiTaMenu::setPlayerName(const String& name) {
  _playerName = name;
  if (_playerName.length() > 8) {
    _playerName = _playerName.substring(0, 8);
  }
}

void KiTaMenu::draw() {
  tft.fillScreen(TFT_BLACK);

  // Zurueck-Button
  backButton->draw(&tft);

  // Spielername (neben dem Zurueck-Button)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(60, 12);
  tft.print(_playerName);

  // Ueberschrift
  tft.setTextSize(2);
  tft.setTextColor(0xFD20, TFT_BLACK);  // Orange
  tft.setCursor(130, 50);
  tft.print("KiTa");

  // Schreiben-Button
  schreibenButton->draw(&tft);

  // Stift-Icon neben dem Text
  drawPencilIcon(250, 100);

  // Erklaerung
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(60, 165);
  tft.print("Buchstaben und Zahlen schreiben lernen");

  // Platzhalter fuer weitere Aktivitaeten
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(60, 200);
  tft.print("Weitere Aktivitaeten folgen...");
}

void KiTaMenu::drawPencilIcon(int x, int y) {
  // Grosser Stift
  // Stiftkoerper (gelb)
  tft.fillRect(x - 5, y - 25, 10, 40, TFT_YELLOW);

  // Metallring
  tft.fillRect(x - 6, y + 10, 12, 5, TFT_DARKGREY);

  // Spitze
  tft.fillTriangle(x - 5, y + 15, x + 5, y + 15, x, y + 28, 0xFE60);  // Holz
  tft.fillTriangle(x - 2, y + 22, x + 2, y + 22, x, y + 28, TFT_BLACK);  // Graphit

  // Radiergummi oben
  tft.fillRect(x - 5, y - 30, 10, 8, TFT_MAGENTA);
  tft.fillRect(x - 6, y - 25, 12, 3, TFT_DARKGREY);
}

int KiTaMenu::handleTouch(int16_t x, int16_t y) {
  // Zurueck-Button
  if (backButton->contains(x, y)) {
    return 0;  // Zurueck zur Themenauswahl
  }

  // Schreiben-Button
  if (schreibenButton->contains(x, y)) {
    return 1;  // Schreiben
  }

  return -1;  // Nichts getroffen
}
