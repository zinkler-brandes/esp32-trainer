#include "mathe.h"

Mathe::Mathe() {
  // Numpad erstellen: 3x4 Grid
  // Layout: [7][8][9]
  //         [4][5][6]
  //         [1][2][3]
  //         [C][0][OK]

  int startX = 20;
  int startY = 50;
  int btnWidth = 90;
  int btnHeight = 40;
  int spacing = 8;

  // Reihe 1: 7, 8, 9
  buttons[0] = new Button(startX, startY, btnWidth, btnHeight, "7");
  buttons[1] = new Button(startX + (btnWidth + spacing), startY, btnWidth, btnHeight, "8");
  buttons[2] = new Button(startX + 2*(btnWidth + spacing), startY, btnWidth, btnHeight, "9");

  // Reihe 2: 4, 5, 6
  buttons[3] = new Button(startX, startY + (btnHeight + spacing), btnWidth, btnHeight, "4");
  buttons[4] = new Button(startX + (btnWidth + spacing), startY + (btnHeight + spacing), btnWidth, btnHeight, "5");
  buttons[5] = new Button(startX + 2*(btnWidth + spacing), startY + (btnHeight + spacing), btnWidth, btnHeight, "6");

  // Reihe 3: 1, 2, 3
  buttons[6] = new Button(startX, startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "1");
  buttons[7] = new Button(startX + (btnWidth + spacing), startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "2");
  buttons[8] = new Button(startX + 2*(btnWidth + spacing), startY + 2*(btnHeight + spacing), btnWidth, btnHeight, "3");

  // Reihe 4: C, 0, OK
  buttons[9] = new Button(startX, startY + 3*(btnHeight + spacing), btnWidth, btnHeight, "C");
  buttons[9]->setColors(TFT_RED, TFT_WHITE);  // C = rot

  buttons[10] = new Button(startX + (btnWidth + spacing), startY + 3*(btnHeight + spacing), btnWidth, btnHeight, "0");

  buttons[11] = new Button(startX + 2*(btnWidth + spacing), startY + 3*(btnHeight + spacing), btnWidth, btnHeight, "OK");
  buttons[11]->setColors(TFT_GREEN, TFT_WHITE);  // OK = grün
}

void Mathe::init() {
  // Display initialisieren
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);  // Backlight an

  tft.setRotation(1); // Landscape (320x240)
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Leos Mathe-Trainer");

  // Buttons zeichnen
  drawButtons();
}

void Mathe::drawButtons() {
  for (int i = 0; i < 12; i++) {
    buttons[i]->draw(&tft);
  }
}

void Mathe::handleButtonPress(int16_t x, int16_t y) {
  // Prüfen welcher Button gedrückt wurde
  for (int i = 0; i < 12; i++) {
    if (buttons[i]->contains(x, y)) {
      Serial.printf("Button pressed: %s\n", buttons[i]->getLabel().c_str());

      // TODO: Hier die Logik für den Mathe-Trainer einbauen
      // - Ziffer zur Antwort hinzufügen
      // - C: Antwort löschen
      // - OK: Antwort prüfen

      break;
    }
  }
}
