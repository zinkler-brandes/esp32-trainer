#include "Button.h"

// Konstruktor
Button::Button(int16_t x, int16_t y, uint16_t w, uint16_t h, String label) {
  _x = x;
  _y = y;
  _width = w;
  _height = h;
  _label = label;

  // Standardfarben und Textgroesse
  _color = TFT_BLUE;        // Button-Hintergrund blau
  _textColor = TFT_WHITE;   // Text weiss
  _textSize = 2;            // Standard-Textgroesse
}

// Button auf Display zeichnen
void Button::draw(TFT_eSPI* tft) {
  // Gefuelltes Rechteck (Button-Hintergrund)
  tft->fillRoundRect(_x, _y, _width, _height, 8, _color);

  // Rahmen um den Button
  tft->drawRoundRect(_x, _y, _width, _height, 8, TFT_WHITE);

  // Text zentriert im Button
  tft->setTextColor(_textColor);
  tft->setTextSize(_textSize);
  tft->setTextDatum(MC_DATUM); // Middle Center
  tft->drawString(_label, _x + _width/2, _y + _height/2);
}

// Prüfen ob Touch innerhalb des Buttons liegt
bool Button::contains(int16_t touchX, int16_t touchY) {
  return (touchX >= _x && touchX <= (_x + _width) &&
          touchY >= _y && touchY <= (_y + _height));
}

// Label zurückgeben
String Button::getLabel() {
  return _label;
}

// Farben ändern
void Button::setColors(uint16_t btnColor, uint16_t txtColor) {
  _color = btnColor;
  _textColor = txtColor;
}

// Label aendern
void Button::setLabel(const String& label) {
  _label = label;
}

// Textgroesse aendern
void Button::setTextSize(uint8_t size) {
  _textSize = size;
}
