#ifndef SRC_TOUCH_H
#define SRC_TOUCH_H

#include <Arduino.h>
#include <XPT2046_Bitbang.h>

class Touch {
  public:
    Touch();
    void begin();
    bool isTouched();
    void getCoordinates(int16_t &x, int16_t &y);

  private:
    XPT2046_Bitbang* ts;

    // Touch SPI Pins für ESP32-2432S028 (separater Bus!)
    static const uint8_t CLK_PIN = 25;
    static const uint8_t MOSI_PIN = 32;
    static const uint8_t MISO_PIN = 39;
    static const uint8_t CS_PIN = 33;
};

#endif
