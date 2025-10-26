#include <Arduino.h>
#include "Menu.h"
#include "mathe.h"
#include "Touch.h"

// Modi
enum AppMode {
  MODE_MENU,
  MODE_MATHE,
  MODE_FUSSBALL_RESULTS,
  MODE_FUSSBALL_TABLES
};

// Globale Objekte
Menu menu;
Mathe matheTrainer;
Touch touch;
AppMode currentMode = MODE_MENU;

void setup() {
  Serial.begin(115200);
  delay(100);

  // Touch initialisieren
  touch.begin();

  // Menü initialisieren und anzeigen
  menu.init();
  menu.draw();

  Serial.println("System started - showing menu");
}

void loop() {
  static unsigned long lastTouch = 0;

  if (touch.isTouched()) {
    // Entprellen: Nur alle 200ms verarbeiten
    if (millis() - lastTouch > 200) {
      lastTouch = millis();

      int16_t x, y;
      touch.getCoordinates(x, y);

      // Nur verarbeiten wenn gültige Koordinaten
      if (x >= 0 && y >= 0) {
        Serial.printf("Touch at: (%d, %d)\n", x, y);

        // Touch je nach Modus verarbeiten
        switch (currentMode) {
          case MODE_MENU:
            {
              int choice = menu.handleTouch(x, y);
              if (choice == 1) {
                // Mathe-Trainer starten
                Serial.println("Starting Mathe-Trainer");
                matheTrainer.init();
                currentMode = MODE_MATHE;
              } else if (choice == 2) {
                Serial.println("Fussball-Ergebnisse (noch nicht implementiert)");
                // TODO: Fussball-Ergebnisse Modus
              } else if (choice == 3) {
                Serial.println("Fussball-Tabellen (noch nicht implementiert)");
                // TODO: Fussball-Tabellen Modus
              }
            }
            break;

          case MODE_MATHE:
            {
              bool returnToMenu = matheTrainer.handleButtonPress(x, y);
              if (returnToMenu) {
                Serial.println("Returning to menu");
                menu.draw();
                currentMode = MODE_MENU;
              }
            }
            break;

          case MODE_FUSSBALL_RESULTS:
            // TODO: Implementieren
            break;

          case MODE_FUSSBALL_TABLES:
            // TODO: Implementieren
            break;
        }
      }
    }
  }
}
