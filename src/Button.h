#ifndef SRC_BUTTON_H
#define SRC_BUTTON_H

#include <Arduino.h>
#include <TFT_eSPI.h>

class Button {
  public:
    // Konstruktor
    Button(int16_t x, int16_t y, uint16_t w, uint16_t h, String label);

    // Button auf Display zeichnen
    void draw(TFT_eSPI* tft);

    // Prüfen ob Touch-Koordinaten innerhalb des Buttons liegen
    bool contains(int16_t touchX, int16_t touchY);

    // Label zurückgeben (z.B. "7" für Ziffer 7)
    String getLabel();

    // Farben ändern (optional)
    void setColors(uint16_t btnColor, uint16_t txtColor);

    // Label ändern (für dynamische Buttons)
    void setLabel(const String& label);

    // Textgroesse aendern (Standard: 2)
    void setTextSize(uint8_t size);

  private:
    int16_t _x, _y;           // Position (obere linke Ecke)
    uint16_t _width, _height; // Groesse
    String _label;            // Text auf dem Button
    uint16_t _color;          // Button-Farbe
    uint16_t _textColor;      // Text-Farbe
    uint8_t _textSize;        // Text-Groesse (Standard: 2)
};

#endif
