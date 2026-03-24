#include "KeyboardScreen.h"

KeyboardScreen::KeyboardScreen() : _name("") {
  createKeys();
}

void KeyboardScreen::createKeys() {
  // Tastatur-Layout (QWERTZ, nur Großbuchstaben)
  // Reihe 1: Q W E R T Z U I O
  // Reihe 2: A S D F G H J K L
  // Reihe 3: Y X C V B N M [<-]
  // Reihe 4: [LEER]
  // Reihe 5: [Abbrechen] [OK]

  const char* row1 = "QWERTZUIOP";
  const char* row2 = "ASDFGHJKL";
  const char* row3 = "YXCVBNM";

  int keyW = 28;
  int keyH = 28;
  int spacing = 3;
  int startY = 60;

  int keyIndex = 0;

  // Reihe 1: QWERTZUIOP (10 Tasten)
  int row1StartX = (320 - (10 * keyW + 9 * spacing)) / 2;
  for (int i = 0; i < 10; i++) {
    char label[2] = {row1[i], '\0'};
    keys[keyIndex] = new Button(row1StartX + i * (keyW + spacing), startY, keyW, keyH, label);
    keys[keyIndex]->setColors(TFT_NAVY, TFT_WHITE);
    keyIndex++;
  }

  // Reihe 2: ASDFGHJKL (9 Tasten, zentriert mit leichtem Versatz)
  int row2Y = startY + keyH + spacing;
  int row2StartX = (320 - (9 * keyW + 8 * spacing)) / 2 + 5;
  for (int i = 0; i < 9; i++) {
    char label[2] = {row2[i], '\0'};
    keys[keyIndex] = new Button(row2StartX + i * (keyW + spacing), row2Y, keyW, keyH, label);
    keys[keyIndex]->setColors(TFT_NAVY, TFT_WHITE);
    keyIndex++;
  }

  // Reihe 3: YXCVBNM + Backspace (8 Tasten)
  int row3Y = row2Y + keyH + spacing;
  int row3StartX = (320 - (8 * keyW + 7 * spacing)) / 2;
  for (int i = 0; i < 7; i++) {
    char label[2] = {row3[i], '\0'};
    keys[keyIndex] = new Button(row3StartX + i * (keyW + spacing), row3Y, keyW, keyH, label);
    keys[keyIndex]->setColors(TFT_NAVY, TFT_WHITE);
    keyIndex++;
  }
  // Backspace (etwas breiter)
  keys[keyIndex] = new Button(row3StartX + 7 * (keyW + spacing), row3Y, keyW + 10, keyH, "<-");
  keys[keyIndex]->setColors(TFT_MAROON, TFT_WHITE);
  keyIndex++;

  // Reihe 4: Leerzeichen
  int row4Y = row3Y + keyH + spacing;
  keys[keyIndex] = new Button(70, row4Y, 180, keyH, "LEER");
  keys[keyIndex]->setColors(TFT_DARKGREY, TFT_WHITE);
  keyIndex++;

  // Buttons: Abbrechen und OK
  int row5Y = row4Y + keyH + spacing + 5;
  btnCancel = new Button(30, row5Y, 100, 35, "Abbruch");
  btnCancel->setColors(TFT_DARKGREY, TFT_WHITE);

  btnOk = new Button(190, row5Y, 100, 35, "OK");
  btnOk->setColors(TFT_GREEN, TFT_WHITE);
}

void KeyboardScreen::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void KeyboardScreen::draw() {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(80, 5);
  tft.print("Name eingeben");

  // Name-Eingabefeld
  updateNameDisplay();

  // Tastatur zeichnen
  for (int i = 0; i < 28; i++) {  // 26 Buchstaben + Backspace + Space
    if (keys[i]) {
      keys[i]->draw(&tft);
    }
  }

  // Buttons
  btnCancel->draw(&tft);
  btnOk->draw(&tft);
}

void KeyboardScreen::updateNameDisplay() {
  // Eingabefeld clearen
  tft.fillRect(30, 28, 260, 25, TFT_BLACK);

  // Rahmen
  tft.drawRect(30, 28, 260, 25, TFT_WHITE);

  // Name anzeigen
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(38, 32);
  tft.print(_name);

  // Cursor
  int cursorX = 38 + _name.length() * 12;
  if (cursorX < 280) {
    tft.print("_");
  }

  // Zeichenzähler
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setCursor(250, 32);
  tft.printf("%d/%d", _name.length(), KB_MAX_NAME_LENGTH);
}

int KeyboardScreen::handleTouch(int16_t x, int16_t y) {
  // OK-Button prüfen
  if (btnOk->contains(x, y)) {
    if ((int)_name.length() >= KB_MIN_NAME_LENGTH) {
      return 1;  // Name fertig
    } else {
      // Feedback: Name zu kurz
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.setTextSize(1);
      tft.setCursor(100, 52);
      tft.print("Mind. 2 Zeichen!");
      return 0;
    }
  }

  // Abbrechen-Button prüfen
  if (btnCancel->contains(x, y)) {
    return 2;  // Abgebrochen
  }

  // Tastatur prüfen
  for (int i = 0; i < 28; i++) {
    if (keys[i] && keys[i]->contains(x, y)) {
      String label = keys[i]->getLabel();

      if (label == "<-") {
        // Backspace
        if (_name.length() > 0) {
          _name = _name.substring(0, _name.length() - 1);
          updateNameDisplay();
        }
      } else if (label == "LEER") {
        // Leerzeichen
        if ((int)_name.length() < KB_MAX_NAME_LENGTH) {
          _name += " ";
          updateNameDisplay();
        }
      } else {
        // Buchstabe
        if ((int)_name.length() < KB_MAX_NAME_LENGTH) {
          _name += label;
          updateNameDisplay();
        }
      }
      break;
    }
  }

  return 0;  // Weiter
}

String KeyboardScreen::getName() {
  // Trim (führende und folgende Leerzeichen entfernen)
  String trimmed = _name;
  trimmed.trim();
  return trimmed;
}

void KeyboardScreen::setName(const String& name) {
  _name = name;
}

void KeyboardScreen::clear() {
  _name = "";
}
