#ifndef SRC_PROFILEMANAGER_H
#define SRC_PROFILEMANAGER_H

#include <Arduino.h>
#include <vector>
#include "SDManager.h"
#include "Profile.h"
#include "TournamentSave.h"
#include "WorldCupSave.h"

class ProfileManager {
  public:
    ProfileManager();

    bool begin(SDManager* sd);

    // CRUD
    int getProfileCount();
    Profile getProfile(int index);
    Profile getProfileById(int id);
    bool createProfile(const String& name);
    bool deleteProfile(int id);

    // Reset alle Profile
    bool resetAll();

    // Hilfsfunktionen
    bool hasProfiles();
    Profile getFirstProfile();

    // Lieblingsteam setzen
    bool setFavoriteTeam(int profileId, TournamentType type, const FavoriteTeam& team);

    // Turnier-Speicherstände (DFB/CL)
    bool hasTournamentSave(int profileId, TournamentType type);
    TournamentSave loadTournamentSave(int profileId, TournamentType type);
    bool saveTournament(int profileId, const TournamentSave& save);
    bool deleteTournamentSave(int profileId, TournamentType type);

    // WM 2026 Speicherstände
    bool hasWorldCupSave(int profileId);
    WorldCupSave loadWorldCupSave(int profileId);
    bool saveWorldCup(int profileId, const WorldCupSave& save);
    bool deleteWorldCupSave(int profileId);

    // Rekordematrix
    bool recordTournamentWin(int profileId, TournamentType tournamentType, MatheMode matheMode);
    RecordsMatrix getRecords(int profileId);

    // Quiz-Medaillen
    bool saveFlagQuizMedal(int profileId, int medal);
    bool saveClubQuizMedal(int profileId, int medal);
    bool saveStadionQuizMedal(int profileId, int medal);
    bool saveKennzeichenQuizMedal(int profileId, int medal);

    // Intro-Status
    bool hasSeenIntro(int profileId, uint16_t introType);
    bool setIntroSeen(int profileId, uint16_t introType);

  private:
    SDManager* _sd;
    std::vector<Profile> _profiles;
    int _nextId;

    bool loadIndex();
    bool saveIndex();
    String getTournamentSavePath(int profileId, TournamentType type);
};

#endif
