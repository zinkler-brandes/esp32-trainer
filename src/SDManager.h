#ifndef SRC_SDMANAGER_H
#define SRC_SDMANAGER_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

// SD-Karten-Pins für ESP32-2432S028
#define SD_CS    5
#define SD_SCK  18
#define SD_MISO 19
#define SD_MOSI 23

class SDManager {
  public:
    SDManager();

    bool begin();           // SD initialisieren
    bool isReady();         // SD verfügbar?
    bool formatCard();      // Alles löschen (Reset)

    // Datei-Operationen
    bool fileExists(const char* path);
    bool createDir(const char* path);
    bool writeFile(const char* path, const char* content);
    String readFile(const char* path);
    bool deleteFile(const char* path);

  private:
    bool _ready;
    SPIClass* _spi;
};

#endif
