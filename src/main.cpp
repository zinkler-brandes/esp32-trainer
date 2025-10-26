#include <Arduino.h>
#include "mathe.h"
#include "Touch.h"

Mathe matheTrainer;
Touch touch;

// Pins für RGB LED und LDR
#define RGB_LED_R 4
#define RGB_LED_G 16
#define RGB_LED_B 17
#define LDR_PIN 34

void setup() {
  Serial.begin(115200);
  delay(100);

  matheTrainer.init();
  touch.begin();
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
        matheTrainer.handleButtonPress(x, y);
      }
    }
  }
}
