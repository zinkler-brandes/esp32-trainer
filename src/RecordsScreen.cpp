#include "RecordsScreen.h"

// Grid-Layout Konstanten (kompakter)
#define GRID_START_X 55
#define GRID_START_Y 58
#define CELL_WIDTH 52
#define CELL_HEIGHT 30
#define HEADER_HEIGHT 14

// Turnier-Namen fuer Spalten
const char* TOURNAMENT_LABELS[] = {"DFB", "CL", "WM"};

// Rechenarten fuer Zeilen (4 Zeilen: +/-, 1x1, 1:1, Alle)
const char* MATHE_LABELS[] = {"+/-", "1x1", "1:1", "Alle"};

// Anzahl der Rechenarten
#define MATHE_COUNT 4

// Farben fuer die Turniere
const uint16_t TOURNAMENT_COLORS[] = {
  TFT_RED,      // DFB-Pokal
  0x001F,       // CL Blau
  0xFE60        // WM Gold
};

RecordsScreen::RecordsScreen() {
  backButton = new Button(10, 8, 40, 30, "<");
  backButton->setColors(TFT_DARKGREY, TFT_WHITE);
}

void RecordsScreen::init(const RecordsMatrix& records, const String& playerName,
                         int flagQuizMedal, int clubQuizMedal,
                         int stadionQuizMedal, int kennzeichenQuizMedal) {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);

  _records = records;
  _playerName = playerName;
  _flagQuizMedal = flagQuizMedal;
  _clubQuizMedal = clubQuizMedal;
  _stadionQuizMedal = stadionQuizMedal;
  _kennzeichenQuizMedal = kennzeichenQuizMedal;
}

void RecordsScreen::draw() {
  tft.fillScreen(TFT_BLACK);

  // Header
  backButton->draw(&tft);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 12);
  tft.print("REKORDEMATRIX");

  // Spielername und Fortschritt
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 38);
  tft.printf("%s: %d/12 Siege", _playerName.c_str(), _records.countWins());

  // Spezial-Nachricht wenn komplett
  if (_records.isComplete()) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(180, 38);
    tft.print("KOMPLETT!");
  }

  drawGrid();
  drawQuizMedals();
}

void RecordsScreen::drawGrid() {
  // Spalten-Header (Turniere)
  tft.setTextSize(1);
  for (int col = 0; col < 3; col++) {
    int x = GRID_START_X + col * CELL_WIDTH + CELL_WIDTH / 2;
    int y = GRID_START_Y - HEADER_HEIGHT;

    tft.setTextColor(TOURNAMENT_COLORS[col], TFT_BLACK);
    // Zentrieren
    int textWidth = strlen(TOURNAMENT_LABELS[col]) * 6;
    tft.setCursor(x - textWidth / 2, y);
    tft.print(TOURNAMENT_LABELS[col]);
  }

  // Zeilen-Header (Rechenarten) und Zellen - jetzt 4 Zeilen
  for (int row = 0; row < MATHE_COUNT; row++) {
    // Rechenart links
    int labelY = GRID_START_Y + row * CELL_HEIGHT + CELL_HEIGHT / 2 - 4;
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(10, labelY);
    tft.print(MATHE_LABELS[row]);

    // Zellen zeichnen
    for (int col = 0; col < 3; col++) {
      drawCell(row, col, _records.isWon(col, row));
    }
  }
}

void RecordsScreen::drawCell(int row, int col, bool won) {
  int x = GRID_START_X + col * CELL_WIDTH;
  int y = GRID_START_Y + row * CELL_HEIGHT;

  // Zellrahmen
  uint16_t borderColor = won ? TOURNAMENT_COLORS[col] : TFT_DARKGREY;
  tft.drawRect(x, y, CELL_WIDTH, CELL_HEIGHT, borderColor);
  tft.drawRect(x + 1, y + 1, CELL_WIDTH - 2, CELL_HEIGHT - 2, borderColor);

  // Inhalt
  if (won) {
    drawMiniTrophy(x + CELL_WIDTH / 2, y + CELL_HEIGHT / 2, col);
  } else {
    drawEmptyCell(x + CELL_WIDTH / 2, y + CELL_HEIGHT / 2);
  }
}

void RecordsScreen::drawMiniTrophy(int cx, int cy, int tournamentType) {
  // Mini-Pokal basierend auf Turnier-Typ (kleiner fuer kompakteres Grid)
  uint16_t trophyColor = TOURNAMENT_COLORS[tournamentType];

  // Vereinfachter Mini-Pokal (skaliert)
  // Kelch oben
  tft.fillRect(cx - 6, cy - 8, 12, 10, trophyColor);
  tft.fillRect(cx - 8, cy - 6, 16, 5, trophyColor);

  // Sockel
  tft.fillRect(cx - 2, cy + 2, 4, 3, trophyColor);
  tft.fillRect(cx - 5, cy + 5, 10, 3, trophyColor);

  // Henkel (nur bei CL)
  if (tournamentType == 1) {
    tft.drawCircle(cx - 8, cy - 4, 2, trophyColor);
    tft.drawCircle(cx + 8, cy - 4, 2, trophyColor);
  }

  // Stern oben (bei WM)
  if (tournamentType == 2) {
    tft.fillTriangle(cx, cy - 12, cx - 2, cy - 8, cx + 2, cy - 8, trophyColor);
  }
}

void RecordsScreen::drawEmptyCell(int cx, int cy) {
  // Fragezeichen (kleiner)
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(cx - 6, cy - 8);
  tft.print("?");
}

void RecordsScreen::drawQuizMedals() {
  // Quiz-Medaillen unter dem Grid anzeigen (2x2 Layout)
  int y1 = GRID_START_Y + MATHE_COUNT * CELL_HEIGHT + 8;
  int y2 = y1 + 18;

  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Zeile 1: Flaggen und Vereine
  tft.setCursor(15, y1);
  tft.print("Flaggen:");
  drawMedal(70, y1 - 2, _flagQuizMedal);

  tft.setCursor(115, y1);
  tft.print("Vereine:");
  drawMedal(170, y1 - 2, _clubQuizMedal);

  // Zeile 2: Stadien und Kennzeichen
  tft.setCursor(15, y2);
  tft.print("Stadien:");
  drawMedal(70, y2 - 2, _stadionQuizMedal);

  tft.setCursor(115, y2);
  tft.print("Kennz.:");
  drawMedal(165, y2 - 2, _kennzeichenQuizMedal);

  // Quiz-Zaehler rechts
  int quizWins = 0;
  if (_flagQuizMedal > 0) quizWins++;
  if (_clubQuizMedal > 0) quizWins++;
  if (_stadionQuizMedal > 0) quizWins++;
  if (_kennzeichenQuizMedal > 0) quizWins++;

  tft.setCursor(220, y1 + 8);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.printf("Quiz: %d/4", quizWins);
}

void RecordsScreen::drawMedal(int x, int y, int medal) {
  if (medal == 0) {
    // Keine Medaille - Fragezeichen
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(x, y);
    tft.print("?");
    return;
  }

  // Medaillen-Farben
  uint16_t color;
  switch (medal) {
    case 1: color = 0xCD60; break;  // Bronze
    case 2: color = 0xC618; break;  // Silber
    case 3: color = 0xFE60; break;  // Gold
    default: color = TFT_DARKGREY; break;
  }

  // Medaille zeichnen (kleiner Kreis mit Band)
  int cx = x + 6;
  int cy = y + 6;

  // Band
  tft.fillTriangle(cx - 3, cy - 8, cx + 3, cy - 8, cx, cy - 3, TFT_RED);

  // Medaillen-Kreis
  tft.fillCircle(cx, cy, 5, color);
  tft.drawCircle(cx, cy, 5, TFT_WHITE);
}

int RecordsScreen::handleTouch(int16_t x, int16_t y) {
  if (backButton->contains(x, y)) {
    return 0;  // Zurueck
  }
  return -1;  // Nichts
}
