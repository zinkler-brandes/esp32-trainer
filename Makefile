# Makefile für ESP32-2432S028 Development mit VIM

# PlatformIO aus venv nutzen
PIO = ./venv/bin/pio

.PHONY: build upload monitor clean test all

# Standard Target
all: build

# Kompilieren
build:
	@echo "Building project..."
	$(PIO) run

# Auf ESP32 flashen
upload:
	@echo "Uploading to ESP32..."
	$(PIO) run -t upload

# Serielle Konsole öffnen
monitor:
	@echo "Opening serial monitor (Ctrl+C to exit)..."
	$(PIO) device monitor

# Kompilieren und flashen
flash: build upload

# Kompilieren, flashen und Monitor starten
run: upload monitor

# Projekt aufräumen
clean:
	@echo "Cleaning build files..."
	$(PIO) run -t clean

# Dependencies aktualisieren
update:
	@echo "Updating libraries..."
	$(PIO) pkg update

# Projekt Info
info:
	@echo "ESP32-2432S028 Cheap Yellow Display Project"
	@echo "============================================"
	@$(PIO) project config

# Verfügbare Ports anzeigen
ports:
	@echo "Available serial ports:"
	@$(PIO) device list
