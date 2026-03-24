#include <Arduino.h>
#include "Menu.h"
#include "mathe.h"
#include "Touch.h"
#include "MatheMenu.h"
#include "EinzelspielMenu.h"
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
#include "TournamentContinueDialog.h"
#include "TeamSelectScreen.h"
#include "WorldCupTournament.h"
#include "WMTeamSelectScreen.h"
#include "TournamentIntro.h"
#include "RecordsScreen.h"
#include "Schreiben.h"
#include "FlagQuiz.h"
#include "ClubQuiz.h"
#include "QuizMenu.h"
#include "StadionQuiz.h"
#include "KennzeichenQuiz.h"
#include "InfoScreen.h"
#include "SplashScreen.h"
#include "ThemeSelectScreen.h"
#include "ReitMenu.h"
#include "KiTaMenu.h"
#include "ParcoursMenu.h"
#include "ReitabzeichenMenu.h"
#include "SpringreitenTrainer.h"
#include "ParcoursField.h"
#include "ReitIntro.h"

// Modi
enum AppMode {
  // Profil-Modi
  MODE_PROFILE_SELECT,       // Profilauswahl (direkt spielen)
  MODE_PROFILE_CREATE,       // Tastatur für neues Profil
  MODE_PROFILE_MANAGE,       // Profilverwaltung
  MODE_CONFIRM_DELETE,       // Lösch-Bestätigung
  MODE_CONFIRM_RESET,        // Reset-Bestätigung
  MODE_CONFIRM_EXIT,         // Exit-Bestätigung (Spiel abbrechen)
  // Spiel-Modi
  MODE_MENU,
  MODE_MATHE_SELECTION,
  MODE_EINZELSPIEL_MODUS,
  MODE_DIFFICULTY_SELECTION,
  MODE_TIME_SELECTION,
  MODE_MATHE,
  // Turnier-Modi
  MODE_TOURNAMENT_SELECTION,
  MODE_TOURNAMENT_CONTINUE,        // Fortsetzen-Dialog
  MODE_TEAM_SELECT,                // Teamauswahl
  MODE_TOURNAMENT_MATHE_SELECTION,
  MODE_TOURNAMENT_INTRO,
  MODE_TOURNAMENT,
  MODE_TOURNAMENT_PENALTY,
  MODE_TROPHY,
  // WM 2026 Modi
  MODE_WM_CONTINUE,                // WM Fortsetzen-Dialog
  MODE_WM_TEAM_SELECT,             // WM Teamauswahl
  MODE_WM_MATHE_SELECTION,         // WM Rechenart
  MODE_WM_GROUP_INTRO,             // WM Gruppenphase Intro
  MODE_WM_MATCH_INTRO,             // WM Spielvorschau
  MODE_WM_PLAYING,                 // WM Spiel laeuft
  MODE_WM_GROUP_TABLE,             // WM Gruppentabelle
  MODE_WM_KNOCKOUT_INTRO,          // WM K.O.-Runde Intro
  MODE_WM_PENALTY,                 // WM Elfmeterschiessen
  MODE_WM_ELIMINATED,              // WM Ausgeschieden
  MODE_WM_CHAMPION,                // WM Weltmeister
  // Sonstige Modi
  MODE_RECORDS,                    // Rekordematrix
  MODE_SCHREIBEN,                  // Schreiben-Uebung
  MODE_QUIZ_MENU,                  // Quiz-Auswahl
  MODE_FLAG_QUIZ,                  // WM Flaggen-Quiz
  MODE_CLUB_QUIZ,                  // Vereinsquiz (Bundesland raten)
  MODE_STADION_QUIZ,               // Stadion-Quiz
  MODE_KENNZEICHEN_QUIZ,           // Kennzeichen-Quiz
  // Thema-Modi
  MODE_THEME_SELECT,               // Themenauswahl (Fussball/KiTa/Reiten)
  MODE_KITA_MENU,                  // KiTa-Hauptmenue
  // Reiten-Modi
  MODE_REIT_MENU,                  // Reit-Hauptmenue
  MODE_PARCOURS_MENU,              // Rechenart-Auswahl fuer Parcours
  MODE_REITABZEICHEN_SELECTION,    // Schwierigkeit (Bronze/Silber/Gold)
  MODE_REIT_TIME_SELECTION,        // Zeitauswahl Parcours
  MODE_SPRINGREITEN,               // Springreiten laeuft
  MODE_REIT_TOURNAMENT_SELECTION,  // Turnier-Auswahl
  MODE_REIT_TOURNAMENT_CONTINUE,   // Turnier fortsetzen Dialog
  MODE_REIT_TOURNAMENT_INTRO,      // Turnier-Intro
  MODE_REIT_TOURNAMENT,            // Turnier-Spiel laeuft
  MODE_STECHEN,                    // Stechen (wie Elfmeterschiessen)
  MODE_REIT_TROPHY,                // Pokal-Anzeige
  MODE_REIT_RECORDS                // Reit-Rekorde
};

// Globale Objekte
Menu menu;
MatheMenu matheMenu;
EinzelspielMenu einzelspielMenu;
DifficultyMenu difficultyMenu;
TimeMenu timeMenu;
Mathe matheTrainer;
Touch touch;
TournamentMenu tournamentMenu;
TournamentMatheModeMenu tournamentMatheModeMenu;
Tournament tournament;
TrophyDisplay trophyDisplay;
WorldCupTournament worldCupTournament;
WMTeamSelectScreen wmTeamSelectScreen;
TournamentIntro tournamentIntro;
Schreiben schreiben;
FlagQuiz flagQuiz;
ClubQuiz clubQuiz;
QuizMenu quizMenu;
StadionQuiz stadionQuiz;
KennzeichenQuiz kennzeichenQuiz;
InfoScreen infoScreen;
SplashScreen splashScreen;
ThemeSelectScreen themeSelectScreen;
ReitMenu reitMenu;
KiTaMenu kitaMenu;
ParcoursMenu parcoursMenu;
ReitabzeichenMenu reitabzeichenMenu;
SpringreitenTrainer springreitenTrainer;
ReitIntro reitIntro;

// Gewaehlte Reit-Optionen
Reitabzeichen selectedReitabzeichen;

// Profil-System
SDManager sdManager;
ProfileManager profileManager;
ProfileSelectScreen profileSelectScreen;
ProfileManageScreen profileManageScreen;
KeyboardScreen keyboardScreen;
ConfirmDialog confirmDialog;
TournamentContinueDialog tournamentContinueDialog;
TeamSelectScreen teamSelectScreen;
RecordsScreen recordsScreen;

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

// Modus vor Exit-Bestätigung (um zu wissen ob Turnier oder Einzelspiel)
AppMode modeBeforeExit = MODE_MENU;

// Aktuelles Thema (Fussball oder Reiten)
Theme currentTheme = THEME_FUSSBALL;

// Setter für Profilname und Gegner in mathe.cpp
extern void setPlayerName(const String& name);
extern void setPlayerTeamColor(uint16_t color);
extern void setOpponentName(const String& name);
extern void setOpponentColor(uint16_t color);

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

void showThemeSelect() {
  themeSelectScreen.setPlayerName(currentProfile.name);
  themeSelectScreen.draw();
  currentMode = MODE_THEME_SELECT;
}

void loginProfile(const Profile& profile) {
  currentProfile = profile;
  setPlayerName(currentProfile.name);
  menu.setPlayerName(currentProfile.name);
  reitMenu.setPlayerName(currentProfile.name);
  showThemeSelect();
  Serial.printf("Logged in as: %s (id=%d)\n", currentProfile.name.c_str(), currentProfile.id);
}

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("=== Kopfball Starting ===");

  // Screens initialisieren (vor Splash, damit Flackern nicht sichtbar)
  menu.init();
  profileSelectScreen.init();
  profileManageScreen.init();
  keyboardScreen.init();
  confirmDialog.init();
  tournamentContinueDialog.init();
  tournamentIntro.init();

  // Neue Screens initialisieren
  themeSelectScreen.init();
  reitMenu.init();
  kitaMenu.init();
  parcoursMenu.init();
  reitabzeichenMenu.init();

  // Splash Screen zeigen
  splashScreen.init();
  splashScreen.show();

  // Touch initialisieren
  touch.begin();

  // SD-Karte initialisieren
  bool sdReady = sdManager.begin();

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

  // Timer fuer Mathe-Trainer pruefen (alle 500ms, auch im Penalty-Modus)
  if ((currentMode == MODE_MATHE || currentMode == MODE_TOURNAMENT || currentMode == MODE_TOURNAMENT_PENALTY ||
       currentMode == MODE_WM_PLAYING || currentMode == MODE_WM_PENALTY) && millis() - lastTimerUpdate > 500) {
    lastTimerUpdate = millis();
    matheTrainer.update();
  }

  // Timer fuer Springreiten-Trainer pruefen
  if ((currentMode == MODE_SPRINGREITEN || currentMode == MODE_STECHEN) && millis() - lastTimerUpdate > 500) {
    lastTimerUpdate = millis();
    springreitenTrainer.update();
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

          case MODE_CONFIRM_EXIT:
            {
              int choice = confirmDialog.handleTouch(x, y);
              if (choice == 1) {
                // Ja - Spiel abbrechen
                if (modeBeforeExit == MODE_TOURNAMENT) {
                  // Turnier abbrechen -> zurueck zur Turnier-Auswahl
                  // Save bleibt erhalten (letzte gewonnene Runde)
                  Serial.println("Tournament exit confirmed");
                  tournamentMenu.init();
                  tournamentMenu.draw();
                  currentMode = MODE_TOURNAMENT_SELECTION;
                } else if (modeBeforeExit == MODE_WM_PLAYING) {
                  // WM abbrechen -> zurueck zur Turnier-Auswahl
                  Serial.println("World Cup exit confirmed");
                  tournamentMenu.init();
                  tournamentMenu.draw();
                  currentMode = MODE_TOURNAMENT_SELECTION;
                } else if (modeBeforeExit == MODE_SPRINGREITEN) {
                  // Springreiten abbrechen -> zurueck zum Reit-Menue
                  Serial.println("Springreiten exit confirmed");
                  reitMenu.draw();
                  currentMode = MODE_REIT_MENU;
                } else {
                  // Einzelspiel abbrechen -> zurueck zum Menue
                  Serial.println("Game exit confirmed");
                  menu.draw();
                  currentMode = MODE_MENU;
                }
              } else if (choice == 2) {
                // Nein - zurueck zum Spiel
                if (modeBeforeExit == MODE_SPRINGREITEN) {
                  springreitenTrainer.redrawScreen();
                } else {
                  matheTrainer.redrawScreen();
                }
                currentMode = modeBeforeExit;
              }
            }
            break;

          // ========== THEMENAUSWAHL ==========

          case MODE_THEME_SELECT:
            {
              int choice = themeSelectScreen.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zur Profilauswahl
                Serial.println("Returning to profile selection from theme select");
                showProfileSelect();
              } else if (choice == 1) {
                // Fussball gewaehlt
                Serial.println("Theme: Fussball");
                currentTheme = THEME_FUSSBALL;
                menu.draw();
                currentMode = MODE_MENU;
              } else if (choice == 2) {
                // KiTa gewaehlt
                Serial.println("Theme: KiTa");
                currentTheme = THEME_KITA;
                kitaMenu.setPlayerName(currentProfile.name);
                kitaMenu.draw();
                currentMode = MODE_KITA_MENU;
              } else if (choice == 3) {
                // Reiten gewaehlt
                Serial.println("Theme: Reiten");
                currentTheme = THEME_REITEN;
                reitMenu.draw();
                currentMode = MODE_REIT_MENU;
              }
            }
            break;

          // ========== KITA-MENÜ ==========

          case MODE_KITA_MENU:
            {
              int choice = kitaMenu.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zur Themenauswahl
                Serial.println("Returning to theme selection");
                showThemeSelect();
              } else if (choice == 1) {
                // Schreiben-Uebung oeffnen
                Serial.println("Opening Schreiben from KiTa");
                // Info-Screen zeigen (nur beim ersten Mal)
                if (!profileManager.hasSeenIntro(currentProfile.id, INTRO_SCHREIBEN)) {
                  if (!infoScreen.show(INFO_SCHREIBEN, &touch)) {
                    kitaMenu.draw();
                    break;
                  }
                  profileManager.setIntroSeen(currentProfile.id, INTRO_SCHREIBEN);
                }
                schreiben.init();
                currentMode = MODE_SCHREIBEN;
              }
            }
            break;

          // ========== REIT-MENÜ ==========

          case MODE_REIT_MENU:
            {
              int choice = reitMenu.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zur Themenauswahl
                Serial.println("Returning to theme selection");
                showThemeSelect();
              } else if (choice == 1) {
                // Parcours (Einzelspiel)
                Serial.println("Opening Parcours Menu");
                parcoursMenu.draw();
                currentMode = MODE_PARCOURS_MENU;
              } else if (choice == 2) {
                // Turnier - TODO (Phase 4)
                Serial.println("Opening Reit-Turnier Menu (TODO)");
              } else if (choice == 3) {
                // Rekorde - TODO (Phase 5)
                Serial.println("Opening Reit-Records (TODO)");
              }
            }
            break;

          case MODE_PARCOURS_MENU:
            {
              int choice = parcoursMenu.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zum Reit-Menue
                Serial.println("Returning to Reit Menu");
                reitMenu.draw();
                currentMode = MODE_REIT_MENU;
              } else if (choice >= 1 && choice <= 4) {
                // Rechenart gewaehlt -> weiter zu Reitabzeichen
                if (choice == 1) selectedMatheMode = MATHE_PLUS_MINUS;
                else if (choice == 2) selectedMatheMode = MATHE_MULTIPLY;
                else if (choice == 3) selectedMatheMode = MATHE_DIVIDE;
                else selectedMatheMode = MATHE_ALL;

                Serial.printf("Selected Mathe mode: %d\n", selectedMatheMode);
                reitabzeichenMenu.draw();
                currentMode = MODE_REITABZEICHEN_SELECTION;
              }
            }
            break;

          case MODE_REITABZEICHEN_SELECTION:
            {
              int choice = reitabzeichenMenu.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zur Rechenart-Auswahl
                Serial.println("Returning to Parcours Menu");
                parcoursMenu.draw();
                currentMode = MODE_PARCOURS_MENU;
              } else if (choice >= 1 && choice <= 3) {
                // Reitabzeichen gewaehlt
                if (choice == 1) selectedReitabzeichen = REITABZEICHEN_BRONZE;
                else if (choice == 2) selectedReitabzeichen = REITABZEICHEN_SILBER;
                else selectedReitabzeichen = REITABZEICHEN_GOLD;

                // Intro-Screen anzeigen (blockiert bis Button)
                Serial.println("Showing Reit Intro");
                reitIntro.showIntro(selectedReitabzeichen, &touch);

                // Dann Spiel starten
                Serial.printf("Starting Springreiten: mode=%d, level=%d\n",
                              selectedMatheMode, selectedReitabzeichen);
                springreitenTrainer.init(selectedMatheMode, selectedReitabzeichen);
                currentMode = MODE_SPRINGREITEN;
              }
            }
            break;

          case MODE_SPRINGREITEN:
            {
              int result = springreitenTrainer.handleButtonPress(x, y);
              if (result == 1) {
                // Home (nach Game Over)
                Serial.println("Returning to Reit Menu");
                reitMenu.draw();
                currentMode = MODE_REIT_MENU;
              } else if (result == 2) {
                // Exit angefragt
                modeBeforeExit = MODE_SPRINGREITEN;
                confirmDialog.show("ABBRECHEN", "Parcours wirklich beenden?");
                currentMode = MODE_CONFIRM_EXIT;
              }
            }
            break;

          // ========== HAUPTMENÜ (FUSSBALL) ==========

          case MODE_MENU:
            {
              int choice = menu.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zur Themenauswahl
                Serial.println("Returning to theme selection");
                showThemeSelect();
              } else if (choice == 1) {
                // Mathe-Fach oeffnen
                Serial.println("Opening Mathe Menu");
                matheMenu.init();
                matheMenu.draw();
                currentMode = MODE_MATHE_SELECTION;
              } else if (choice == 2) {
                // Rekordematrix oeffnen
                Serial.println("Opening Records Screen");
                RecordsMatrix records = profileManager.getRecords(currentProfile.id);
                recordsScreen.init(records, currentProfile.name,
                                   currentProfile.flagQuizBestMedal,
                                   currentProfile.clubQuizBestMedal,
                                   currentProfile.stadionQuizBestMedal,
                                   currentProfile.kennzeichenQuizBestMedal);
                recordsScreen.draw();
                currentMode = MODE_RECORDS;
              } else if (choice == 3) {
                // Quiz-Menue oeffnen
                Serial.println("Opening Quiz Menu");
                quizMenu.init();
                quizMenu.draw();
                currentMode = MODE_QUIZ_MENU;
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
                // Einzelspiel -> Modus wählen (+/- oder 1x1)
                Serial.println("Opening Einzelspiel Menu");
                einzelspielMenu.init();
                einzelspielMenu.draw();
                currentMode = MODE_EINZELSPIEL_MODUS;
              } else if (matheChoice == 2) {
                // Pokal-Modus
                Serial.println("Opening Tournament Menu");
                tournamentMenu.init();
                tournamentMenu.draw();
                currentMode = MODE_TOURNAMENT_SELECTION;
              }
            }
            break;

          case MODE_EINZELSPIEL_MODUS:
            {
              int choice = einzelspielMenu.handleTouch(x, y);
              if (choice == 0) {
                // Zurück zur Mathe-Auswahl
                Serial.println("Returning to Mathe Selection");
                matheMenu.draw();
                currentMode = MODE_MATHE_SELECTION;
              } else if (choice == 1) {
                // +/- Modus gewählt -> weiter zu Schwierigkeit
                Serial.println("Selected: +/-");
                selectedMatheMode = MATHE_PLUS_MINUS;
                difficultyMenu.init();
                difficultyMenu.draw();
                currentMode = MODE_DIFFICULTY_SELECTION;
              } else if (choice == 2) {
                // 1x1 Modus gewählt -> weiter zu Schwierigkeit
                Serial.println("Selected: 1x1");
                selectedMatheMode = MATHE_MULTIPLY;
                difficultyMenu.init();
                difficultyMenu.draw();
                currentMode = MODE_DIFFICULTY_SELECTION;
              } else if (choice == 3) {
                // 1:1 Modus gewählt -> weiter zu Schwierigkeit
                Serial.println("Selected: 1:1");
                selectedMatheMode = MATHE_DIVIDE;
                difficultyMenu.init();
                difficultyMenu.draw();
                currentMode = MODE_DIFFICULTY_SELECTION;
              } else if (choice == 4) {
                // Alle Modus gewählt -> weiter zu Schwierigkeit
                Serial.println("Selected: Alle");
                selectedMatheMode = MATHE_ALL;
                difficultyMenu.init();
                difficultyMenu.draw();
                currentMode = MODE_DIFFICULTY_SELECTION;
              }
            }
            break;

          case MODE_DIFFICULTY_SELECTION:
            {
              int diffChoice = difficultyMenu.handleTouch(x, y);
              if (diffChoice == 0) {
                // Zurück zur Einzelspiel-Modus-Auswahl
                Serial.println("Returning to Einzelspiel Menu");
                einzelspielMenu.draw();
                currentMode = MODE_EINZELSPIEL_MODUS;
              } else if (diffChoice == 1) {
                // St. Vit (erlernen) -> Direkt starten mit 1 Minute
                Serial.println("Selected: St. Vit (erlernen)");
                selectedDifficulty = 1;
                // Info-Screen zeigen (nur beim ersten Mal)
                if (!profileManager.hasSeenIntro(currentProfile.id, INTRO_EINZELSPIEL)) {
                  if (!infoScreen.show(INFO_EINZELSPIEL, &touch)) {
                    difficultyMenu.draw();
                    break;
                  }
                  profileManager.setIntroSeen(currentProfile.id, INTRO_EINZELSPIEL);
                }
                Difficulty diff = DifficultyMenu::getDifficulty(1);
                matheTrainer.clearTournamentMode();  // Keine Turnier-Logos
                matheTrainer.init(selectedMatheMode, 60000, diff.answerTimeMs, diff.stepsForGoal);
                currentMode = MODE_MATHE;
              } else if (diffChoice >= 2 && diffChoice <= 4) {
                // Hannover, Dortmund, Bayern -> weiter zu Zeit
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
                  case 2: duration = 120000; break;  // 2 Minuten
                  case 3: duration = 180000; break;  // 3 Minuten
                  default: duration = 120000; break;
                }

                // Info-Screen zeigen (nur beim ersten Mal)
                if (!profileManager.hasSeenIntro(currentProfile.id, INTRO_EINZELSPIEL)) {
                  if (!infoScreen.show(INFO_EINZELSPIEL, &touch)) {
                    timeMenu.draw();
                    break;
                  }
                  profileManager.setIntroSeen(currentProfile.id, INTRO_EINZELSPIEL);
                }

                Difficulty diff = DifficultyMenu::getDifficulty(selectedDifficulty);
                Serial.printf("Starting game: mode=%d, time=%lu, answerTime=%lu, steps=%d\n",
                              selectedMatheMode, duration, diff.answerTimeMs, diff.stepsForGoal);

                matheTrainer.clearTournamentMode();  // Keine Turnier-Logos
                matheTrainer.init(selectedMatheMode, duration, diff.answerTimeMs, diff.stepsForGoal);
                currentMode = MODE_MATHE;
              }
            }
            break;

          case MODE_MATHE:
            {
              int result = matheTrainer.handleButtonPress(x, y);
              if (result == 1) {
                // Home (nach Game Over)
                Serial.println("Returning to menu");
                menu.draw();
                currentMode = MODE_MENU;
              } else if (result == 2) {
                // Exit angefragt
                modeBeforeExit = MODE_MATHE;
                confirmDialog.show("ABBRECHEN", "Spiel wirklich beenden?");
                currentMode = MODE_CONFIRM_EXIT;
              }
            }
            break;

          // ========== TURNIER-MODI ==========

          case MODE_TOURNAMENT_SELECTION:
            {
              int choice = tournamentMenu.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zum Hauptmenue
                menu.draw();
                currentMode = MODE_MENU;
              } else if (choice == 1) {
                // DFB-Pokal gewaehlt
                selectedTournamentType = TOURNAMENT_DFB_POKAL;
                // Pruefen ob Spielstand existiert
                if (profileManager.hasTournamentSave(currentProfile.id, TOURNAMENT_DFB_POKAL)) {
                  TournamentSave save = profileManager.loadTournamentSave(currentProfile.id, TOURNAMENT_DFB_POKAL);
                  tournamentContinueDialog.show(save);
                  currentMode = MODE_TOURNAMENT_CONTINUE;
                } else {
                  // Kein Save -> Teamauswahl
                  teamSelectScreen.init(TOURNAMENT_DFB_POKAL, currentProfile.favTeamDFB);
                  teamSelectScreen.draw();
                  currentMode = MODE_TEAM_SELECT;
                }
              } else if (choice == 2) {
                // Champions League gewaehlt
                selectedTournamentType = TOURNAMENT_CHAMPIONS_LEAGUE;
                // Pruefen ob Spielstand existiert
                if (profileManager.hasTournamentSave(currentProfile.id, TOURNAMENT_CHAMPIONS_LEAGUE)) {
                  TournamentSave save = profileManager.loadTournamentSave(currentProfile.id, TOURNAMENT_CHAMPIONS_LEAGUE);
                  tournamentContinueDialog.show(save);
                  currentMode = MODE_TOURNAMENT_CONTINUE;
                } else {
                  // Kein Save -> Teamauswahl
                  teamSelectScreen.init(TOURNAMENT_CHAMPIONS_LEAGUE, currentProfile.favTeamCL);
                  teamSelectScreen.draw();
                  currentMode = MODE_TEAM_SELECT;
                }
              } else if (choice == 3) {
                // WM 2026 gewaehlt
                selectedTournamentType = TOURNAMENT_WORLD_CUP_2026;
                // Pruefen ob WM-Spielstand existiert
                if (profileManager.hasWorldCupSave(currentProfile.id)) {
                  WorldCupSave save = profileManager.loadWorldCupSave(currentProfile.id);
                  tournamentContinueDialog.showWC(save);
                  currentMode = MODE_WM_CONTINUE;
                } else {
                  // Kein Save -> Teamauswahl
                  wmTeamSelectScreen.init(currentProfile.favTeamWC);
                  wmTeamSelectScreen.draw();
                  currentMode = MODE_WM_TEAM_SELECT;
                }
              }
            }
            break;

          case MODE_TOURNAMENT_CONTINUE:
            {
              int choice = tournamentContinueDialog.handleTouch(x, y);
              if (choice == 1) {
                // Fortsetzen - Save laden und Spielerteam setzen
                TournamentSave save = profileManager.loadTournamentSave(currentProfile.id, selectedTournamentType);
                tournament.loadFromSave(save);
                // Spielerteam aus Profil setzen
                FavoriteTeam favTeam = (selectedTournamentType == TOURNAMENT_DFB_POKAL)
                  ? currentProfile.favTeamDFB : currentProfile.favTeamCL;
                tournament.setPlayerTeam(favTeam.name, favTeam.abbrev, favTeam.primaryColor);
                tournamentIntro.showIntro(selectedTournamentType, &touch);
                tournament.draw();
                currentMode = MODE_TOURNAMENT_INTRO;
              } else if (choice == 2) {
                // Neu starten - Save löschen, Teamauswahl
                profileManager.deleteTournamentSave(currentProfile.id, selectedTournamentType);
                FavoriteTeam favTeam = (selectedTournamentType == TOURNAMENT_DFB_POKAL)
                  ? currentProfile.favTeamDFB : currentProfile.favTeamCL;
                teamSelectScreen.init(selectedTournamentType, favTeam);
                teamSelectScreen.draw();
                currentMode = MODE_TEAM_SELECT;
              }
            }
            break;

          case MODE_WM_CONTINUE:
            {
              int choice = tournamentContinueDialog.handleTouch(x, y);
              if (choice == 1) {
                // Fortsetzen - WM-Save laden
                WorldCupSave save = profileManager.loadWorldCupSave(currentProfile.id);
                worldCupTournament.loadFromSave(save);
                worldCupTournament.draw();
                // Basierend auf State zum richtigen Modus wechseln
                WCState state = worldCupTournament.getState();
                if (state == WC_GROUP_TABLE) {
                  currentMode = MODE_WM_GROUP_TABLE;
                } else if (state == WC_KNOCKOUT_INTRO) {
                  currentMode = MODE_WM_KNOCKOUT_INTRO;
                } else if (state == WC_GROUP_COMPLETE) {
                  currentMode = MODE_WM_GROUP_INTRO;
                } else {
                  currentMode = MODE_WM_MATCH_INTRO;
                }
              } else if (choice == 2) {
                // Neu starten - WM-Save loeschen, Teamauswahl
                profileManager.deleteWorldCupSave(currentProfile.id);
                wmTeamSelectScreen.init(currentProfile.favTeamWC);
                wmTeamSelectScreen.draw();
                currentMode = MODE_WM_TEAM_SELECT;
              }
            }
            break;

          case MODE_TEAM_SELECT:
            {
              int choice = teamSelectScreen.handleTouch(x, y);
              if (choice == 0) {
                // Zurück zur Turnier-Auswahl
                tournamentMenu.init();
                tournamentMenu.draw();
                currentMode = MODE_TOURNAMENT_SELECTION;
              } else if (choice == 1) {
                // Team gewählt - speichern
                FavoriteTeam team = teamSelectScreen.getSelectedTeam();
                profileManager.setFavoriteTeam(currentProfile.id, selectedTournamentType, team);
                // Profil aktualisieren
                if (selectedTournamentType == TOURNAMENT_DFB_POKAL) {
                  currentProfile.favTeamDFB = team;
                } else {
                  currentProfile.favTeamCL = team;
                }

                // Weiter zum Mathe-Modus (fuer beide: DFB und CL)
                tournamentMatheModeMenu.init();
                tournamentMatheModeMenu.draw();
                currentMode = MODE_TOURNAMENT_MATHE_SELECTION;
              }
            }
            break;

          case MODE_TOURNAMENT_MATHE_SELECTION:
            {
              int choice = tournamentMatheModeMenu.handleTouch(x, y);
              // Korrektes Lieblingsteam basierend auf Turnier
              FavoriteTeam favTeam = (selectedTournamentType == TOURNAMENT_DFB_POKAL)
                ? currentProfile.favTeamDFB : currentProfile.favTeamCL;

              if (choice == 0) {
                // Zurück zur Teamauswahl
                teamSelectScreen.init(selectedTournamentType, favTeam);
                teamSelectScreen.draw();
                currentMode = MODE_TEAM_SELECT;
              } else if (choice >= 1 && choice <= 4) {
                // Rechenart gewählt
                if (choice == 1) selectedMatheMode = MATHE_PLUS_MINUS;
                else if (choice == 2) selectedMatheMode = MATHE_MULTIPLY;
                else if (choice == 3) selectedMatheMode = MATHE_DIVIDE;
                else selectedMatheMode = MATHE_ALL;

                // Info-Screen zeigen (nur beim ersten Mal pro Turnier-Typ)
                uint16_t introFlag = (selectedTournamentType == TOURNAMENT_DFB_POKAL) ? INTRO_DFB_POKAL : INTRO_CL;
                InfoType infoType = (selectedTournamentType == TOURNAMENT_DFB_POKAL) ? INFO_DFB_POKAL : INFO_CHAMPIONS_LEAGUE;
                if (!profileManager.hasSeenIntro(currentProfile.id, introFlag)) {
                  if (!infoScreen.show(infoType, &touch)) {
                    tournamentMatheModeMenu.draw();
                    break;
                  }
                  profileManager.setIntroSeen(currentProfile.id, introFlag);
                }

                tournament.init(selectedTournamentType, selectedMatheMode);
                tournament.setPlayerTeam(favTeam.name, favTeam.abbrev, favTeam.primaryColor);
                tournamentIntro.showIntro(selectedTournamentType, &touch);
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
                // Spielerteam für Anzeige setzen (Abkürzung für kompakte Darstellung)
                setPlayerName(tournament.getPlayerTeamAbbrev());
                setPlayerTeamColor(tournament.getPlayerTeamColor());
                // Gegner setzen
                Team opponent = tournament.getCurrentOpponent();
                setOpponentName(opponent.abbrev);
                setOpponentColor(opponent.primaryColor);

                // 90 Sekunden regulär + 10-35 Sekunden Nachspielzeit
                int nachspielzeitSek = random(10, 36);  // 10-35 Sek
                Serial.printf("Spielzeit: 1:30 + %d Sek Nachspielzeit\n", nachspielzeitSek);

                matheTrainer.init(
                  tournament.getMatheMode(),
                  90000,  // 90 Sekunden reguläre Spielzeit
                  tournament.getAnswerTime(),
                  tournament.getStepsForGoal(),
                  nachspielzeitSek
                );
                // Logos auf Scoreboard aktivieren
                matheTrainer.setTournamentMode(selectedTournamentType);
                currentMode = MODE_TOURNAMENT;
              }
            }
            break;

          case MODE_TOURNAMENT:
            {
              // Touch verarbeiten (Numpad etc.)
              int result = matheTrainer.handleButtonPress(x, y);

              // Exit angefragt?
              if (result == 2) {
                modeBeforeExit = MODE_TOURNAMENT;
                confirmDialog.show("ABBRECHEN", "Turnier wirklich beenden?");
                currentMode = MODE_CONFIRM_EXIT;
                break;
              }

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
                  // Turnier gewonnen -> Jubel-Animation zeigen!
                  matheTrainer.showChampionCelebration();
                  // Save löschen + Rekord eintragen
                  profileManager.deleteTournamentSave(currentProfile.id, selectedTournamentType);
                  profileManager.recordTournamentWin(currentProfile.id, selectedTournamentType, selectedMatheMode);
                  trophyDisplay.init();
                  trophyDisplay.showTrophy(selectedTournamentType);
                  currentMode = MODE_TROPHY;
                } else if (tournament.getState() == TOURNAMENT_PENALTY) {
                  // Elfmeterschießen starten
                  tournament.draw();
                  delay(2000);  // Kurz "ELFMETERSCHIESSEN" zeigen
                  // Gegner-Info für Penalty setzen
                  Team opponent = tournament.getCurrentOpponent();
                  setOpponentName(opponent.abbrev);
                  setOpponentColor(opponent.primaryColor);
                  matheTrainer.initPenalty(
                    tournament.getMatheMode(),
                    tournament.getAnswerTime()
                  );
                  currentMode = MODE_TOURNAMENT_PENALTY;
                } else if (tournament.getState() == TOURNAMENT_ROUND_WON) {
                  // Runde gewonnen -> Save mit naechster Runde speichern
                  TournamentSave save = tournament.createSaveForNextRound();
                  profileManager.saveTournament(currentProfile.id, save);
                  tournament.draw();
                  currentMode = MODE_TOURNAMENT_INTRO;
                } else {
                  // ROUND_LOST -> Save löschen
                  profileManager.deleteTournamentSave(currentProfile.id, selectedTournamentType);
                  tournament.draw();
                  currentMode = MODE_TOURNAMENT_INTRO;
                }
              }
            }
            break;

          case MODE_TOURNAMENT_PENALTY:
            {
              // Elfmeterschießen - Numpad für Aufgabe
              int result = matheTrainer.handleButtonPress(x, y);

              // Exit angefragt?
              if (result == 2) {
                modeBeforeExit = MODE_TOURNAMENT;  // Bei Exit zurück zur Turnier-Auswahl
                confirmDialog.show("ABBRECHEN", "Turnier wirklich beenden?");
                currentMode = MODE_CONFIRM_EXIT;
                break;
              }

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
                    // Gewonnen -> Jubel-Animation zeigen!
                    matheTrainer.showChampionCelebration();
                    // Save löschen + Rekord eintragen
                    profileManager.deleteTournamentSave(currentProfile.id, selectedTournamentType);
                    profileManager.recordTournamentWin(currentProfile.id, selectedTournamentType, selectedMatheMode);
                    trophyDisplay.init();
                    trophyDisplay.showTrophy(selectedTournamentType);
                    currentMode = MODE_TROPHY;
                  } else if (tournament.getState() == TOURNAMENT_ROUND_WON) {
                    // Runde gewonnen -> Save mit naechster Runde speichern
                    TournamentSave save = tournament.createSaveForNextRound();
                    profileManager.saveTournament(currentProfile.id, save);
                    tournament.draw();
                    currentMode = MODE_TOURNAMENT_INTRO;
                  } else {
                    // Verloren -> Save löschen
                    profileManager.deleteTournamentSave(currentProfile.id, selectedTournamentType);
                    tournament.draw();
                    currentMode = MODE_TOURNAMENT_INTRO;
                  }
                } else {
                  // Nächster Elfmeter - neues Penalty-Spiel starten
                  // Gegner-Info aktualisieren
                  Team opponent = tournament.getCurrentOpponent();
                  setOpponentName(opponent.abbrev);
                  setOpponentColor(opponent.primaryColor);
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

          // ========== WM 2026 MODI ==========

          case MODE_WM_TEAM_SELECT:
            {
              int choice = wmTeamSelectScreen.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zur Turnier-Auswahl
                tournamentMenu.init();
                tournamentMenu.draw();
                currentMode = MODE_TOURNAMENT_SELECTION;
              } else if (choice == 1) {
                // Team gewaehlt - speichern
                FavoriteTeam team = wmTeamSelectScreen.getSelectedTeam();
                profileManager.setFavoriteTeam(currentProfile.id, TOURNAMENT_WORLD_CUP_2026, team);
                currentProfile.favTeamWC = team;

                // Weiter zur Rechenart-Auswahl
                tournamentMatheModeMenu.init();
                tournamentMatheModeMenu.draw();
                currentMode = MODE_WM_MATHE_SELECTION;
              }
            }
            break;

          case MODE_WM_MATHE_SELECTION:
            {
              int choice = tournamentMatheModeMenu.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zur Teamauswahl
                wmTeamSelectScreen.init(currentProfile.favTeamWC);
                wmTeamSelectScreen.draw();
                currentMode = MODE_WM_TEAM_SELECT;
              } else if (choice >= 1 && choice <= 4) {
                // Rechenart gewaehlt
                if (choice == 1) selectedMatheMode = MATHE_PLUS_MINUS;
                else if (choice == 2) selectedMatheMode = MATHE_MULTIPLY;
                else if (choice == 3) selectedMatheMode = MATHE_DIVIDE;
                else selectedMatheMode = MATHE_ALL;

                // Info-Screen zeigen (nur beim ersten Mal)
                if (!profileManager.hasSeenIntro(currentProfile.id, INTRO_WM)) {
                  if (!infoScreen.show(INFO_WM, &touch)) {
                    tournamentMatheModeMenu.draw();
                    break;
                  }
                  profileManager.setIntroSeen(currentProfile.id, INTRO_WM);
                }

                // WM-Turnier starten
                int teamIndex = wmTeamSelectScreen.getSelectedTeamIndex();
                worldCupTournament.init(selectedMatheMode, teamIndex);
                worldCupTournament.draw();
                currentMode = MODE_WM_GROUP_INTRO;
              }
            }
            break;

          case MODE_WM_GROUP_INTRO:
            {
              int choice = worldCupTournament.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zur Turnier-Auswahl
                tournamentMenu.init();
                tournamentMenu.draw();
                currentMode = MODE_TOURNAMENT_SELECTION;
              } else if (choice == 1 || worldCupTournament.getState() == WC_MATCH_INTRO) {
                // Match Intro anzeigen
                worldCupTournament.draw();
                currentMode = MODE_WM_MATCH_INTRO;
              }
            }
            break;

          case MODE_WM_MATCH_INTRO:
            {
              int choice = worldCupTournament.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zum Menue
                tournamentMenu.init();
                tournamentMenu.draw();
                currentMode = MODE_TOURNAMENT_SELECTION;
              } else if (choice == 1) {
                // Anpfiff! - Spiel starten
                const WMTeam& playerTeam = worldCupTournament.getPlayerTeam();
                const WMTeam& opponent = worldCupTournament.getCurrentOpponent();

                setPlayerName(playerTeam.abbrev);
                setPlayerTeamColor(getTextColorForBg(playerTeam.shirtColor));
                setOpponentName(opponent.abbrev);
                setOpponentColor(getTextColorForBg(opponent.shirtColor));

                // Spielzeit: 90 Sek + Nachspielzeit
                int nachspielzeitSek = random(10, 36);
                Serial.printf("WM Spielzeit: 1:30 + %d Sek Nachspielzeit\n", nachspielzeitSek);

                matheTrainer.init(
                  worldCupTournament.getMatheMode(),
                  90000,  // 90 Sekunden
                  worldCupTournament.getAnswerTime(),
                  worldCupTournament.getStepsForGoal(),
                  nachspielzeitSek
                );
                // Flaggen auf Scoreboard aktivieren (mit Team-Indices fuer WM)
                matheTrainer.setTournamentMode(
                  TOURNAMENT_WORLD_CUP_2026,
                  worldCupTournament.getPlayerTeamIndex(),
                  worldCupTournament.getCurrentOpponentIndex()
                );
                currentMode = MODE_WM_PLAYING;
              }
            }
            break;

          case MODE_WM_PLAYING:
            {
              int result = matheTrainer.handleButtonPress(x, y);

              // Exit angefragt?
              if (result == 2) {
                modeBeforeExit = MODE_WM_PLAYING;
                confirmDialog.show("ABBRECHEN", "WM wirklich beenden?");
                currentMode = MODE_CONFIRM_EXIT;
                break;
              }

              // Pruefen ob Spiel vorbei
              if (matheTrainer.isGameOver()) {
                int playerGoals = matheTrainer.getGoalsScored();
                int cpuGoals = matheTrainer.getGoalsAgainst();

                Serial.printf("WM Match ended: %d : %d\n", playerGoals, cpuGoals);

                // In Gruppenphase oder K.O.?
                if (worldCupTournament.getKnockoutRound() < 0) {
                  // Gruppenphase
                  worldCupTournament.handleGroupMatchResult(playerGoals, cpuGoals);
                  // Auto-Save nach Gruppenspiel
                  WorldCupSave wcSave = worldCupTournament.createSave();
                  profileManager.saveWorldCup(currentProfile.id, wcSave);
                  worldCupTournament.draw();
                  currentMode = MODE_WM_GROUP_TABLE;
                } else {
                  // K.O.-Phase
                  if (playerGoals == cpuGoals) {
                    // Elfmeterschiessen
                    worldCupTournament.startPenaltyShootout(playerGoals, cpuGoals);
                    const WMTeam& opponent = worldCupTournament.getCurrentOpponent();
                    setOpponentName(opponent.abbrev);
                    setOpponentColor(getTextColorForBg(opponent.shirtColor));
                    matheTrainer.initPenalty(
                      worldCupTournament.getMatheMode(),
                      worldCupTournament.getAnswerTime()
                    );
                    currentMode = MODE_WM_PENALTY;
                  } else {
                    Serial.printf("main: K.O. result player=%d cpu=%d\n", playerGoals, cpuGoals);
                    worldCupTournament.handleKnockoutMatchResult(playerGoals, cpuGoals);
                    WCState wcState = worldCupTournament.getState();
                    Serial.printf("main: After K.O. result, state=%d\n", wcState);

                    if (wcState == WC_CHAMPION) {
                      // WM gewonnen -> Jubel-Animation zeigen!
                      matheTrainer.showChampionCelebration();
                      // Rekord eintragen, Save loeschen
                      profileManager.recordTournamentWin(currentProfile.id, TOURNAMENT_WORLD_CUP_2026, worldCupTournament.getMatheMode());
                      profileManager.deleteWorldCupSave(currentProfile.id);
                      trophyDisplay.init();
                      trophyDisplay.showTrophy(TOURNAMENT_WORLD_CUP_2026);
                      currentMode = MODE_TROPHY;
                    } else if (wcState == WC_ELIMINATED || playerGoals < cpuGoals) {
                      // Ausgeschieden - Save loeschen
                      // Zusaetzlicher Check: Wenn Spieler weniger Tore hat, ist er raus
                      Serial.println("main: Player ELIMINATED");
                      profileManager.deleteWorldCupSave(currentProfile.id);
                      worldCupTournament.setState(WC_ELIMINATED);  // Sicherstellen
                      worldCupTournament.draw();
                      currentMode = MODE_WM_ELIMINATED;
                    } else if (wcState == WC_KNOCKOUT_INTRO && playerGoals > cpuGoals) {
                      // Naechste Runde - nur wenn Spieler gewonnen hat
                      Serial.println("main: Player advances to next round");
                      WorldCupSave wcSave = worldCupTournament.createSave();
                      profileManager.saveWorldCup(currentProfile.id, wcSave);
                      worldCupTournament.draw();
                      currentMode = MODE_WM_KNOCKOUT_INTRO;
                    } else {
                      // Fallback - sollte nicht passieren
                      Serial.printf("main: Unexpected state %d after K.O. match!\n", wcState);
                      worldCupTournament.draw();
                      currentMode = MODE_WM_KNOCKOUT_INTRO;
                    }
                  }
                }
              }
            }
            break;

          case MODE_WM_GROUP_TABLE:
            {
              int choice = worldCupTournament.handleTouch(x, y);
              if (choice == -1) {
                // Weiter gedrueckt - State hat sich geaendert
                WCState state = worldCupTournament.getState();
                if (state == WC_MATCH_INTRO) {
                  currentMode = MODE_WM_MATCH_INTRO;
                } else if (state == WC_GROUP_COMPLETE) {
                  currentMode = MODE_WM_GROUP_INTRO;  // Zeigt Gruppen-Abschluss
                } else if (state == WC_KNOCKOUT_INTRO) {
                  currentMode = MODE_WM_KNOCKOUT_INTRO;
                } else if (state == WC_ELIMINATED) {
                  currentMode = MODE_WM_ELIMINATED;
                }
              }
            }
            break;

          case MODE_WM_KNOCKOUT_INTRO:
            {
              int choice = worldCupTournament.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck
                tournamentMenu.init();
                tournamentMenu.draw();
                currentMode = MODE_TOURNAMENT_SELECTION;
              } else if (choice == 1) {
                // Anpfiff - K.O.-Spiel starten
                const WMTeam& playerTeam = worldCupTournament.getPlayerTeam();
                const WMTeam& opponent = worldCupTournament.getCurrentOpponent();

                setPlayerName(playerTeam.abbrev);
                setPlayerTeamColor(getTextColorForBg(playerTeam.shirtColor));
                setOpponentName(opponent.abbrev);
                setOpponentColor(getTextColorForBg(opponent.shirtColor));

                int nachspielzeitSek = random(10, 36);
                matheTrainer.init(
                  worldCupTournament.getMatheMode(),
                  90000,
                  worldCupTournament.getAnswerTime(),
                  worldCupTournament.getStepsForGoal(),
                  nachspielzeitSek
                );
                // Flaggen auf Scoreboard aktivieren (mit Team-Indices fuer K.O.-Runde)
                matheTrainer.setTournamentMode(
                  TOURNAMENT_WORLD_CUP_2026,
                  worldCupTournament.getPlayerTeamIndex(),
                  worldCupTournament.getCurrentOpponentIndex()
                );
                currentMode = MODE_WM_PLAYING;
              }
            }
            break;

          case MODE_WM_PENALTY:
            {
              int result = matheTrainer.handleButtonPress(x, y);

              if (result == 2) {
                modeBeforeExit = MODE_WM_PLAYING;
                confirmDialog.show("ABBRECHEN", "WM wirklich beenden?");
                currentMode = MODE_CONFIRM_EXIT;
                break;
              }

              if (matheTrainer.isPenaltyAnswered()) {
                bool playerScored = matheTrainer.wasPenaltyCorrect();
                bool cpuScored = (random(0, 100) < worldCupTournament.getCpuPenaltyProb());
                int currentRound = worldCupTournament.getPenaltyRound();

                bool shootoutDone = worldCupTournament.handlePenalty(playerScored, cpuScored);

                matheTrainer.showPenaltyResult(
                  playerScored,
                  cpuScored,
                  worldCupTournament.getPlayerPenalties(),
                  worldCupTournament.getCpuPenalties(),
                  currentRound
                );

                if (shootoutDone) {
                  if (worldCupTournament.getState() == WC_CHAMPION) {
                    // WM gewonnen -> Jubel-Animation zeigen!
                    matheTrainer.showChampionCelebration();
                    // Rekord eintragen, Save loeschen
                    profileManager.recordTournamentWin(currentProfile.id, TOURNAMENT_WORLD_CUP_2026, worldCupTournament.getMatheMode());
                    profileManager.deleteWorldCupSave(currentProfile.id);
                    trophyDisplay.init();
                    trophyDisplay.showTrophy(TOURNAMENT_WORLD_CUP_2026);
                    currentMode = MODE_TROPHY;
                  } else if (worldCupTournament.getState() == WC_ELIMINATED) {
                    // Ausgeschieden - Save loeschen
                    profileManager.deleteWorldCupSave(currentProfile.id);
                    worldCupTournament.draw();
                    currentMode = MODE_WM_ELIMINATED;
                  } else {
                    // Naechste Runde - Auto-Save
                    WorldCupSave wcSave = worldCupTournament.createSave();
                    profileManager.saveWorldCup(currentProfile.id, wcSave);
                    worldCupTournament.draw();
                    currentMode = MODE_WM_KNOCKOUT_INTRO;
                  }
                } else {
                  const WMTeam& opponent = worldCupTournament.getCurrentOpponent();
                  setOpponentName(opponent.abbrev);
                  setOpponentColor(getTextColorForBg(opponent.shirtColor));
                  matheTrainer.initPenalty(
                    worldCupTournament.getMatheMode(),
                    worldCupTournament.getAnswerTime()
                  );
                }
              }
            }
            break;

          case MODE_WM_ELIMINATED:
            {
              int choice = worldCupTournament.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zum Menue
                menu.draw();
                currentMode = MODE_MENU;
              } else if (choice == 2) {
                // Zuschauer-Modus (TODO)
                menu.draw();
                currentMode = MODE_MENU;
              }
            }
            break;

          case MODE_WM_CHAMPION:
            {
              int choice = worldCupTournament.handleTouch(x, y);
              if (choice == 0) {
                menu.draw();
                currentMode = MODE_MENU;
              }
            }
            break;

          case MODE_RECORDS:
            {
              int choice = recordsScreen.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zum Hauptmenue
                menu.draw();
                currentMode = MODE_MENU;
              }
            }
            break;

          case MODE_SCHREIBEN:
            {
              int choice = schreiben.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zum KiTa-Menue
                kitaMenu.draw();
                currentMode = MODE_KITA_MENU;
              }
            }
            break;

          case MODE_QUIZ_MENU:
            {
              int choice = quizMenu.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zum Hauptmenue
                menu.draw();
                currentMode = MODE_MENU;
              } else if (choice == 1) {
                // Flaggen-Quiz
                Serial.println("Opening Flag Quiz");
                if (!profileManager.hasSeenIntro(currentProfile.id, INTRO_FLAG_QUIZ)) {
                  if (!infoScreen.show(INFO_FLAG_QUIZ, &touch)) {
                    quizMenu.draw();
                    break;
                  }
                  profileManager.setIntroSeen(currentProfile.id, INTRO_FLAG_QUIZ);
                }
                flagQuiz.init();
                currentMode = MODE_FLAG_QUIZ;
              } else if (choice == 2) {
                // Vereine-Quiz
                Serial.println("Opening Club Quiz");
                if (!profileManager.hasSeenIntro(currentProfile.id, INTRO_CLUB_QUIZ)) {
                  if (!infoScreen.show(INFO_CLUB_QUIZ, &touch)) {
                    quizMenu.draw();
                    break;
                  }
                  profileManager.setIntroSeen(currentProfile.id, INTRO_CLUB_QUIZ);
                }
                clubQuiz.init();
                currentMode = MODE_CLUB_QUIZ;
              } else if (choice == 3) {
                // Stadien-Quiz
                Serial.println("Opening Stadion Quiz");
                if (!profileManager.hasSeenIntro(currentProfile.id, INTRO_STADION_QUIZ)) {
                  if (!infoScreen.show(INFO_STADION_QUIZ, &touch)) {
                    quizMenu.draw();
                    break;
                  }
                  profileManager.setIntroSeen(currentProfile.id, INTRO_STADION_QUIZ);
                }
                stadionQuiz.init();
                currentMode = MODE_STADION_QUIZ;
              } else if (choice == 4) {
                // Kennzeichen-Quiz
                Serial.println("Opening Kennzeichen Quiz");
                if (!profileManager.hasSeenIntro(currentProfile.id, INTRO_KENNZEICHEN)) {
                  if (!infoScreen.show(INFO_KENNZEICHEN_QUIZ, &touch)) {
                    quizMenu.draw();
                    break;
                  }
                  profileManager.setIntroSeen(currentProfile.id, INTRO_KENNZEICHEN);
                }
                kennzeichenQuiz.init();
                currentMode = MODE_KENNZEICHEN_QUIZ;
              }
            }
            break;

          case MODE_FLAG_QUIZ:
            {
              int choice = flagQuiz.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zum Quiz-Menue (oder Game Over bestaetigt)
                Serial.println("Flag Quiz ended");
                int medal = flagQuiz.getHighestMedal();
                int correct = flagQuiz.getCorrectCount();
                Serial.printf("Result: %d correct, medal=%d\n", correct, medal);

                // Medaille im Profil speichern
                if (medal > 0) {
                  profileManager.saveFlagQuizMedal(currentProfile.id, medal);
                  currentProfile = profileManager.getProfileById(currentProfile.id);
                }

                quizMenu.draw();
                currentMode = MODE_QUIZ_MENU;
              }
              // choice == 1 oder 2: Quiz laeuft weiter (intern gehandhabt)
            }
            break;

          case MODE_CLUB_QUIZ:
            {
              int choice = clubQuiz.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zum Quiz-Menue (oder Game Over bestaetigt)
                Serial.println("Club Quiz ended");
                int medal = clubQuiz.getHighestMedal();
                int correct = clubQuiz.getCorrectCount();
                Serial.printf("Result: %d correct, medal=%d\n", correct, medal);

                // Medaille im Profil speichern
                if (medal > 0) {
                  profileManager.saveClubQuizMedal(currentProfile.id, medal);
                  currentProfile = profileManager.getProfileById(currentProfile.id);
                }

                quizMenu.draw();
                currentMode = MODE_QUIZ_MENU;
              }
              // choice == 1 oder 2: Quiz laeuft weiter (intern gehandhabt)
            }
            break;

          case MODE_STADION_QUIZ:
            {
              int choice = stadionQuiz.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zum Quiz-Menue (oder Game Over bestaetigt)
                Serial.println("Stadion Quiz ended");
                int medal = stadionQuiz.getHighestMedal();
                int correct = stadionQuiz.getCorrectCount();
                Serial.printf("Result: %d correct, medal=%d\n", correct, medal);

                // Medaille im Profil speichern
                if (medal > 0) {
                  profileManager.saveStadionQuizMedal(currentProfile.id, medal);
                  currentProfile = profileManager.getProfileById(currentProfile.id);
                }

                quizMenu.draw();
                currentMode = MODE_QUIZ_MENU;
              }
              // choice == 1 oder 2: Quiz laeuft weiter (intern gehandhabt)
            }
            break;

          case MODE_KENNZEICHEN_QUIZ:
            {
              int choice = kennzeichenQuiz.handleTouch(x, y);
              if (choice == 0) {
                // Zurueck zum Quiz-Menue (oder Game Over bestaetigt)
                Serial.println("Kennzeichen Quiz ended");
                int medal = kennzeichenQuiz.getHighestMedal();
                int correct = kennzeichenQuiz.getCorrectCount();
                Serial.printf("Result: %d correct, medal=%d\n", correct, medal);

                // Medaille im Profil speichern
                if (medal > 0) {
                  profileManager.saveKennzeichenQuizMedal(currentProfile.id, medal);
                  currentProfile = profileManager.getProfileById(currentProfile.id);
                }

                quizMenu.draw();
                currentMode = MODE_QUIZ_MENU;
              }
              // choice == 1 oder 2: Quiz laeuft weiter (intern gehandhabt)
            }
            break;
        }
      }
    }
  }
}
