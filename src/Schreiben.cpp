#include "Schreiben.h"

// Wortlisten - einfach fuer 5-Jaehrige, nur GROSSBUCHSTABEN

// Familie/Vornamen
const char* VORNAMEN[] = {
  "MAMA", "PAPA", "TIM", "LEO", "JANO", "JASPER", "BJARNE", "IDA"
};
const int NUM_VORNAMEN = 8;

// Nachnamen (bekannte Fussballer)
const char* NACHNAMEN[] = {
  "KAHN", "LAHM", "REUS", "KROOS", "SANE", "NEUER", "MUELLER",
  "GOETZE", "WIRTZ", "BRANDT", "HUMMELS", "KIMMICH", "HAVERTZ",
  "GINTER", "RUDIGER", "TER STEGEN", "MUSIALA", "GNABRY",
  // Aktuelle Bayern-Spieler
  "KANE", "DAVIES", "COMAN", "GORETZKA", "LAIMER", "OLISE",
  "DE LIGT", "UPAMECANO", "KIM", "GUERREIRO", "PALHINHA"
};
const int NUM_NACHNAMEN = 29;

// Bundesliga-Teams
const char* TEAMS[] = {
  "BAYERN", "DORTMUND", "BREMEN", "KOELN", "BERLIN", "MAINZ",
  "BOCHUM", "LEIPZIG", "FREIBURG", "WOLFSBURG", "STUTTGART",
  "FRANKFURT", "GLADBACH", "HOFFENHEIM"
};
const int NUM_TEAMS = 14;

Schreiben::Schreiben() : _currentWord(""), _wordsCompleted(0), _stepPosition(0),
                         _usedVornamen(nullptr), _usedNachnamen(nullptr), _usedTeams(nullptr),
                         _remainingVornamen(0), _remainingNachnamen(0), _remainingTeams(0) {
  // Weiter-Button (gross, unten)
  _weiterButton = new Button(60, 190, 200, 45, "WEITER");
  _weiterButton->setColors(0x07E0, TFT_BLACK);  // Gruen

  // Zurueck-Button (oben links, klein)
  _backButton = new Button(5, 5, 35, 30, "<");
  _backButton->setColors(TFT_DARKGREY, TFT_WHITE);

  // Tracking-Arrays allokieren
  _usedVornamen = new bool[NUM_VORNAMEN];
  _usedNachnamen = new bool[NUM_NACHNAMEN];
  _usedTeams = new bool[NUM_TEAMS];
}

void Schreiben::init() {
  _tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  _tft.setRotation(1);

  _field.setDisplay(&_tft);
  _field.setStepsForGoal(5);
  _field.setPlayerTeamColor(TFT_GREEN);

  _wordsCompleted = 0;
  _stepPosition = 0;

  // Zufallsgenerator initialisieren
  randomSeed(analogRead(0) + millis());

  // Wort-Pool zuruecksetzen
  resetWordPool();

  nextWord();
  draw();
}

void Schreiben::resetWordPool() {
  // Alle Woerter als unbenutzt markieren
  for (int i = 0; i < NUM_VORNAMEN; i++) {
    _usedVornamen[i] = false;
  }
  for (int i = 0; i < NUM_NACHNAMEN; i++) {
    _usedNachnamen[i] = false;
  }
  for (int i = 0; i < NUM_TEAMS; i++) {
    _usedTeams[i] = false;
  }
  _remainingVornamen = NUM_VORNAMEN;
  _remainingNachnamen = NUM_NACHNAMEN;
  _remainingTeams = NUM_TEAMS;
}

void Schreiben::nextWord() {
  // Pruefen ob alle Woerter verbraucht sind
  if (_remainingVornamen == 0 && _remainingNachnamen == 0 && _remainingTeams == 0) {
    resetWordPool();
  }

  // Kategorie waehlen (nur aus verfuegbaren)
  // 0 = Vornamen, 1 = Nachnamen, 2 = Teams
  int availableCategories = 0;
  if (_remainingVornamen > 0) availableCategories++;
  if (_remainingNachnamen > 0) availableCategories++;
  if (_remainingTeams > 0) availableCategories++;

  int categoryPick = random(0, availableCategories);
  int category = -1;
  int count = 0;
  if (_remainingVornamen > 0) {
    if (count == categoryPick) category = 0;
    count++;
  }
  if (_remainingNachnamen > 0) {
    if (count == categoryPick) category = 1;
    count++;
  }
  if (_remainingTeams > 0) {
    if (count == categoryPick) category = 2;
  }

  if (category == 0) {
    // Zufaelligen unbenutzten Vornamen waehlen
    int pick = random(0, _remainingVornamen);
    int idx = 0;
    for (int i = 0; i < NUM_VORNAMEN; i++) {
      if (!_usedVornamen[i]) {
        if (idx == pick) {
          _currentWord = VORNAMEN[i];
          _usedVornamen[i] = true;
          _remainingVornamen--;
          break;
        }
        idx++;
      }
    }
  } else if (category == 1) {
    // Zufaelligen unbenutzten Nachnamen waehlen
    int pick = random(0, _remainingNachnamen);
    int idx = 0;
    for (int i = 0; i < NUM_NACHNAMEN; i++) {
      if (!_usedNachnamen[i]) {
        if (idx == pick) {
          _currentWord = NACHNAMEN[i];
          _usedNachnamen[i] = true;
          _remainingNachnamen--;
          break;
        }
        idx++;
      }
    }
  } else {
    // Zufaelliges unbenutztes Team waehlen
    int pick = random(0, _remainingTeams);
    int idx = 0;
    for (int i = 0; i < NUM_TEAMS; i++) {
      if (!_usedTeams[i]) {
        if (idx == pick) {
          _currentWord = TEAMS[i];
          _usedTeams[i] = true;
          _remainingTeams--;
          break;
        }
        idx++;
      }
    }
  }
}

void Schreiben::draw() {
  _tft.fillScreen(TFT_BLACK);

  // Anzeigetafel oben (schwarzer Bereich mit Wort)
  drawScoreboard();

  // Spielfeld zeichnen (ohne Scoreboard, nur Feld)
  _field.drawField(0);
  _field.drawGoalkeeper();

  // Spieler an aktueller Position
  drawPlayer();

  // Zaehler unten links
  _tft.setTextColor(TFT_WHITE, TFT_BLACK);
  _tft.setTextSize(2);
  _tft.setCursor(5, 165);
  _tft.printf("WOERTER: %d", _wordsCompleted);

  // Buttons
  _backButton->draw(&_tft);
  _weiterButton->draw(&_tft);
}

void Schreiben::drawScoreboard() {
  // Schwarze Anzeigetafel oben
  _tft.fillRect(0, 0, 320, 55, TFT_BLACK);
  _tft.drawRect(2, 2, 316, 51, TFT_DARKGREY);

  // Wort gross und zentriert
  _tft.setTextColor(TFT_WHITE, TFT_BLACK);
  _tft.setTextSize(4);
  _tft.setTextDatum(MC_DATUM);  // Middle Center

  // Wort zentrieren
  _tft.drawString(_currentWord, 160, 28);
}

void Schreiben::drawPlayer() {
  // Spieler auf dem Feld zeichnen
  // Position 0-4, nach 5 Woertern schiesst er
  _field.drawPlayer(_stepPosition, true);
}

void Schreiben::showJubelAnimation() {
  // Kurzer Jubel (ca. 1 Sekunde, 4 Frames)
  for (int frame = 0; frame < 4; frame++) {
    _field.drawJubilatingSpectators(frame);
    delay(250);
  }
}

void Schreiben::showGoalAnimation() {
  // Volle Tor-Animation wie in SoccerField
  _field.showGoalAnimation(_stepPosition);
}

int Schreiben::handleTouch(int16_t x, int16_t y) {
  // Zurueck-Button
  if (_backButton->contains(x, y)) {
    return 0;  // Zurueck zum Menue
  }

  // Weiter-Button
  if (_weiterButton->contains(x, y)) {
    _wordsCompleted++;
    _stepPosition++;

    if (_stepPosition >= 5) {
      // Nach 5 Woertern: TOR!
      showGoalAnimation();
      _stepPosition = 0;
    } else {
      // Kurzer Jubel
      showJubelAnimation();
    }

    // Naechstes Wort
    nextWord();
    draw();
    return -1;
  }

  return -1;  // Nichts getroffen
}
