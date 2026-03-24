#include "ReitIntro.h"
#include "SpringreitenTrainer.h"

// Reit-Trivia
static const char* REIT_TRIVIA[] = {
  "Springreiten ist seit 1912 olympisch.",
  "Der hoechste Sprung war 2,47m (Huaso, 1949).",
  "Fehler gibt es fuer Abwuerfe und Verweigerungen.",
  "Im Stechen zaehlt die schnellste Zeit.",
  "Ein Abwurf gibt 4 Strafpunkte.",
  "Verweigerung gibt ebenfalls 4 Punkte.",
  "3 Verweigerungen = Disqualifikation.",
  "Das Pferd muss mind. 6 Jahre alt sein.",
  "Der CHIO Aachen ist das groesste Turnier.",
  "Ludger Beerbaum gewann 4x Olympia-Gold.",
  "Hans Guenter Winkler gewann 5x Olympia-Gold.",
  "Ein Oxer hat 2 Stangen hintereinander.",
  "Ein Steilsprung hat nur 1 Stange.",
  "Der Wassergraben ist mind. 4m breit.",
  "Parcours haben 10-16 Hindernisse.",
  "Die Hoehe geht bis 1,60m bei Profis.",
  "Pferde springen bis 2m hoch.",
  "Reithelme sind Pflicht beim Springen.",
  "Der Reiter lenkt mit Zuegel und Schenkeln.",
  "Springpferde sind meist Warmblut-Rassen.",
  "Holsteiner sind beliebte Springpferde.",
  "Hannoveraner sind ebenfalls top.",
  "Ein Parcours dauert ca. 60-90 Sekunden.",
  "Zeitueberschreitung gibt Strafpunkte.",
  "Der Start ist zwischen zwei Flaggen.",
  "Rote Flagge = rechts passieren.",
  "Weisse Flagge = links passieren.",
  "Die Glocke signalisiert Start/Stopp.",
  "Pferde tragen oft Gamaschen zum Schutz.",
  "Die Hufeisen sind speziell fuer Grip."
};
static const int TRIVIA_COUNT = 30;

ReitIntro::ReitIntro() {
  weiterButton = new Button(60, 200, 200, 35, "LOS GEHT'S!");
}

void ReitIntro::init() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

void ReitIntro::showIntro(Reitabzeichen level, Touch* touch) {
  // Display initialisieren
  tft.init();
  tft.setRotation(1);

  randomSeed(millis());
  int triviaIndex = random(0, TRIVIA_COUNT);

  // Farben je nach Level
  uint16_t primaryColor;
  const char* levelName;
  int obstacles, answerTime;

  if (level == REITABZEICHEN_BRONZE) {
    primaryColor = 0xCD30;  // Bronze
    levelName = "BRONZE";
    obstacles = 6;
    answerTime = 15;
  } else if (level == REITABZEICHEN_SILBER) {
    primaryColor = 0xC618;  // Silber
    levelName = "SILBER";
    obstacles = 8;
    answerTime = 10;
  } else {
    primaryColor = 0xFE00;  // Gold
    levelName = "GOLD";
    obstacles = 10;
    answerTime = 6;
  }

  // Schwarzer Hintergrund (wie beim Fussball-Intro)
  tft.fillScreen(TFT_BLACK);

  // Titel gross und zentriert
  tft.setTextColor(primaryColor, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(35, 10);
  tft.print("SPRINGREITEN");

  // Level-Name
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(70, 45);
  tft.print("Reitabzeichen ");
  tft.setTextColor(primaryColor, TFT_BLACK);
  tft.print(levelName);

  // Trennlinie
  tft.drawLine(20, 70, 300, 70, primaryColor);

  // Spielregeln - groesser und uebersichtlicher
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.setCursor(20, 85);
  tft.print("Hindernisse: ");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print(obstacles);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(20, 110);
  tft.print("Antwortzeit: ");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.printf("%d Sek.", answerTime);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(20, 135);
  tft.print("Fehler:      ");
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.print("+4");

  // Trennlinie
  tft.drawLine(20, 160, 300, 160, primaryColor);

  // Trivia (kleiner, unten)
  tft.setTextSize(1);
  tft.setTextColor(0x8410, TFT_BLACK);  // Grau
  tft.setCursor(20, 170);
  tft.print(REIT_TRIVIA[triviaIndex]);

  // Weiter-Button
  weiterButton->setColors(primaryColor, TFT_BLACK);
  weiterButton->draw(&tft);

  // Auf Button warten
  bool waiting = true;
  while (waiting) {
    if (touch->isTouched()) {
      int16_t x, y;
      touch->getCoordinates(x, y);
      if (weiterButton->contains(x, y)) {
        waiting = false;
      }
      delay(150);
    }
    delay(10);
  }
}

void ReitIntro::drawHorseJumping(int x, int y) {
  uint16_t brown = 0x8200;
  uint16_t lightBrown = 0xA285;

  // Körper (oval, nach oben geneigt)
  tft.fillEllipse(x, y + 10, 28, 14, brown);
  tft.fillEllipse(x, y + 12, 24, 10, lightBrown);

  // Hals (schräg nach oben)
  tft.fillTriangle(x + 18, y + 2, x + 35, y - 25, x + 14, y + 14, brown);

  // Kopf (länglich)
  tft.fillEllipse(x + 38, y - 28, 12, 7, brown);
  // Nüstern
  tft.fillCircle(x + 48, y - 26, 2, TFT_BLACK);
  // Auge
  tft.fillCircle(x + 40, y - 32, 3, TFT_BLACK);
  tft.fillCircle(x + 41, y - 32, 1, TFT_WHITE);

  // Ohren (spitz)
  tft.fillTriangle(x + 32, y - 40, x + 35, y - 32, x + 29, y - 32, brown);
  tft.fillTriangle(x + 42, y - 40, x + 45, y - 32, x + 39, y - 32, brown);

  // Mähne
  for (int i = 0; i < 6; i++) {
    int mx = x + 20 + i * 3;
    int my = y - 5 - i * 4;
    tft.drawLine(mx, my, mx - 6, my + 8, 0x4100);
    tft.drawLine(mx + 1, my, mx - 5, my + 8, 0x4100);
  }

  // Vorderbeine (angezogen beim Sprung)
  tft.fillRect(x - 8, y + 18, 6, 14, brown);
  tft.fillRect(x + 2, y + 20, 6, 12, brown);

  // Hinterbeine (gestreckt)
  tft.fillRect(x + 12, y + 18, 6, 16, brown);
  tft.fillRect(x + 22, y + 16, 6, 18, brown);

  // Hufe
  tft.fillRect(x - 9, y + 31, 8, 4, TFT_BLACK);
  tft.fillRect(x + 1, y + 31, 8, 4, TFT_BLACK);
  tft.fillRect(x + 11, y + 33, 8, 4, TFT_BLACK);
  tft.fillRect(x + 21, y + 33, 8, 4, TFT_BLACK);

  // Schweif (wehend)
  tft.fillTriangle(x - 28, y + 5, x - 42, y - 8, x - 25, y + 18, 0x4100);
  tft.fillTriangle(x - 30, y + 8, x - 45, y - 5, x - 28, y + 20, 0x4100);

  // Reiter (vereinfacht)
  int riderX = x + 2;
  int riderY = y - 8;

  // Stiefel
  tft.fillRect(riderX - 4, riderY + 12, 5, 14, TFT_BLACK);
  tft.fillRect(riderX + 5, riderY + 12, 5, 14, TFT_BLACK);

  // Reithose
  tft.fillRect(riderX - 4, riderY + 6, 14, 8, 0xE71C);

  // Jacke
  tft.fillRect(riderX - 5, riderY - 8, 16, 16, 0x0010);

  // Kopf
  tft.fillCircle(riderX + 3, riderY - 14, 6, 0xFE60);

  // Helm
  tft.fillEllipse(riderX + 3, riderY - 20, 7, 4, TFT_BLACK);
  tft.fillRect(riderX - 5, riderY - 18, 16, 3, TFT_BLACK);

  // Hindernis darunter
  int obsX = x + 55;
  int obsY = y + 25;
  // Ständer
  tft.fillRect(obsX - 15, obsY - 20, 4, 25, TFT_WHITE);
  tft.fillRect(obsX + 11, obsY - 20, 4, 25, TFT_WHITE);
  // Stange
  tft.fillRect(obsX - 12, obsY - 15, 24, 4, TFT_RED);
  tft.fillRect(obsX - 12, obsY - 12, 24, 3, TFT_WHITE);
}

void ReitIntro::drawRibbon(int x, int y, uint16_t color) {
  // Rosette/Schleife
  tft.fillCircle(x, y, 12, color);
  tft.fillCircle(x, y, 8, TFT_WHITE);
  tft.fillCircle(x, y, 5, color);

  // Bänder
  tft.fillTriangle(x - 8, y + 10, x - 2, y + 10, x - 10, y + 28, color);
  tft.fillTriangle(x + 8, y + 10, x + 2, y + 10, x + 10, y + 28, color);
}
