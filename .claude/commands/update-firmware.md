# Firmware für Web-Flasher aktualisieren

Dieser Skill lädt die neueste Firmware von GitHub Releases und aktualisiert die Web-Flasher-Dateien.

## Ablauf

1. **Neueste Version ermitteln**
   ```bash
   gh release view --json tagName,assets -q '.tagName'
   ```

2. **Prüfen ob Binary verfügbar**
   - Warte ggf. bis GitHub Action fertig ist
   - Asset muss `tikitaka-vX.X.X.bin` heißen

3. **Binary herunterladen**
   ```bash
   gh release download vX.X.X -p "tikitaka-*.bin" -D docs/ --clobber
   ```

4. **manifest.json aktualisieren**
   - `version` auf neue Version setzen
   - `builds[0].parts[0].path` auf `tikitaka-vX.X.X.bin` setzen

5. **flasher.html aktualisieren**
   - Versionsnummer im Text aktualisieren

6. **Commit erstellen**
   ```bash
   git add docs/tikitaka-*.bin docs/manifest.json docs/flasher.html
   git commit -m "Update firmware to vX.X.X"
   git push origin main
   ```

7. **Bestätigung**
   - Zeige: "Firmware vX.X.X für Web-Flasher aktualisiert!"
   - Hinweis: "Änderungen gepusht. GitHub Pages wird in ~1 Min aktualisiert."

## Parameter

- Ohne Parameter: Nimmt die neueste Version
- Mit Version: `/update-firmware v0.2.0` - Nimmt die angegebene Version

## Beispiel

```
Nutzer: /update-firmware

Claude: Lade Firmware v0.1.1 herunter...
        ✓ tikitaka-v0.1.1.bin (1.2 MB)
        ✓ manifest.json aktualisiert
        ✓ flasher.html aktualisiert
        ✓ Änderungen committed und gepusht

        Firmware v0.1.1 für Web-Flasher aktualisiert!
```

## Hinweise

- Alte Binary-Dateien werden überschrieben
- GitHub Pages braucht ~1 Minute zum Aktualisieren
- Falls Action noch läuft: Warten und erneut versuchen
