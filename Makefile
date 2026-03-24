# Makefile für ESP32-2432S028 Development mit VIM

# PlatformIO aus venv nutzen
PIO = ./venv/bin/pio

.PHONY: build upload monitor clean test all erase reset merged

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

# ESP32 komplett löschen (für Recovery)
erase:
	@echo "Erasing ESP32 flash completely..."
	$(PIO) run -t erase

# Kompletter Reset: Löschen + Neu flashen
reset: erase upload
	@echo "Reset complete!"

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

# Merged Binary erstellen (für Web-Flasher)
merged: build
	@echo "Creating merged binary..."
	@python3 -m esptool --chip esp32 merge_bin \
		--output tikitaka-merged.bin \
		--flash_mode dio \
		--flash_freq 40m \
		--flash_size 4MB \
		0x1000 .pio/build/esp32-2432S028/bootloader.bin \
		0x8000 .pio/build/esp32-2432S028/partitions.bin \
		0xe000 ~/.platformio/packages/framework-arduinoespressif32/tools/partitions/boot_app0.bin \
		0x10000 .pio/build/esp32-2432S028/firmware.bin
	@echo "Created: tikitaka-merged.bin"
