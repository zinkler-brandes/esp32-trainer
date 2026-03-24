# Release veröffentlichen

Dieser Skill erstellt ein neues Kopfball-Release.

## Ablauf

1. **Version erfragen**
   - Frage den Nutzer nach der neuen Versionsnummer (z.B. `1.0.0` oder `v1.0.0`)
   - Akzeptiere beide Formate, normalisiere intern zu `v1.0.0`
   - Validiere: muss semver-Format sein (MAJOR.MINOR.PATCH)

2. **Prüfungen durchführen**
   - Prüfe mit `git tag -l` ob das Tag bereits existiert
   - Prüfe mit `git status --porcelain` ob uncommittete Änderungen existieren
   - Falls ja: Abbrechen und Nutzer informieren

3. **Release Notes erfragen**
   - Frage den Nutzer: "Was ist neu in dieser Version?"
   - Zeige die letzten 5 Commits als Kontext: `git log --oneline -5`
   - Nutzer kann kurze Stichpunkte angeben

4. **manifest.json aktualisieren**
   - Falls `docs/manifest.json` existiert:
     - Aktualisiere `version` auf neue Version
     - Aktualisiere den Download-Pfad in `builds[0].parts[0].path` auf:
       `https://github.com/zinkler-brandes/esp32-trainer/releases/download/vX.X.X/tikitaka-vX.X.X.bin`
   - Falls nicht vorhanden: Überspringe diesen Schritt

5. **Git-Operationen**
   ```bash
   # Falls manifest.json geändert wurde:
   git add docs/manifest.json
   git commit -m "Release vX.X.X"

   # Tag erstellen mit Release Notes als Nachricht
   git tag -a vX.X.X -m "Release vX.X.X

   [Release Notes hier]"

   # Push (Branch + Tag)
   git push origin main
   git push origin vX.X.X
   ```

6. **Bestätigung**
   - Zeige: "Release vX.X.X erstellt!"
   - Zeige Link: `https://github.com/zinkler-brandes/esp32-trainer/releases/tag/vX.X.X`
   - Hinweis: "GitHub Action baut jetzt die Firmware. In ~2 Min verfügbar."

## Beispiel-Interaktion

```
Nutzer: /release

Claude: Welche Version soll released werden? (aktuell: v0.9.0)

Nutzer: 1.0.0

Claude: Was ist neu in v1.0.0?

        Letzte Commits:
        - abc1234 WM-Modus fertig
        - def5678 Flaggen-Quiz hinzugefügt
        - ghi9012 Bugfix Touch-Kalibrierung

Nutzer: - WM 2026 mit allen 48 Teams
        - Flaggen-Quiz
        - Verbessertes Touch

Claude: Release v1.0.0 erstellt!
        → https://github.com/zinkler-brandes/esp32-trainer/releases/tag/v1.0.0

        GitHub Action baut jetzt die Firmware (~2 Min).
```

## Hinweise

- Der GitHub-Username im Pfad muss ggf. angepasst werden
- Die GitHub Action `.github/workflows/release.yml` muss existieren
- Ohne Action wird nur das Tag erstellt, aber keine .bin Datei
