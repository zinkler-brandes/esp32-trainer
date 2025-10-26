#include "mathe.h"

Mathe::Mathe() {
  // Numpad erstellen: 3x4 Grid
  // Layout: [7][8][9]
  //         [4][5][6]
  //         [1][2][3]
  //         [C][0][OK]

  int startX = 20;
  int startY = 60;
  int btnWidth = 90;
  int btnHeight = 38;
  int spacing = 6;

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

  // Start-Button (rechts oben)
  homeButton = new Button(290, 5, 25, 25, "S");
  homeButton->setColors(TFT_ORANGE, TFT_WHITE);
}

void Mathe::init() {
  // Display initialisieren
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);  // Backlight an

  tft.setRotation(1); // Landscape (320x240)
  tft.fillScreen(TFT_BLACK);

  // Spiel initialisieren
  _score = 0;
  _userInput = "";

  // Zufallsgenerator mit LDR initialisieren (echtes Rauschen!)
  randomSeed(analogRead(34));  // Pin 34 = LDR Lichtsensor
  Serial.printf("Random seed: %d\n", analogRead(34));

  // Erste Aufgabe generieren
  generateTask();

  // Buttons zeichnen
  drawButtons();

  // Display aktualisieren
  updateDisplay();
}

void Mathe::drawButtons() {
  for (int i = 0; i < 12; i++) {
    buttons[i]->draw(&tft);
  }
  homeButton->draw(&tft);
}

bool Mathe::handleButtonPress(int16_t x, int16_t y) {
  // Home-Button prüfen
  if (homeButton->contains(x, y)) {
    Serial.println("HOME pressed - returning to menu");
    return true;  // Zurück zum Menü
  }

  // Numpad-Buttons prüfen
  for (int i = 0; i < 12; i++) {
    if (buttons[i]->contains(x, y)) {
      String label = buttons[i]->getLabel();
      Serial.printf("Button pressed: %s\n", label.c_str());

      if (label == "C") {
        clearInput();
      } else if (label == "OK") {
        checkAnswer();
      } else {
        // Ziffer (0-9)
        addDigit(label);
      }

      break;
    }
  }

  return false;  // Kein Home-Button, weiter im Modus
}

void Mathe::generateTask() {
  // Zufällig Addition oder Subtraktion
  _operation = random(0, 2) == 0 ? '+' : '-';

  if (_operation == '+') {
    // Addition: Beide Zahlen 1-25, Summe <= 50
    _num1 = random(1, 80);
    _num2 = random(1, min(26, 120 - _num1));  // Damit Summe <= 120
    _result = _num1 + _num2;
  } else {
    // Subtraktion: Erste Zahl 10-100, zweite Zahl 1-10, Ergebnis >= 0
    _num2 = random(1, 11);  // Subtrahend max 10
    _num1 = random(_num2, 101);  // Damit Ergebnis >= 0
    _result = _num1 - _num2;
  }

  Serial.printf("New task: %d %c %d = %d\n", _num1, _operation, _num2, _result);
}

void Mathe::updateDisplay() {
  // Score anzeigen (links oben)
  tft.fillRect(0, 0, 100, 30, TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 5);
  tft.printf("# %d", _score);

  // Aufgabe anzeigen (mittig oben, aber nicht über Home-Button)
  tft.fillRect(100, 0, 165, 30, TFT_BLACK);  // Nur bis x=265 (Home-Button startet bei 270)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(110, 5);
  tft.printf("%d %c %d = ?", _num1, _operation, _num2);

  // Home-Button neu zeichnen (falls überschrieben)
  homeButton->draw(&tft);

  // Eingabe anzeigen (unter der Aufgabe)
  tft.fillRect(10, 30, 300, 20, TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 32);
  tft.print("Antwort: ");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print(_userInput);
}

void Mathe::addDigit(String digit) {
  // Maximal 3 Ziffern (Ergebnis <= 100)
  if (_userInput.length() < 3) {
    _userInput += digit;
    updateDisplay();
  }
}

void Mathe::clearInput() {
  _userInput = "";
  updateDisplay();
}

void Mathe::checkAnswer() {
  if (_userInput.length() == 0) {
    return;  // Keine Eingabe
  }

  int answer = _userInput.toInt();
  bool correct = (answer == _result);

  showFeedback(correct);

  if (correct) {
    _score++;
    delay(1500);  // Richtig: 1,5 Sekunden
  } else {
    _score = 0;
    delay(5000);  // Falsch: 3 Sekunden (länger zum Lernen)
  }

  // Neue Aufgabe
  _userInput = "";
  generateTask();
  updateDisplay();
}

void Mathe::showFeedback(bool correct) {
  // Feedback-Bereich (über den Buttons)
  tft.fillRect(0, 0, 320, 50, correct ? TFT_GREEN : TFT_RED);
  tft.setTextColor(TFT_WHITE, correct ? TFT_GREEN : TFT_RED);
  tft.setTextSize(3);

  if (correct) {
    tft.setCursor(80, 15);
    tft.print("RICHTIG!");
  } else {
    tft.setCursor(60, 5);
    tft.printf("FALSCH!");
    tft.setTextSize(2);
    tft.setCursor(60, 28);
    tft.printf("%d %c %d = %d", _num1, _operation, _num2, _result);
  }
}
