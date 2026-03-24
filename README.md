# ESP32-2432S028 Trainer

Lernprojekt für das ESP32-2432S028 "Cheap Yellow Display" Board.

## Hardware
- ESP32 (ESP32-WROOM)
- 2.8" TFT Display (240x320, ILI9341)
- Resistiver Touchscreen (XPT2046)
- SD Card Reader
- RGB LED (Pins 4, 16, 17)
- LDR Lichtsensor (Pin 34)

## Setup

### Erstmalige Einrichtung

**Option 1: Automatisches Setup (empfohlen)**
```bash
./setup.sh
```

**Option 2: Manuelle Einrichtung**
```bash
# Python-Version setzen (mit pyenv)
pyenv local 3.10.13

# Virtual Environment erstellen
python -m venv venv

# venv aktivieren
source venv/bin/activate

# PlatformIO installieren
pip install platformio
```

### Täglicher Workflow

Das Makefile nutzt automatisch das venv, du musst es NICHT aktivieren!

```bash
# Projekt kompilieren
make build

# Auf ESP32 flashen
make upload

# Serielle Konsole
make monitor

# Alles auf einmal (kompilieren, flashen, monitor)
make run
```

## VIM Workflow
1. Code editieren: `vim src/main.cpp`
2. Kompilieren und testen: `make run`
3. Fehler beheben, wiederholen

## Pinout
```
Display (ILI9341):
- MISO: GPIO 12
- MOSI: GPIO 13
- SCLK: GPIO 14
- CS:   GPIO 15
- DC:   GPIO 2
- BL:   GPIO 21

Touch (XPT2046):
- CS:   GPIO 33

RGB LED:
- Red:   GPIO 4  (LOW = an)
- Green: GPIO 16 (LOW = an)
- Blue:  GPIO 17 (LOW = an)

LDR: GPIO 34 (analog)
```