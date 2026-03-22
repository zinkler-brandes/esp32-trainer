#include "TournamentIntro.h"

// ========== DFB-POKAL TRIVIA ==========
static const char* DFB_TRIVIA[] = {
  "Der DFB-Pokal wurde 1935 gegruendet.",
  "Bayern Muenchen hat den Pokal am haeufigsten gewonnen.",
  "Das Finale findet immer in Berlin statt.",
  "Auch Amateurteams koennen teilnehmen.",
  "1. FC Nuernberg gewann den ersten DFB-Pokal 1935.",
  "Der Pokal wiegt 5,2 Kilogramm.",
  "Der Pokal ist 53 cm hoch.",
  "Es spielen 64 Teams im Wettbewerb.",
  "Der Sieger qualifiziert sich fuer die Europa League.",
  "Schalke 04 gewann 5 Mal den Pokal.",
  "Borussia Dortmund holte den Pokal 5 Mal.",
  "Werder Bremen war 6 Mal Pokalsieger.",
  "Eintracht Frankfurt gewann 5 Mal.",
  "Der Pokal wurde im Krieg pausiert (1944-1952).",
  "1954 wurde er als DFB-Pokal neu gestartet.",
  "Das Olympiastadion Berlin fasst 74.475 Zuschauer.",
  "Hannover 96 gewann nur 1 Mal (1992).",
  "Holstein Kiel schlug Bayern 2021 im Elfmeterschiessen.",
  "Rot-Weiss Essen schlug Dortmund 1953.",
  "Das Finale ist immer an Himmelfahrt.",
  "Die ARD uebertraegt das Finale seit 1953.",
  "Bayer Leverkusen holte 2 Pokale.",
  "Der Pokal besteht aus Silber mit Vergoldung.",
  "Es gibt keine Verlaengerung in fruehen Runden.",
  "VfB Stuttgart gewann 3 Mal.",
  "Karlsruher SC holte 2 Pokale.",
  "1860 Muenchen gewann 2 Mal.",
  "Kickers Offenbach holte 2 Pokale.",
  "Fortuna Duesseldorf gewann 2 Mal.",
  "Borussia Moenchengladbach hat 3 Pokale.",
  "Kaiserslautern gewann 2 Mal.",
  "Hamburger SV holte 3 Pokale.",
  "Der Pokalsieger erhaelt 4 Mio Euro Praemie.",
  "Jede Runde bringt Praemiengeld.",
  "2020 fand das Finale ohne Zuschauer statt.",
  "RB Leipzig stand 2019 und 2021 im Finale.",
  "Union Berlin erreichte 2001 das Finale.",
  "Alemannia Aachen war 2004 im Finale.",
  "Energie Cottbus erreichte 1997 das Finale.",
  "Hertha BSC holte 2 Pokale.",
  "MSV Duisburg gewann 1966 und 1975.",
  "Der Pokal hat keine Gruppenphase.",
  "Es wird immer K.o. gespielt.",
  "Amateure haben Heimrecht gegen Profis.",
  "Uwe Seeler war Rekordtorschuetze im Pokal.",
  "Gerd Mueller schoss auch viele Pokaltore.",
  "Thomas Mueller erreichte viele Finals.",
  "Manuel Neuer holte 6 Pokale.",
  "Philipp Lahm gewann 6 Mal.",
  "Der Pokal ist der aelteste deutsche Wettbewerb."
};

// ========== CHAMPIONS LEAGUE TRIVIA ==========
static const char* CL_TRIVIA[] = {
  "Real Madrid gewann 15 Champions League Titel.",
  "Der Henkelpott wiegt 8 Kilogramm.",
  "Der Pokal ist 74 cm hoch.",
  "AC Mailand holte 7 Titel.",
  "Bayern Muenchen gewann 6 Mal.",
  "Liverpool hat 6 Titel gewonnen.",
  "Barcelona gewann 5 Mal.",
  "Die CL wurde 1955 gegruendet.",
  "Bis 1992 hiess sie Europapokal der Meister.",
  "Cristiano Ronaldo ist Rekordtorschuetze.",
  "Lionel Messi schoss ueber 120 CL-Tore.",
  "Das Finale 2005 war das Wunder von Istanbul.",
  "Liverpool holte 3:0 Rueckstand gegen Milan auf.",
  "Real Madrid gewann 5 Mal in Folge (1956-1960).",
  "Alfredo Di Stefano war der erste Star.",
  "Franz Beckenbauer holte 3 Titel mit Bayern.",
  "Borussia Dortmund gewann 1997.",
  "Juventus Turin hat 2 Titel.",
  "Inter Mailand gewann 3 Mal.",
  "Chelsea gewann 2012 und 2021.",
  "Manchester City holte 2023 erstmals den Titel.",
  "Das Finale 1999 war dramatisch fuer ManUnited.",
  "Ole Gunnar Solskjaer schoss das Siegtor 1999.",
  "Porto gewann 2004 unter Jose Mourinho.",
  "Ajax Amsterdam hat 4 Titel.",
  "Benfica Lissabon gewann 2 Mal.",
  "Nottingham Forest holte 1979 und 1980.",
  "Aston Villa gewann 1982.",
  "Hamburger SV holte 1983 den Titel.",
  "Steaua Bukarest gewann 1986.",
  "Roter Stern Belgrad holte 1991.",
  "Olympique Marseille gewann 1993.",
  "Die Hymne ist weltberuehmt.",
  "Handel komponierte die Original-Melodie.",
  "Der Pokal bleibt beim Verein nach 3 Siegen.",
  "Real Madrid durfte den Pokal behalten.",
  "Die Gruppenphase gibt es seit 1991.",
  "36 Teams spielen ab 2024 in einer Liga.",
  "Das Finale findet in wechselnden Staedten statt.",
  "Wembley ist beliebter Finalort.",
  "Das Finale 2023 war in Istanbul.",
  "Sergio Ramos koepfte wichtige Tore.",
  "Zinedine Zidane war Spieler und Trainer-Sieger.",
  "Carlo Ancelotti gewann als Trainer 4 Mal.",
  "Pep Guardiola holte 2 Titel mit Barcelona.",
  "Sir Alex Ferguson gewann 2 Titel.",
  "Arrigo Sacchi revolutionierte mit Milan.",
  "Johan Cruyff praegte Ajax und Barcelona.",
  "Der Henkelpott heisst so wegen der Henkel.",
  "Jeder Spieler erhaelt eine Miniatur-Tropaee."
};

// ========== WM TRIVIA ==========
static const char* WM_TRIVIA[] = {
  "Die erste WM fand 1930 in Uruguay statt.",
  "Brasilien gewann 5 Weltmeisterschaften.",
  "Deutschland holte 4 WM-Titel.",
  "Italien gewann ebenfalls 4 Mal.",
  "Argentinien hat 3 WM-Siege.",
  "Frankreich gewann 2 Mal (1998, 2018).",
  "Uruguay gewann die ersten zwei WMs.",
  "England holte 1966 den einzigen Titel.",
  "Spanien gewann 2010 in Suedafrika.",
  "Der WM-Pokal wiegt 6,175 Kilogramm.",
  "Der Pokal ist 36,8 cm hoch.",
  "Er besteht aus 18-karaettigem Gold.",
  "Bis 1970 gab es den Jules-Rimet-Pokal.",
  "Brasilien durfte ihn 1970 behalten.",
  "Der aktuelle Pokal existiert seit 1974.",
  "Miroslav Klose ist WM-Rekordtorschuetze (16).",
  "Ronaldo (Brasilien) schoss 15 WM-Tore.",
  "Gerd Mueller erzielte 14 WM-Tore.",
  "Just Fontaine schoss 13 Tore in einer WM.",
  "Pele gewann 3 Weltmeisterschaften.",
  "Diego Maradona fuehrte Argentinien 1986.",
  "Die Hand Gottes war 1986 gegen England.",
  "Zinedine Zidane kopfte 2 Tore im Finale 1998.",
  "Das Finale 2014 gewann Deutschland 1:0.",
  "Mario Goetze schoss das Siegtor 2014.",
  "Die WM 2022 fand in Katar statt.",
  "Lionel Messi holte 2022 endlich den WM-Titel.",
  "Das Finale 2022 endete 3:3 nach Verlaengerung.",
  "Kylian Mbappe schoss 3 Tore im Finale 2022.",
  "Die WM 2026 findet in USA/Kanada/Mexiko statt.",
  "48 Teams nehmen ab 2026 teil.",
  "Deutschland war 4 Mal Gastgeber.",
  "Das Wunder von Bern war 1954.",
  "Fritz Walter fuehrte Deutschland 1954.",
  "Das WM-Finale 1950 hatte 199.854 Zuschauer.",
  "Das Maracana in Rio war Rekord-Stadion.",
  "Lothar Matthaeus spielte 25 WM-Spiele.",
  "Cafu bestritt auch 20 WM-Spiele.",
  "Die WM 2002 fand in Japan/Korea statt.",
  "Oliver Kahn war 2002 bester Spieler.",
  "Ronaldo schoss 2 Tore im Finale 2002.",
  "Die WM 1966 hatte den WM-Hund Pickles.",
  "Er fand den gestohlenen Pokal wieder.",
  "Geoff Hurst erzielte den ersten Final-Hattrick.",
  "Das Wembley-Tor ist bis heute umstritten.",
  "Die WM 1970 war die erste in Farbe.",
  "Brasilien 1970 gilt als bestes Team aller Zeiten.",
  "Beckenbauer spielte 1970 mit Armschlinge.",
  "Die WM hat alle 4 Jahre stattgefunden.",
  "Nur 1942 und 1946 fiel sie wegen Krieg aus."
};

TournamentIntro::TournamentIntro() {
  weiterButton = new Button(60, 200, 200, 35, "WEITER");
}

void TournamentIntro::init() {
  tft.init();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(1);
}

const char* TournamentIntro::getDFBTrivia(int index) {
  if (index >= 0 && index < TRIVIA_COUNT) {
    return DFB_TRIVIA[index];
  }
  return DFB_TRIVIA[0];
}

const char* TournamentIntro::getCLTrivia(int index) {
  if (index >= 0 && index < TRIVIA_COUNT) {
    return CL_TRIVIA[index];
  }
  return CL_TRIVIA[0];
}

const char* TournamentIntro::getWMTrivia(int index) {
  if (index >= 0 && index < TRIVIA_COUNT) {
    return WM_TRIVIA[index];
  }
  return WM_TRIVIA[0];
}

void TournamentIntro::showIntro(TournamentType type, Touch* touch) {
  // Zufaelligen Trivia-Index waehlen
  randomSeed(millis());
  int triviaIndex = random(0, TRIVIA_COUNT);

  // Offizielle Farben je nach Turnier
  uint16_t bgColor, primaryColor, secondaryColor, accentColor, buttonColor;
  const char* title;
  const char* trivia;

  if (type == TOURNAMENT_DFB_POKAL) {
    // DFB-Pokal: Grün, Gold, Weiß
    bgColor = 0x0320;  // Dunkles Grün
    primaryColor = DFB_GREEN_LIGHT;
    secondaryColor = DFB_GOLD;
    accentColor = DFB_WHITE;
    buttonColor = DFB_GOLD;
    title = "DFB-POKAL";
    trivia = getDFBTrivia(triviaIndex);
  } else if (type == TOURNAMENT_CHAMPIONS_LEAGUE) {
    // Champions League: Blau, Weiß, Silber
    bgColor = 0x0008;  // Dunkles Blau
    primaryColor = CL_BLUE_LIGHT;
    secondaryColor = CL_WHITE;
    accentColor = CL_SILVER;
    buttonColor = CL_BLUE_LIGHT;
    title = "CHAMPIONS LEAGUE";
    trivia = getCLTrivia(triviaIndex);
  } else {
    // WM: Gold, Rot, Grün
    bgColor = 0x2000;  // Dunkles Weinrot
    primaryColor = WM_GOLD;
    secondaryColor = WM_GREEN;
    accentColor = TFT_WHITE;
    buttonColor = WM_GOLD;
    title = "WM 2026";
    trivia = getWMTrivia(triviaIndex);
  }

  // Bildschirm mit Hintergrundfarbe fuellen
  tft.fillScreen(bgColor);

  // Dekorative Linien oben
  tft.fillRect(0, 0, 320, 4, primaryColor);
  tft.fillRect(0, 4, 320, 2, secondaryColor);

  // Titel oben
  tft.setTextColor(secondaryColor, bgColor);
  tft.setTextSize(3);

  // Titel zentrieren
  int titleLen = strlen(title);
  int titleX = 160 - (titleLen * 9);  // Size 3 = ~18px pro Zeichen
  tft.setCursor(titleX, 15);
  tft.print(title);

  // Pokal zeichnen (zentriert)
  if (type == TOURNAMENT_DFB_POKAL) {
    drawDFBPokal(160, 48, 5);
  } else if (type == TOURNAMENT_CHAMPIONS_LEAGUE) {
    drawHenkelpott(160, 43, 4);
  } else {
    drawWorldCupTrophy(160, 43, 4);
  }

  // "Wusstest du?" Label
  tft.setTextColor(secondaryColor, bgColor);
  tft.setTextSize(1);
  tft.setCursor(10, 148);
  tft.print("WUSSTEST DU?");

  // Trivia-Text (mehrzeilig)
  tft.setTextColor(accentColor, bgColor);
  tft.setTextSize(1);

  // Text umbrechen (max ~50 Zeichen pro Zeile)
  String text = String(trivia);
  int y = 162;
  int maxChars = 50;

  while (text.length() > 0 && y < 195) {
    String line;
    if ((int)text.length() <= maxChars) {
      line = text;
      text = "";
    } else {
      // Letztes Leerzeichen finden
      int lastSpace = text.lastIndexOf(' ', maxChars);
      if (lastSpace > 0) {
        line = text.substring(0, lastSpace);
        text = text.substring(lastSpace + 1);
      } else {
        line = text.substring(0, maxChars);
        text = text.substring(maxChars);
      }
    }
    tft.setCursor(10, y);
    tft.print(line);
    y += 11;
  }

  // Weiter-Button zeichnen
  weiterButton->setColors(buttonColor, TFT_BLACK);
  weiterButton->draw(&tft);

  // Auf Button-Druck warten
  bool waiting = true;
  while (waiting) {
    if (touch->isTouched()) {
      int16_t x, y;
      touch->getCoordinates(x, y);
      if (weiterButton->contains(x, y)) {
        waiting = false;
      }
      // Kurze Entprellung
      delay(150);
    }
    delay(10);
  }
}

void TournamentIntro::drawDFBPokal(int centerX, int startY, int pixelSize) {
  uint16_t gold = DFB_GOLD;
  uint16_t goldDark = 0xC520;

  // Breiter Rand oben
  for (int x = -4; x <= 4; x++) {
    tft.fillRect(centerX + x * pixelSize, startY, pixelSize, pixelSize, gold);
  }

  // Schale (breiter werdend)
  for (int y = 1; y <= 3; y++) {
    int width = 4 + y;
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? goldDark : gold;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Schale nach innen (schmaler werdend)
  for (int y = 4; y <= 6; y++) {
    int width = 7 - (y - 4);
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? goldDark : gold;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Stiel
  for (int y = 7; y <= 9; y++) {
    for (int x = -1; x <= 1; x++) {
      uint16_t color = (x == -1 || x == 1) ? goldDark : gold;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Fuss (breiter)
  for (int y = 10; y <= 11; y++) {
    int width = (y == 10) ? 2 : 3;
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? goldDark : gold;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Glanz-Effekt
  tft.fillRect(centerX - 3 * pixelSize + 2, startY + 2 * pixelSize + 2, pixelSize - 2, pixelSize * 2, TFT_WHITE);
}

void TournamentIntro::drawHenkelpott(int centerX, int startY, int pixelSize) {
  uint16_t silver = CL_SILVER;
  uint16_t silverDark = 0x8410;

  // Henkel links und rechts (oben)
  for (int y = 0; y <= 3; y++) {
    tft.fillRect(centerX - 8 * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, silver);
    tft.fillRect(centerX + 8 * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, silver);
  }

  // Henkel-Bogen
  tft.fillRect(centerX - 8 * pixelSize, startY + 3 * pixelSize, pixelSize * 2, pixelSize, silver);
  tft.fillRect(centerX + 7 * pixelSize, startY + 3 * pixelSize, pixelSize * 2, pixelSize, silver);

  // Oberer Rand der Schale
  for (int x = -6; x <= 6; x++) {
    tft.fillRect(centerX + x * pixelSize, startY, pixelSize, pixelSize, silver);
  }

  // Schale (breiter werdend)
  for (int y = 1; y <= 4; y++) {
    int width = 6 + (y > 2 ? 1 : 0);
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? silverDark : silver;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Schale verjuengt sich
  for (int y = 5; y <= 8; y++) {
    int width = 7 - (y - 4);
    if (width < 2) width = 2;
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? silverDark : silver;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Stiel
  for (int y = 9; y <= 11; y++) {
    for (int x = -1; x <= 1; x++) {
      uint16_t color = (x == -1 || x == 1) ? silverDark : silver;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Fuss
  for (int y = 12; y <= 14; y++) {
    int width = (y == 12) ? 2 : (y == 13) ? 3 : 4;
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? silverDark : silver;
      tft.fillRect(centerX + x * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Glanz-Effekt
  tft.fillRect(centerX - 4 * pixelSize + 2, startY + 2 * pixelSize + 2, pixelSize - 1, pixelSize * 2, TFT_WHITE);
}

void TournamentIntro::drawWorldCupTrophy(int centerX, int startY, int pixelSize) {
  uint16_t gold = WM_GOLD;
  uint16_t goldDark = 0xC520;

  // Zwei Figuren, die den Globus halten (vereinfacht)
  for (int y = 0; y <= 2; y++) {
    tft.fillRect(centerX - 6 * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, gold);
    tft.fillRect(centerX - 5 * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, goldDark);
    tft.fillRect(centerX + 5 * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, goldDark);
    tft.fillRect(centerX + 6 * pixelSize, startY + y * pixelSize, pixelSize, pixelSize, gold);
  }

  // Globus (Kreis oben)
  int globusY = startY + 3 * pixelSize;
  for (int x = -4; x <= 4; x++) {
    tft.fillRect(centerX + x * pixelSize, globusY, pixelSize, pixelSize, gold);
  }
  for (int y = 1; y <= 3; y++) {
    int width = (y == 2) ? 5 : 4;
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? goldDark : gold;
      tft.fillRect(centerX + x * pixelSize, globusY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }
  for (int y = 4; y <= 5; y++) {
    int width = 5 - (y - 3);
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? goldDark : gold;
      tft.fillRect(centerX + x * pixelSize, globusY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Arme der Figuren zum Globus
  tft.fillRect(centerX - 5 * pixelSize, startY + 3 * pixelSize, pixelSize, pixelSize, gold);
  tft.fillRect(centerX + 5 * pixelSize, startY + 3 * pixelSize, pixelSize, pixelSize, gold);

  // Stiel
  int stielY = globusY + 6 * pixelSize;
  for (int y = 0; y <= 3; y++) {
    for (int x = -1; x <= 1; x++) {
      uint16_t color = (x == 0) ? gold : goldDark;
      tft.fillRect(centerX + x * pixelSize, stielY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Fuss (breiter werdend)
  int fussY = stielY + 4 * pixelSize;
  for (int y = 0; y <= 2; y++) {
    int width = 2 + y;
    for (int x = -width; x <= width; x++) {
      uint16_t color = (x == -width || x == width) ? goldDark : gold;
      tft.fillRect(centerX + x * pixelSize, fussY + y * pixelSize, pixelSize, pixelSize, color);
    }
  }

  // Glanz-Effekt auf dem Globus
  tft.fillRect(centerX - 2 * pixelSize + 2, globusY + pixelSize + 2, pixelSize - 1, pixelSize * 2, TFT_WHITE);
}
