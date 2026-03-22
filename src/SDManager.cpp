#include "SDManager.h"

SDManager::SDManager() : _ready(false), _spi(nullptr) {
}

bool SDManager::begin() {
  Serial.println("Initializing SD card...");

  // SPI für SD-Karte initialisieren (separater Bus vom Display)
  _spi = new SPIClass(HSPI);
  _spi->begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  // SD-Karte initialisieren
  if (!SD.begin(SD_CS, *_spi)) {
    Serial.println("SD card initialization failed!");
    _ready = false;
    return false;
  }

  // Kartentyp prüfen
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached!");
    _ready = false;
    return false;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  // Kartengröße anzeigen
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  // Profil-Verzeichnis erstellen falls nicht vorhanden
  if (!SD.exists("/profiles")) {
    Serial.println("Creating /profiles directory...");
    if (!SD.mkdir("/profiles")) {
      Serial.println("Failed to create /profiles directory!");
      // Trotzdem weitermachen - vielleicht ist es ein FAT16 Problem
    }
  }

  _ready = true;
  Serial.println("SD card initialized successfully!");
  return true;
}

bool SDManager::isReady() {
  return _ready;
}

bool SDManager::formatCard() {
  if (!_ready) return false;

  Serial.println("Formatting profiles directory...");

  // Alle Dateien im /profiles Verzeichnis löschen
  File root = SD.open("/profiles");
  if (root) {
    File file = root.openNextFile();
    while (file) {
      String fileName = String("/profiles/") + file.name();
      file.close();
      SD.remove(fileName.c_str());
      Serial.printf("Deleted: %s\n", fileName.c_str());
      file = root.openNextFile();
    }
    root.close();
  }

  return true;
}

bool SDManager::fileExists(const char* path) {
  if (!_ready) return false;
  return SD.exists(path);
}

bool SDManager::createDir(const char* path) {
  if (!_ready) return false;
  if (SD.exists(path)) return true;
  return SD.mkdir(path);
}

bool SDManager::writeFile(const char* path, const char* content) {
  if (!_ready) return false;

  File file = SD.open(path, FILE_WRITE);
  if (!file) {
    Serial.printf("Failed to open file for writing: %s\n", path);
    return false;
  }

  size_t written = file.print(content);
  file.close();

  if (written == 0) {
    Serial.printf("Failed to write to file: %s\n", path);
    return false;
  }

  Serial.printf("Written %d bytes to: %s\n", written, path);
  return true;
}

String SDManager::readFile(const char* path) {
  if (!_ready) return "";

  File file = SD.open(path);
  if (!file) {
    Serial.printf("Failed to open file for reading: %s\n", path);
    return "";
  }

  String content = "";
  while (file.available()) {
    content += (char)file.read();
  }
  file.close();

  return content;
}

bool SDManager::deleteFile(const char* path) {
  if (!_ready) return false;

  if (!SD.exists(path)) {
    return true;  // Datei existiert nicht, also "erfolgreich gelöscht"
  }

  return SD.remove(path);
}
