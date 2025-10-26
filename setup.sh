#!/bin/bash
# Setup-Script für ESP32-2432S028 Entwicklung

echo "ESP32-2432S028 Setup"
echo "===================="

# Prüfen ob pyenv installiert ist
if ! command -v pyenv &> /dev/null; then
    echo "Error: pyenv ist nicht installiert!"
    exit 1
fi

# Python-Version setzen
echo "Setting Python version to 3.10.13..."
pyenv local 3.10.13

# Venv erstellen falls nicht vorhanden
if [ ! -d "venv" ]; then
    echo "Creating virtual environment..."
    python -m venv venv
fi

# Venv aktivieren und PlatformIO installieren
echo "Installing PlatformIO..."
source venv/bin/activate
pip install --upgrade pip
pip install platformio

echo ""
echo "Setup complete!"
echo ""
echo "Next steps:"
echo "1. Activate venv:  source venv/bin/activate"
echo "2. Build project:  make build"
echo "3. Upload to ESP:  make upload"
echo "4. Run all:        make run"
echo ""
echo "Or use Makefile directly (venv is automatic):"
echo "  make build"
echo "  make run"
