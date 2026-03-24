#include "InfoScreen.h"
#include "Button.h"

// Farben
#define INFO_BG       TFT_BLACK
#define INFO_TITLE    TFT_CYAN
#define INFO_TEXT     TFT_WHITE
#define INFO_ICON_BG  0x2104  // Dunkelgrau

InfoScreen::InfoScreen() {
}

void InfoScreen::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

bool InfoScreen::show(InfoType type, Touch* touch) {
  // Display initialisieren
  tft.setRotation(1);
  tft.fillScreen(INFO_BG);

  // Titel und Text je nach Typ
  const char* title = "";
  const char* text = "";

  switch (type) {
    case INFO_SCHREIBEN:
      title = "SCHREIBEN";
      text = "Schreibe das Wort bitte auf ein Blatt Papier ab und lasse danach von deinen Eltern kontrollieren, ob du es richtig abgeschrieben hast.";
      break;
    case INFO_FLAG_QUIZ:
      title = "FLAGGEN-QUIZ";
      text = "Erkenne die Flagge! Welches WM-Land zeigt diese Flagge? Du hast 3 Leben - sammle so viele Punkte wie moeglich!";
      break;
    case INFO_CLUB_QUIZ:
      title = "VEREINE-QUIZ";
      text = "Wo spielt dieser Verein? Tippe auf das richtige Bundesland auf der Deutschlandkarte!";
      break;
    case INFO_STADION_QUIZ:
      title = "STADIEN-QUIZ";
      text = "Zu welchem Verein gehoert dieses Stadion? Erkenne das Stadion und waehle den richtigen Verein!";
      break;
    case INFO_KENNZEICHEN_QUIZ:
      title = "KENNZEICHEN";
      text = "Woher kommt dieses Kennzeichen? Ordne das KFZ-Kennzeichen der richtigen Stadt zu!";
      break;
    case INFO_EINZELSPIEL:
      title = "EINZELSPIEL";
      text = "Trainiere dein Mathe! Loese die Aufgaben bevor die Zeit ablaeuft. Je schneller du antwortest, desto mehr Tore schiesst du!";
      break;
    case INFO_DFB_POKAL:
      title = "DFB-POKAL";
      text = "5 Runden bis zum Sieg! Pro Runde hast du weniger Zeit zum Antworten. Loese Aufgaben richtig, um Tore zu schiessen!";
      break;
    case INFO_CHAMPIONS_LEAGUE:
      title = "CHAMPIONS LEAGUE";
      text = "Besiege die staerksten Clubs Europas! Je staerker der Gegner, desto schneller musst du rechnen.";
      break;
    case INFO_WM:
      title = "WM 2026";
      text = "3 Gruppenspiele + K.O.-Runde! Die Gegner werden staerker - schaffst du es zum Weltmeister?";
      break;
  }

  // Icon zeichnen (zentriert oben)
  int iconX = 130;
  int iconY = 20;
  int iconSize = 60;

  switch (type) {
    case INFO_SCHREIBEN:
      drawPencilIcon(iconX, iconY, iconSize);
      break;
    case INFO_FLAG_QUIZ:
      drawFlagIcon(iconX, iconY, iconSize);
      break;
    case INFO_CLUB_QUIZ:
      drawShieldIcon(iconX, iconY, iconSize);
      break;
    case INFO_STADION_QUIZ:
      drawStadiumIcon(iconX, iconY, iconSize);
      break;
    case INFO_KENNZEICHEN_QUIZ:
      drawLicensePlateIcon(iconX, iconY, iconSize);
      break;
    case INFO_EINZELSPIEL:
      drawCalculatorIcon(iconX, iconY, iconSize);
      break;
    case INFO_DFB_POKAL:
      drawTrophyIcon(iconX, iconY, iconSize, TFT_YELLOW);
      break;
    case INFO_CHAMPIONS_LEAGUE:
      drawTrophyIcon(iconX, iconY, iconSize, TFT_SILVER);
      break;
    case INFO_WM:
      drawTrophyIcon(iconX, iconY, iconSize, TFT_GOLD);
      break;
  }

  // Titel
  tft.setTextColor(INFO_TITLE, INFO_BG);
  tft.setTextSize(2);
  int titleWidth = strlen(title) * 12;
  tft.setCursor((320 - titleWidth) / 2, 90);
  tft.print(title);

  // Text (mit Zeilenumbruch)
  drawWrappedText(text, 15, 115, 290, INFO_TEXT);

  // Zurueck-Button (oben links)
  Button btnZurueck(10, 5, 60, 25, "<");
  btnZurueck.setColors(TFT_DARKGREY, TFT_WHITE);
  btnZurueck.draw(&tft);

  // Weiter-Button (unten zentriert)
  Button btnWeiter(110, 195, 100, 35, "WEITER");
  btnWeiter.setColors(TFT_GREEN, TFT_WHITE);
  btnWeiter.draw(&tft);

  // Auf Touch warten
  unsigned long lastTouch = 0;
  while (true) {
    if (touch->isTouched() && millis() - lastTouch > 200) {
      lastTouch = millis();
      int16_t x, y;
      touch->getCoordinates(x, y);
      if (btnWeiter.contains(x, y)) {
        return true;  // Weiter
      }
      if (btnZurueck.contains(x, y)) {
        return false;  // Zurueck
      }
    }
    delay(20);
  }

  return true;
}

void InfoScreen::drawWrappedText(const char* text, int x, int y, int maxWidth, uint16_t color) {
  tft.setTextColor(color, INFO_BG);
  tft.setTextSize(1);

  int cursorX = x;
  int cursorY = y;
  int charWidth = 6;
  int lineHeight = 12;
  int maxCharsPerLine = maxWidth / charWidth;

  String word = "";
  int lineLen = 0;

  for (int i = 0; text[i] != '\0'; i++) {
    char c = text[i];

    if (c == ' ' || c == '\n') {
      // Wort ausgeben
      if (lineLen + word.length() > (unsigned int)maxCharsPerLine) {
        // Neue Zeile
        cursorY += lineHeight;
        cursorX = x;
        lineLen = 0;
      }

      tft.setCursor(cursorX + lineLen * charWidth, cursorY);
      tft.print(word);
      lineLen += word.length();

      if (c == ' ' && lineLen < (unsigned int)maxCharsPerLine) {
        tft.print(" ");
        lineLen++;
      }

      word = "";

      if (c == '\n') {
        cursorY += lineHeight;
        cursorX = x;
        lineLen = 0;
      }
    } else {
      word += c;
    }
  }

  // Letztes Wort
  if (word.length() > 0) {
    if (lineLen + word.length() > (unsigned int)maxCharsPerLine) {
      cursorY += lineHeight;
      cursorX = x;
      lineLen = 0;
    }
    tft.setCursor(cursorX + lineLen * charWidth, cursorY);
    tft.print(word);
  }
}

// === Icon-Zeichenfunktionen ===

void InfoScreen::drawPencilIcon(int x, int y, int size) {
  // Bleistift (gelb/orange)
  uint16_t yellow = TFT_YELLOW;
  uint16_t orange = 0xFD20;
  uint16_t wood = 0xDEB0;
  uint16_t gray = TFT_DARKGREY;

  // Stift-Koerper (diagonal)
  int w = size * 2 / 3;
  int h = size / 5;

  // Gelber Koerper
  tft.fillRect(x, y + size / 3, w, h, yellow);

  // Spitze (Dreieck)
  tft.fillTriangle(x + w, y + size / 3,
                   x + w, y + size / 3 + h,
                   x + w + size / 4, y + size / 3 + h / 2, wood);

  // Graphit-Spitze
  tft.fillTriangle(x + w + size / 4 - 3, y + size / 3 + h / 2 - 2,
                   x + w + size / 4 - 3, y + size / 3 + h / 2 + 2,
                   x + w + size / 4 + 2, y + size / 3 + h / 2, gray);

  // Radiergummi
  tft.fillRect(x - size / 6, y + size / 3, size / 6, h, orange);
}

void InfoScreen::drawFlagIcon(int x, int y, int size) {
  // Flagge mit Fragezeichen
  uint16_t red = TFT_RED;
  uint16_t white = TFT_WHITE;
  uint16_t blue = TFT_BLUE;

  // Flaggen-Mast
  tft.fillRect(x, y, 3, size, TFT_DARKGREY);

  // Flagge (Trikolore)
  int flagW = size * 2 / 3;
  int flagH = size / 2;
  int flagX = x + 5;
  int flagY = y + 5;

  int stripeW = flagW / 3;
  tft.fillRect(flagX, flagY, stripeW, flagH, blue);
  tft.fillRect(flagX + stripeW, flagY, stripeW, flagH, white);
  tft.fillRect(flagX + 2 * stripeW, flagY, stripeW, flagH, red);

  // Fragezeichen
  tft.setTextColor(TFT_YELLOW, white);
  tft.setTextSize(2);
  tft.setCursor(flagX + flagW / 2 - 5, flagY + flagH / 2 - 7);
  tft.print("?");
}

void InfoScreen::drawShieldIcon(int x, int y, int size) {
  // Wappen/Schild (rot/weiss)
  uint16_t red = TFT_RED;
  uint16_t white = TFT_WHITE;

  int w = size * 2 / 3;
  int h = size;

  // Schild-Form
  tft.fillRect(x, y, w, h * 2 / 3, red);
  tft.fillTriangle(x, y + h * 2 / 3, x + w, y + h * 2 / 3, x + w / 2, y + h, red);

  // Weisse Streifen
  tft.fillRect(x + w / 4, y + h / 6, w / 8, h / 3, white);
  tft.fillRect(x + w / 2, y + h / 6, w / 8, h / 3, white);
}

void InfoScreen::drawStadiumIcon(int x, int y, int size) {
  // Stadion-Silhouette (gruen)
  uint16_t green = TFT_DARKGREEN;
  uint16_t gray = TFT_DARKGREY;

  int w = size;
  int h = size * 2 / 3;

  // Basis (Ellipse vereinfacht als Rechteck mit runden Ecken)
  tft.fillRect(x, y + h / 2, w, h / 2, gray);

  // Tribuenen (links und rechts)
  tft.fillRect(x - 5, y + h / 4, 10, h / 2, gray);
  tft.fillRect(x + w - 5, y + h / 4, 10, h / 2, gray);

  // Spielfeld (gruen)
  tft.fillRect(x + 5, y + h / 2 + 5, w - 10, h / 2 - 10, green);

  // Mittelkreis
  tft.drawCircle(x + w / 2, y + h / 2 + h / 4, 5, TFT_WHITE);
}

void InfoScreen::drawLicensePlateIcon(int x, int y, int size) {
  // Nummernschild (weiss mit blauem Rand)
  uint16_t white = TFT_WHITE;
  uint16_t blue = TFT_BLUE;

  int w = size;
  int h = size / 2;

  // Weisses Schild
  tft.fillRect(x, y + size / 4, w, h, white);
  tft.drawRect(x, y + size / 4, w, h, TFT_BLACK);

  // Blauer Streifen links (EU)
  tft.fillRect(x + 2, y + size / 4 + 2, 8, h - 4, blue);

  // Fragezeichen
  tft.setTextColor(TFT_BLACK, white);
  tft.setTextSize(2);
  tft.setCursor(x + w / 2 - 5, y + size / 4 + h / 2 - 7);
  tft.print("?");
}

void InfoScreen::drawCalculatorIcon(int x, int y, int size) {
  // Taschenrechner (blau)
  uint16_t body = TFT_NAVY;
  uint16_t screen = 0x07E0;  // Gruen
  uint16_t button = TFT_LIGHTGREY;

  int w = size * 2 / 3;
  int h = size;

  // Koerper
  tft.fillRect(x, y, w, h, body);

  // Display
  tft.fillRect(x + 4, y + 4, w - 8, h / 4, screen);
  tft.setTextColor(TFT_BLACK, screen);
  tft.setTextSize(1);
  tft.setCursor(x + 6, y + 8);
  tft.print("123");

  // Tasten (3x3 Grid)
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      int bx = x + 4 + col * (w - 8) / 3;
      int by = y + h / 3 + row * h / 5;
      tft.fillRect(bx, by, (w - 12) / 3, h / 6, button);
    }
  }
}

void InfoScreen::drawTrophyIcon(int x, int y, int size, uint16_t color) {
  // Pokal
  int w = size * 2 / 3;
  int h = size;

  // Kelch
  tft.fillRect(x + w / 6, y, w * 2 / 3, h / 2, color);

  // Henkel
  tft.drawArc(x + w / 6 - 5, y + h / 4, 8, 5, 180, 360, color, color);
  tft.drawArc(x + w * 5 / 6 + 5, y + h / 4, 8, 5, 0, 180, color, color);

  // Stiel
  tft.fillRect(x + w / 3, y + h / 2, w / 3, h / 4, color);

  // Fuss
  tft.fillRect(x + w / 6, y + h * 3 / 4, w * 2 / 3, h / 6, color);
}
