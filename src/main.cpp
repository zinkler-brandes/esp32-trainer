#include <Arduino.h>
#include "Menu.h"
#include "mathe.h"
#include "Touch.h"
#include "MatheMenu.h"
#include "DifficultyMenu.h"
#include "TimeMenu.h"
#include "TournamentMenu.h"
#include "Tournament.h"
#include "TrophyDisplay.h"
#include "SDManager.h"
#include "ProfileManager.h"
#include "ProfileSelectScreen.h"
#include "ProfileManageScreen.h"
#include "KeyboardScreen.h"
#include "ConfirmDialog.h"

// Modi
enum AppMode {
  // Profil-Modi
  MODE_PROFILE_SELECT,       // Profilauswahl (direkt spielen)
  MODE_PROFILE_CREATE,       // Tastatur für neues Profil
  MODE_PROFILE_MANAGE,       // Profilverwaltung
  MODE_CONFIRM_DELETE,       // Lösch-Bestätigung
  MODE_CONFIRM_RESET,        // Reset-Bestätigung
  // Spiel-Modi
  MODE_MENU,
  MODE_MATHE_SELECTION,
  MODE_DIFFICULTY_SELECTION,
  MODE_TIME_SELECTION,
  MODE_MATHE,
  // Turnier-Modi
  MODE_TOURNAMENT_SELECTION,
  MODE_TOURNAMENT_MATHE_SELECTION,
  MODE_TOURNAMENT_INTRO,
  MODE_TOURNAMENT,
  MODE_TOURNAMENT_PENALTY,
  MODE_TROPHY
};

// Globale Objekte
Menu menu;
MatheMenu matheMenu;
DifficultyMenu difficultyMenu;
TimeMenu timeMenu;
Mathe matheTrainer;
Touch touch;
TournamentMenu tournamentMenu;
TournamentMatheModeMenu tournamentMatheModeMenu;
Tournament tournament;
TrophyDisplay trophyDisplay;

// Profil-System
SDManager sdManager;
ProfileManager profileManager;
ProfileSelectScreen profileSelectScreen;
ProfileManageScreen profileManageScreen;
KeyboardScreen keyboardScreen;
ConfirmDialog confirmDialog;

// Aktuelles Profil
Profile currentProfile;

AppMode currentMode = MODE_PROFILE_SELECT;

// Gewählte Mathe-Optionen (für späteren Start)
MatheMode selectedMatheMode;
int selectedDifficulty;  // 1=Bayern, 2=Dortmund, 3=Hannover

// Turnier-Variablen
TournamentType selectedTournamentType;
int tournamentPlayerGoals;
int tournamentCpuGoals;

// Profil, das gelöscht werden soll
int profileToDelete = 0;

// Setter für Profilname in mathe.cpp
extern void setPlayerName(const String& name);

void showProfileSelect() {
  profileSelectScreen.setProfileManager(&profileManager);
  profileSelectScreen.draw();
  currentMode = MODE_PROFILE_SELECT;
}

void showKeyboard() {
  keyboardScreen.clear();
  keyboardScreen.draw();
  currentMode = MODE_PROFILE_CREATE;
}

void loginProfile(const Profile& profile) {
  currentProfile = profile;
  setPlayerName(currentProfile.name);
  menu.setPlayerName(currentProfile.name);
  menu.draw();
  currentMode = MODE_MENU;
  Serial.printf("Logged in as: %s (id=%d)\n", currentProfile.name.c_str(), currentProfile.id);
}

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("=== ESP32 Mathe-Trainer Starting ===");

  // Touch initialisieren
  touch.begin();

  // SD-Karte initialisieren
  bool sdReady = sdManager.begin();

  // Screens initialisieren
  menu.init();
  profileSelectScreen.init();
  profileManageScreen.init();
  keyboardScreen.init();
  confirmDialog.init();

  if (sdReady) {
    // ProfileManager initialisieren
    profileManager.begin(&sdManager);

    int profileCount = profileManager.getProfileCount();
    Serial.printf("Found %d profiles\n", profileCount);

    if (profileCount == 0) {
      // Kein Profil -> Tastatur zeigen für erstes Profil
      Serial.println("No profiles - showing keyboard");
      showKeyboard();
    } else if (profileCount == 1) {
      // Genau 1 Profil -> Auto-Login
      Serial.println("Single profile - auto-login");
      Profile p = profileManager.getFirstProfile();
      loginProfile(p);
    } else {
      // Mehrere Profile -> Auswahl zeigen
      Serial.println("Multiple profiles - showing selection");
      showProfileSelect();
    }
  } else {
    // SD-Fehler -> Trotzdem starten mit "Gast"
    Serial.println("SD error - starting as Guest");
    currentProfile = Profile(0, "GAST");
    setPlayerName("GAST");
    menu.setPlayerName("GAST");
    menu.draw();
    currentMode = MODE_MENU;
  }

  Serial.println("System ready");
}

void loop() {
  static unsigned long lastTouch = 0;
  static unsigned long lastTimerUpdate = 0;

  // Timer für Mathe-Trainer prüfen (alle 500ms)
  if ((currentMode == MODE_MATHE || currentMode == MODE_TOURNAMENT) && millis() - lastTimerUpdate > 500) {
    lastTimerUpdate = millis();
    matheTrainer.update();
  }

  if (touch.isTouched()) {
    // Entprellen: Nur alle 200ms verarbeiten
    if (millis() - lastTouch > 200) {
      lastTouch = millis();

      int16_t x, y;
      touch.getCoordinates(x, y);

      // Nur verarbeiten wenn gültige Koordinaten
      if (x >= 0 && y >= 0) {
        Serial.printf("Touch at: (%d, %d)\n", x, y);

        // Touch je nach Modus verarbeiten
        switch (currentMode) {

          // ========== PROFIL-MODI ==========

          case MODE_PROFILE_SELECT:
            {
              int choice = profileSelectScreen.handleTouch(x, y);
              if (choice > 0) {
                // Profil ausgewählt -> Direkt einloggen!
                Profile p = profileManager.getProfileById(choice);
                loginProfile(p);
              } else if (choice == -1) {
                // Neues Profil erstellen
                showKeyboard();
              } else if (choice == -2) {
                // Verwalten
                profileManageScreen.setProfileManager(&profileManager);
                profileManageScreen.draw();
                currentMode = MODE_PROFILE_MANAGE;
              }
            }
            break;

          case MODE_PROFILE_CREATE:
            {
              int result = keyboardScreen.handleTouch(x, y);
              if (result == 1) {
                // OK gedrückt - Profil erstellen
                String name = keyboardScreen.getName();
                if (profileManager.createProfile(name)) {
                  // Direkt einloggen
                  Profile p = profileManager.getProfileById(profileManager.getProfileCount());
                  // Hole das zuletzt erstellte Profil
                  p = profileManager.getProfile(profileManager.getProfileCount() - 1);
                  loginProfile(p);
                } else {
                  // Fehler - nochmal versuchen
                  showKeyboard();
                }
              } else if (result == 2) {
                // Abbrechen
                if (profileManager.getProfileCount() > 0) {
                  showProfileSelect();
                }
                // Bei 0 Profilen bleibt Tastatur offen
              }
            }
            break;

          case MODE_PROFILE_MANAGE:
            {
              int choice = profileManageScreen.handleTouch(x, y);
              if (choice > 0) {
                // Profil zum Löschen ausgewählt
                profileToDelete = choice;
                Profile p = profileManager.getProfileById(choice);
                confirmDialog.show("LOESCHEN", p.name + " loeschen?");
                currentMode = MODE_CONFIRM_DELETE;
              } else if (choice == -1) {
                // Reset alle Profile
                confirmDialog.show("RESET", "Alle Profile loeschen?");
                currentMode = MODE_CONFIRM_RESET;
              } else if (choice == -2) {
                // Zurück
                showProfileSelect();
              }
            }
            break;

          case MODE_CONFIRM_DELETE:
            {
              int choice = confirmDialog.handleTouch(x, y);
              if (choice == 1) {
                // Ja - Profil löschen
                profileManager.deleteProfile(profileToDelete);
                profileToDelete = 0;

                // Je nach verbleibenden Profilen
                int count = profileManager.getProfileCount();
                if (count == 0) {
                  showKeyboard();
                } else {
                  // Zurück zur Verwaltung
                  profileManageScreen.setProfileManager(&profileManager);
                  profileManageScreen.draw();
                  currentMode = MODE_PROFILE_MANAGE;
                }
              } else if (choice == 2) {
                // Nein - zurück zur Verwaltung
                profileToDelete = 0;
                profileManageScreen.setProfileManager(&profileManager);
                profileManageScreen.draw();
                currentMode = MODE_PROFILE_MANAGE;
              }
            }
            break;

          case MODE_CONFIRM_RESET:
            {
              int choice = confirmDialog.handleTouch(x, y);
              if (choice == 1) {
                // Ja - Alles löschen
                profileManager.resetAll();
                showKeyboard();
              } else if (choice == 2) {
                // Nein - zurück zur Auswahl
                showProfileSelect();
              }
            }
            break;

          // ========== HAUPTMENÜ ==========

          case MODE_MENU:
            {
              int choice = menu.handleTouch(x, y);
              if (choice == 0) {
                // Zurück zur Profilauswahl
                Serial.println("Returning to profile selection");
                showProfileSelect();
              } else if (choice == 1) {
                // Mathe-Fach öffnen
                Serial.println("Opening Mathe Menu");
                matheMenu.init();
                matheMenu.draw();
                currentMode = MODE_MATHE_SELECTION;
              } else if (choice == 2) {
                // Englisch (noch nicht implementiert)
                Serial.println("Englisch - coming soon");
                // TODO: Englisch-Menü
              }
            }
            break;

          case MODE_MATHE_SELECTION:
            {
              int matheChoice = matheMenu.handleTouch(x, y);
              if (matheChoice == 0) {
                // Zurück zum Hauptmenü
                Serial.println("Returning to menu from Mathe Selection");
                menu.draw();
                currentMode = MODE_MENU;
              } else if (matheChoice == 1) {
                // +/- Modus gewählt -> weiter zu Schwierigkeit
                Serial.println("Selected: +/-");
                selectedMatheMode = MATHE_PLUS_MINUS;
                difficultyMenu.init();
                difficultyMenu.draw();
                currentMode = MODE_DIFFICULTY_SELECTION;
              } else if (matheChoice == 2) {
                // 1x1 Modus gewählt -> weiter zu Schwierigkeit
                Serial.println("Selected: 1x1");
                selectedMatheMode = MATHE_MULTIPLY;
                difficultyMenu.init();
                difficultyMenu.draw();
                currentMode = MODE_DIFFICULTY_SELECTION;
              } else if (matheChoice == 3) {
                // Pokal-Modus
                Serial.println("Opening Tournament Menu");
                tournamentMenu.init();
                tournamentMenu.draw();
                currentMode = MODE_TOURNAMENT_SELECTION;
              }
            }
            break;

          case MODE_DIFFICULTY_SELECTION:
            {
              int diffChoice = difficultyMenu.handleTouch(x, y);
              if (diffChoice == 0) {
                // Zurück zur Modus-Auswahl
                Serial.println("Returning to Mathe Selection");
                matheMenu.draw();
                currentMode = MODE_MATHE_SELECTION;
              } else if (diffChoice >= 1 && diffChoice <= 3) {
                // Schwierigkeit gewählt -> weiter zu Zeit
                Serial.printf("Selected difficulty: %d\n", diffChoice);
                selectedDifficulty = diffChoice;
                timeMenu.init();
                timeMenu.draw();
                currentMode = MODE_TIME_SELECTION;
              }
            }
            break;

          case MODE_TIME_SELECTION:
            {
              int timeChoice = timeMenu.handleTouch(x, y);
              if (timeChoice == 0) {
                // Zurück zur Schwierigkeits-Auswahl
                Serial.println("Returning to Difficulty Selection");
                difficultyMenu.draw();
                currentMode = MODE_DIFFICULTY_SELECTION;
              } else if (timeChoice >= 1 && timeChoice <= 3) {
                // Zeit gewählt -> Spiel starten!
                unsigned long duration;
                switch (timeChoice) {
                  case 1: duration = 60000; break;   // 1 Minute
                  case 2: duration = 180000; break;  // 3 Minuten
                  case 3: duration = 300000; break;  // 5 Minuten
                  default: duration = 180000; break;
                }

                Difficulty diff = DifficultyMenu::getDifficulty(selectedDifficulty);
                Serial.printf("Starting game: mode=%d, time=%lu, answerTime=%lu, steps=%d\n",
                              selectedMatheMode, duration, diff.answerTimeMs, diff.stepsForGoal);

                matheTrainer.init(selectedMatheMode, duration, diff.answerTimeMs, diff.stepsForGoal);
                currentMode = MODE_MATHE;
              }
            }
            break;

          case MODE_MATHE:
            {
              bool returnToMenu = matheTrainer.handleButtonPress(x, y);
              if (returnToMenu) {
                Serial.println("Returning to menu");
                menu.draw();
                currentMode = MODE_MENU;
              }
            }
            break;

          // ========== TURNIER-MODI ==========

          case MODE_TOURNAMENT_SELECTION:
            {
              int choice = tournamentMenu.handleTouch(x, y);
              if (choice == 0) {
                // Zurück zum Hauptmenü
                menu.draw();
                currentMode = MODE_MENU;
              } else if (choice == 1) {
                // DFB-Pokal -> Mathe-Modus wählen
                selectedTournamentType = TOURNAMENT_DFB_POKAL;
                tournamentMatheModeMenu.init();
                tournamentMatheModeMenu.draw();
                currentMode = MODE_TOURNAMENT_MATHE_SELECTION;
              } else if (choice == 2) {
                // Champions League -> direkt mit MIXED starten
                selectedTournamentType = TOURNAMENT_CHAMPIONS_LEAGUE;
                selectedMatheMode = MATHE_MIXED;
                tournament.init(selectedTournamentType, selectedMatheMode);
                tournament.draw();
                currentMode = MODE_TOURNAMENT_INTRO;
              }
            }
            break;

          case MODE_TOURNAMENT_MATHE_SELECTION:
            {
              int choice = tournamentMatheModeMenu.handleTouch(x, y);
              if (choice == 0) {
                // Zurück zur Turnier-Auswahl
                tournamentMenu.draw();
                currentMode = MODE_TOURNAMENT_SELECTION;
              } else if (choice == 1) {
                // +/- gewählt
                selectedMatheMode = MATHE_PLUS_MINUS;
                tournament.init(selectedTournamentType, selectedMatheMode);
                tournament.draw();
                currentMode = MODE_TOURNAMENT_INTRO;
              } else if (choice == 2) {
                // 1x1 gewählt
                selectedMatheMode = MATHE_MULTIPLY;
                tournament.init(selectedTournamentType, selectedMatheMode);
                tournament.draw();
                currentMode = MODE_TOURNAMENT_INTRO;
              }
            }
            break;

          case MODE_TOURNAMENT_INTRO:
            {
              int choice = tournament.handleTouch(x, y);
              if (choice == 0) {
                // Zurück zum Menü
                menu.draw();
                currentMode = MODE_MENU;
              } else if (choice == 1) {
                // Anpfiff! - Spiel starten
                // 2 Minuten regulär + 10-35 Sekunden Nachspielzeit
                int nachspielzeitSek = random(10, 36);  // 10-35 Sek
                Serial.printf("Spielzeit: 2:00 + %d Sek Nachspielzeit\n", nachspielzeitSek);

                matheTrainer.init(
                  tournament.getMatheMode(),
                  120000,  // 2 Minuten reguläre Spielzeit
                  tournament.getAnswerTime(),
                  tournament.getStepsForGoal(),
                  nachspielzeitSek
                );
                currentMode = MODE_TOURNAMENT;
              }
            }
            break;

          case MODE_TOURNAMENT:
            {
              // Touch verarbeiten (Numpad etc.)
              matheTrainer.handleButtonPress(x, y);

              // Prüfen ob Spiel vorbei (Timer abgelaufen)
              if (matheTrainer.isGameOver()) {
                // Ergebnis aus matheTrainer holen
                int playerGoals = matheTrainer.getGoalsScored();
                int cpuGoals = matheTrainer.getGoalsAgainst();

                Serial.printf("Match ended: %d : %d\n", playerGoals, cpuGoals);

                // Ergebnis an Tournament übergeben
                tournament.handleMatchResult(playerGoals, cpuGoals);

                // Je nach Ergebnis weiterleiten
                if (tournament.getState() == TOURNAMENT_CHAMPION) {
                  trophyDisplay.init();
                  trophyDisplay.showTrophy(selectedTournamentType);
                  currentMode = MODE_TROPHY;
                } else if (tournament.getState() == TOURNAMENT_PENALTY) {
                  // Elfmeterschießen starten
                  tournament.draw();
                  delay(2000);  // Kurz "ELFMETERSCHIESSEN" zeigen
                  matheTrainer.initPenalty(
                    tournament.getMatheMode(),
                    tournament.getAnswerTime()
                  );
                  currentMode = MODE_TOURNAMENT_PENALTY;
                } else {
                  // ROUND_WON oder ROUND_LOST
                  tournament.draw();
                  currentMode = MODE_TOURNAMENT_INTRO;
                }
              }
            }
            break;

          case MODE_TOURNAMENT_PENALTY:
            {
              // Elfmeterschießen - Numpad für Aufgabe
              matheTrainer.handleButtonPress(x, y);

              // Prüfen ob Antwort gegeben wurde
              if (matheTrainer.isPenaltyAnswered()) {
                bool playerScored = matheTrainer.wasPenaltyCorrect();

                // CPU schießt
                bool cpuScored = (random(0, 100) < tournament.getCpuPenaltyProb());
                int currentRound = tournament.getPenaltyRound();

                // Ergebnis verarbeiten
                bool shootoutDone = tournament.handlePenalty(playerScored, cpuScored);

                // Animation zeigen
                matheTrainer.showPenaltyResult(
                  playerScored,
                  cpuScored,
                  tournament.getPlayerPenalties(),
                  tournament.getCpuPenalties(),
                  currentRound
                );

                if (shootoutDone) {
                  // Elfmeterschießen beendet
                  if (tournament.getState() == TOURNAMENT_CHAMPION) {
                    trophyDisplay.init();
                    trophyDisplay.showTrophy(selectedTournamentType);
                    currentMode = MODE_TROPHY;
                  } else {
                    tournament.draw();
                    currentMode = MODE_TOURNAMENT_INTRO;
                  }
                } else {
                  // Nächster Elfmeter - neues Penalty-Spiel starten
                  matheTrainer.initPenalty(
                    tournament.getMatheMode(),
                    tournament.getAnswerTime()
                  );
                }
              }
            }
            break;

          case MODE_TROPHY:
            {
              int choice = trophyDisplay.handleTouch(x, y);
              if (choice == 0) {
                menu.draw();
                currentMode = MODE_MENU;
              }
            }
            break;
        }
      }
    }
  }
}
