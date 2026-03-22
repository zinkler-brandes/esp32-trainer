#ifndef SRC_TOURNAMENTINTRO_H
#define SRC_TOURNAMENTINTRO_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Tournament.h"
#include "Button.h"
#include "Touch.h"

// Offizielle Farben DFB-Pokal (Grün, Gold, Weiß)
#define DFB_GREEN     0x0400  // Offizielles DFB-Grün (dunkel)
#define DFB_GREEN_LIGHT 0x07E0  // Hellgrün
#define DFB_GOLD      0xFE60  // Gold
#define DFB_WHITE     TFT_WHITE

// Offizielle Farben Champions League (Blau, Weiß, Silber)
#define CL_BLUE       0x000F  // Dunkles UEFA-Blau
#define CL_BLUE_LIGHT 0x001F  // Helleres Blau
#define CL_WHITE      TFT_WHITE
#define CL_SILVER     0xC618

// Offizielle Farben WM (Gold, Rot, Schwarz)
#define WM_GOLD       0xFE60
#define WM_RED        0xF800
#define WM_GREEN      0x07E0

class TournamentIntro {
  public:
    TournamentIntro();

    // Initialisieren
    void init();

    // Ladebildschirm anzeigen (wartet auf Button-Druck)
    void showIntro(TournamentType type, Touch* touch);

  private:
    TFT_eSPI tft;
    Button* weiterButton;

    // Pokal-Zeichenfunktionen
    void drawDFBPokal(int centerX, int startY, int pixelSize);
    void drawHenkelpott(int centerX, int startY, int pixelSize);
    void drawWorldCupTrophy(int centerX, int startY, int pixelSize);

    // Trivia
    const char* getDFBTrivia(int index);
    const char* getCLTrivia(int index);
    const char* getWMTrivia(int index);

    static const int TRIVIA_COUNT = 50;
};

#endif
