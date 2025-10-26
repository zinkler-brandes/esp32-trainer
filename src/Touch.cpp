#include "Touch.h"

Touch::Touch() {
  // Touch-Controller mit separaten SPI-Pins initialisieren
  // Parameter: MOSI, MISO, CLK, CS, width, height
  ts = new XPT2046_Bitbang(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN, 320, 240);
}

void Touch::begin() {
  ts->begin();

  // Kalibrierung setzen (Werte vom Blog-Post)
  ts->setCalibration(208, 3823, 282, 3835);

  Serial.println("Touch initialized with separate SPI pins");
  Serial.printf("  CLK=%d, MOSI=%d, MISO=%d, CS=%d\n", CLK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);
  Serial.println("Calibration set: x(208-3823), y(282-3835)");
}

bool Touch::isTouched() {
  TouchPoint tp = ts->getTouch();
  // Touch ist gültig wenn zRaw > 0
  return (tp.zRaw > 0);
}

void Touch::getCoordinates(int16_t &x, int16_t &y) {
  TouchPoint tp = ts->getTouch();

  // Kein Touch erkannt
  if (tp.zRaw == 0) {
    x = -1;
    y = -1;
    return;
  }

  // Die Library liefert bereits kalibrierte Werte
  x = tp.x;
  y = tp.y;

  // Koordinaten begrenzen
  x = constrain(x, 0, 320);
  y = constrain(y, 0, 240);
}
