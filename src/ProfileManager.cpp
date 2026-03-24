#include "ProfileManager.h"
#include <ArduinoJson.h>

#define INDEX_FILE "/profiles/index.json"

ProfileManager::ProfileManager() : _sd(nullptr), _nextId(1) {
}

bool ProfileManager::begin(SDManager* sd) {
  _sd = sd;

  if (!_sd || !_sd->isReady()) {
    Serial.println("ProfileManager: SD not ready!");
    return false;
  }

  // Index laden oder neu erstellen
  if (!loadIndex()) {
    // Neue leere Index-Datei erstellen
    _profiles.clear();
    _nextId = 1;
    saveIndex();
  }

  Serial.printf("ProfileManager: %d profiles loaded, nextId=%d\n", _profiles.size(), _nextId);
  return true;
}

bool ProfileManager::loadIndex() {
  if (!_sd->fileExists(INDEX_FILE)) {
    Serial.println("ProfileManager: index.json not found");
    return false;
  }

  String content = _sd->readFile(INDEX_FILE);
  if (content.length() == 0) {
    Serial.println("ProfileManager: index.json is empty");
    return false;
  }

  // JSON parsen
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, content);
  if (error) {
    Serial.printf("ProfileManager: JSON parse error: %s\n", error.c_str());
    return false;
  }

  // Daten auslesen
  _profiles.clear();
  _nextId = doc["nextId"] | 1;

  JsonArray profiles = doc["profiles"].as<JsonArray>();
  for (JsonVariant v : profiles) {
    int id = v["id"] | 0;
    String name = v["name"] | "";
    if (id > 0 && name.length() > 0) {
      Profile p(id, name);

      // Lieblingsteam DFB laden
      if (v["dfbTeam"].is<JsonObject>()) {
        p.favTeamDFB.name = v["dfbTeam"]["name"] | "";
        p.favTeamDFB.abbrev = v["dfbTeam"]["abbrev"] | "";
        p.favTeamDFB.primaryColor = v["dfbTeam"]["color1"] | 0;
        p.favTeamDFB.secondaryColor = v["dfbTeam"]["color2"] | 0;
      }

      // Lieblingsteam CL laden
      if (v["clTeam"].is<JsonObject>()) {
        p.favTeamCL.name = v["clTeam"]["name"] | "";
        p.favTeamCL.abbrev = v["clTeam"]["abbrev"] | "";
        p.favTeamCL.primaryColor = v["clTeam"]["color1"] | 0;
        p.favTeamCL.secondaryColor = v["clTeam"]["color2"] | 0;
      }

      // Lieblingsteam WM laden
      if (v["wcTeam"].is<JsonObject>()) {
        p.favTeamWC.name = v["wcTeam"]["name"] | "";
        p.favTeamWC.abbrev = v["wcTeam"]["abbrev"] | "";
        p.favTeamWC.primaryColor = v["wcTeam"]["color1"] | 0;
        p.favTeamWC.secondaryColor = v["wcTeam"]["color2"] | 0;
      }

      // Rekordematrix laden
      if (v["records"].is<JsonArray>()) {
        JsonArray records = v["records"].as<JsonArray>();
        int idx = 0;
        for (int t = 0; t < 3 && idx < 9; t++) {
          for (int m = 0; m < 3 && idx < 9; m++) {
            p.records.won[t][m] = records[idx] | false;
            idx++;
          }
        }
      }

      // Quiz-Medaillen laden
      p.flagQuizBestMedal = v["flagMedal"] | 0;
      p.clubQuizBestMedal = v["clubMedal"] | 0;
      p.stadionQuizBestMedal = v["stadionMedal"] | 0;
      p.kennzeichenQuizBestMedal = v["kennzMedal"] | 0;

      // Intro-Status laden
      p.seenIntros = v["seenIntros"] | 0;

      _profiles.push_back(p);
    }
  }

  return true;
}

bool ProfileManager::saveIndex() {
  JsonDocument doc;

  doc["nextId"] = _nextId;

  JsonArray profiles = doc["profiles"].to<JsonArray>();
  for (const Profile& p : _profiles) {
    JsonObject obj = profiles.add<JsonObject>();
    obj["id"] = p.id;
    obj["name"] = p.name;

    // Lieblingsteam DFB speichern
    if (p.favTeamDFB.isSet()) {
      JsonObject dfb = obj["dfbTeam"].to<JsonObject>();
      dfb["name"] = p.favTeamDFB.name;
      dfb["abbrev"] = p.favTeamDFB.abbrev;
      dfb["color1"] = p.favTeamDFB.primaryColor;
      dfb["color2"] = p.favTeamDFB.secondaryColor;
    }

    // Lieblingsteam CL speichern
    if (p.favTeamCL.isSet()) {
      JsonObject cl = obj["clTeam"].to<JsonObject>();
      cl["name"] = p.favTeamCL.name;
      cl["abbrev"] = p.favTeamCL.abbrev;
      cl["color1"] = p.favTeamCL.primaryColor;
      cl["color2"] = p.favTeamCL.secondaryColor;
    }

    // Lieblingsteam WM speichern
    if (p.favTeamWC.isSet()) {
      JsonObject wc = obj["wcTeam"].to<JsonObject>();
      wc["name"] = p.favTeamWC.name;
      wc["abbrev"] = p.favTeamWC.abbrev;
      wc["color1"] = p.favTeamWC.primaryColor;
      wc["color2"] = p.favTeamWC.secondaryColor;
    }

    // Rekordematrix speichern (9 bools als Array)
    if (p.records.countWins() > 0) {
      JsonArray records = obj["records"].to<JsonArray>();
      for (int t = 0; t < 3; t++) {
        for (int m = 0; m < 3; m++) {
          records.add(p.records.won[t][m]);
        }
      }
    }

    // Quiz-Medaillen speichern
    if (p.flagQuizBestMedal > 0) {
      obj["flagMedal"] = p.flagQuizBestMedal;
    }
    if (p.clubQuizBestMedal > 0) {
      obj["clubMedal"] = p.clubQuizBestMedal;
    }
    if (p.stadionQuizBestMedal > 0) {
      obj["stadionMedal"] = p.stadionQuizBestMedal;
    }
    if (p.kennzeichenQuizBestMedal > 0) {
      obj["kennzMedal"] = p.kennzeichenQuizBestMedal;
    }

    // Intro-Status speichern
    if (p.seenIntros > 0) {
      obj["seenIntros"] = p.seenIntros;
    }
  }

  String output;
  serializeJson(doc, output);

  return _sd->writeFile(INDEX_FILE, output.c_str());
}

int ProfileManager::getProfileCount() {
  return _profiles.size();
}

Profile ProfileManager::getProfile(int index) {
  if (index >= 0 && index < (int)_profiles.size()) {
    return _profiles[index];
  }
  return Profile();
}

Profile ProfileManager::getProfileById(int id) {
  for (const Profile& p : _profiles) {
    if (p.id == id) {
      return p;
    }
  }
  return Profile();
}

bool ProfileManager::createProfile(const String& name) {
  if (name.length() < 2 || name.length() > 12) {
    Serial.println("ProfileManager: Name too short or too long");
    return false;
  }

  Profile newProfile(_nextId, name);
  _profiles.push_back(newProfile);
  _nextId++;

  Serial.printf("ProfileManager: Created profile '%s' with id %d\n", name.c_str(), newProfile.id);

  return saveIndex();
}

bool ProfileManager::deleteProfile(int id) {
  for (auto it = _profiles.begin(); it != _profiles.end(); ++it) {
    if (it->id == id) {
      Serial.printf("ProfileManager: Deleting profile '%s' (id %d)\n", it->name.c_str(), id);
      _profiles.erase(it);
      return saveIndex();
    }
  }
  return false;
}

bool ProfileManager::resetAll() {
  Serial.println("ProfileManager: Resetting all profiles");

  // SD formatieren
  if (_sd) {
    _sd->formatCard();
  }

  // Lokale Daten zurücksetzen
  _profiles.clear();
  _nextId = 1;

  return saveIndex();
}

bool ProfileManager::hasProfiles() {
  return !_profiles.empty();
}

bool ProfileManager::setFavoriteTeam(int profileId, TournamentType type, const FavoriteTeam& team) {
  for (auto& p : _profiles) {
    if (p.id == profileId) {
      if (type == TOURNAMENT_DFB_POKAL) {
        p.favTeamDFB = team;
      } else if (type == TOURNAMENT_CHAMPIONS_LEAGUE) {
        p.favTeamCL = team;
      } else if (type == TOURNAMENT_WORLD_CUP_2026) {
        p.favTeamWC = team;
      }
      const char* typeName = (type == TOURNAMENT_DFB_POKAL) ? "DFB" :
                             (type == TOURNAMENT_CHAMPIONS_LEAGUE) ? "CL" : "WM";
      Serial.printf("ProfileManager: Set favorite team '%s' for profile %d (%s)\n",
        team.name.c_str(), profileId, typeName);
      return saveIndex();
    }
  }
  return false;
}

Profile ProfileManager::getFirstProfile() {
  if (!_profiles.empty()) {
    return _profiles[0];
  }
  return Profile();
}

// ========== Turnier-Speicherstände ==========

String ProfileManager::getTournamentSavePath(int profileId, TournamentType type) {
  String filename;
  if (type == TOURNAMENT_DFB_POKAL) {
    filename = "dfb_";
  } else if (type == TOURNAMENT_CHAMPIONS_LEAGUE) {
    filename = "cl_";
  } else {
    filename = "wc_";
  }
  filename += String(profileId);
  filename += ".json";
  return "/profiles/saves/" + filename;
}

bool ProfileManager::hasTournamentSave(int profileId, TournamentType type) {
  if (!_sd || !_sd->isReady()) return false;
  return _sd->fileExists(getTournamentSavePath(profileId, type).c_str());
}

TournamentSave ProfileManager::loadTournamentSave(int profileId, TournamentType type) {
  TournamentSave save;

  if (!_sd || !_sd->isReady()) return save;

  String path = getTournamentSavePath(profileId, type);
  if (!_sd->fileExists(path.c_str())) return save;

  String content = _sd->readFile(path.c_str());
  if (content.length() == 0) return save;

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, content);
  if (error) {
    Serial.printf("TournamentSave: JSON parse error: %s\n", error.c_str());
    return save;
  }

  save.type = type;
  save.mode = static_cast<MatheMode>(doc["mode"] | 0);
  save.round = doc["round"] | 0;
  save.opponentName = doc["opponent"] | "";
  save.opponentAbbrev = doc["abbrev"] | "";
  save.opponentColor = doc["color"] | 0xFFFF;

  Serial.printf("TournamentSave: Loaded %s round %d vs %s (%s)\n",
    type == TOURNAMENT_DFB_POKAL ? "DFB" : "CL",
    save.round, save.opponentName.c_str(), save.opponentAbbrev.c_str());

  return save;
}

bool ProfileManager::saveTournament(int profileId, const TournamentSave& save) {
  if (!_sd || !_sd->isReady()) return false;

  // Saves-Verzeichnis erstellen falls nötig
  _sd->createDir("/profiles/saves");

  JsonDocument doc;
  doc["mode"] = static_cast<int>(save.mode);
  doc["round"] = save.round;
  doc["opponent"] = save.opponentName;
  doc["abbrev"] = save.opponentAbbrev;
  doc["color"] = save.opponentColor;

  String output;
  serializeJson(doc, output);

  String path = getTournamentSavePath(profileId, save.type);
  Serial.printf("TournamentSave: Saving %s round %d vs %s (%s) to %s\n",
    save.type == TOURNAMENT_DFB_POKAL ? "DFB" : "CL",
    save.round, save.opponentName.c_str(), save.opponentAbbrev.c_str(), path.c_str());

  return _sd->writeFile(path.c_str(), output.c_str());
}

bool ProfileManager::deleteTournamentSave(int profileId, TournamentType type) {
  if (!_sd || !_sd->isReady()) return false;

  String path = getTournamentSavePath(profileId, type);
  Serial.printf("TournamentSave: Deleting %s\n", path.c_str());

  return _sd->deleteFile(path.c_str());
}

// ========== Rekordematrix ==========

bool ProfileManager::recordTournamentWin(int profileId, TournamentType tournamentType, MatheMode matheMode) {
  // Turnier-Index: DFB=0, CL=1, WM=2
  int tournamentIndex = 0;
  if (tournamentType == TOURNAMENT_CHAMPIONS_LEAGUE) {
    tournamentIndex = 1;
  } else if (tournamentType == TOURNAMENT_WORLD_CUP_2026) {
    tournamentIndex = 2;
  }

  // Mathe-Index: +/-=0, 1x1=1, :=2
  int matheIndex = 0;
  if (matheMode == MATHE_MULTIPLY) {
    matheIndex = 1;
  } else if (matheMode == MATHE_DIVIDE) {
    matheIndex = 2;
  }

  for (auto& p : _profiles) {
    if (p.id == profileId) {
      p.records.setWon(tournamentIndex, matheIndex);
      Serial.printf("ProfileManager: Recorded win for profile %d - Tournament %d, Mathe %d\n",
        profileId, tournamentIndex, matheIndex);
      return saveIndex();
    }
  }
  return false;
}

RecordsMatrix ProfileManager::getRecords(int profileId) {
  for (const Profile& p : _profiles) {
    if (p.id == profileId) {
      return p.records;
    }
  }
  return RecordsMatrix();
}

// ========== Quiz-Medaillen ==========

bool ProfileManager::saveFlagQuizMedal(int profileId, int medal) {
  for (auto& p : _profiles) {
    if (p.id == profileId) {
      // Nur speichern wenn neue Medaille besser ist
      if (medal > p.flagQuizBestMedal) {
        p.flagQuizBestMedal = medal;
        Serial.printf("ProfileManager: Saved flag quiz medal %d for profile %d\n",
          medal, profileId);
        return saveIndex();
      }
      return true;  // Keine Änderung nötig
    }
  }
  return false;
}

bool ProfileManager::saveClubQuizMedal(int profileId, int medal) {
  for (auto& p : _profiles) {
    if (p.id == profileId) {
      // Nur speichern wenn neue Medaille besser ist
      if (medal > p.clubQuizBestMedal) {
        p.clubQuizBestMedal = medal;
        Serial.printf("ProfileManager: Saved club quiz medal %d for profile %d\n",
          medal, profileId);
        return saveIndex();
      }
      return true;  // Keine Änderung nötig
    }
  }
  return false;
}

bool ProfileManager::saveStadionQuizMedal(int profileId, int medal) {
  for (auto& p : _profiles) {
    if (p.id == profileId) {
      // Nur speichern wenn neue Medaille besser ist
      if (medal > p.stadionQuizBestMedal) {
        p.stadionQuizBestMedal = medal;
        Serial.printf("ProfileManager: Saved stadion quiz medal %d for profile %d\n",
          medal, profileId);
        return saveIndex();
      }
      return true;  // Keine Änderung nötig
    }
  }
  return false;
}

bool ProfileManager::saveKennzeichenQuizMedal(int profileId, int medal) {
  for (auto& p : _profiles) {
    if (p.id == profileId) {
      // Nur speichern wenn neue Medaille besser ist
      if (medal > p.kennzeichenQuizBestMedal) {
        p.kennzeichenQuizBestMedal = medal;
        Serial.printf("ProfileManager: Saved kennzeichen quiz medal %d for profile %d\n",
          medal, profileId);
        return saveIndex();
      }
      return true;  // Keine Änderung nötig
    }
  }
  return false;
}

// ========== WM 2026 Speicherstände ==========

String getWorldCupSavePath(int profileId) {
  return "/profiles/saves/wc_" + String(profileId) + ".json";
}

bool ProfileManager::hasWorldCupSave(int profileId) {
  if (!_sd || !_sd->isReady()) return false;
  return _sd->fileExists(getWorldCupSavePath(profileId).c_str());
}

WorldCupSave ProfileManager::loadWorldCupSave(int profileId) {
  WorldCupSave save;

  if (!_sd || !_sd->isReady()) return save;

  String path = getWorldCupSavePath(profileId);
  if (!_sd->fileExists(path.c_str())) return save;

  String content = _sd->readFile(path.c_str());
  if (content.length() == 0) return save;

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, content);
  if (error) {
    Serial.printf("WorldCupSave: JSON parse error: %s\n", error.c_str());
    return save;
  }

  // Grunddaten laden
  save.mode = static_cast<MatheMode>(doc["mode"] | 0);
  save.playerTeamIndex = doc["playerTeam"] | 0;
  save.playerGroupIndex = doc["playerGroup"] | 0;
  save.groupMatchIndex = doc["groupMatch"] | 0;
  save.groupPhaseComplete = doc["groupComplete"] | false;
  save.showGroupTable = doc["showTable"] | false;
  save.knockoutRound = doc["koRound"] | -1;
  save.knockoutMatchIndex = doc["koMatch"] | 0;
  save.knockoutOpponentIndex = doc["koOpponent"] | -1;
  save.isSpectator = doc["spectator"] | false;

  // Alle Gruppen laden
  JsonArray groups = doc["groups"].as<JsonArray>();
  int g = 0;
  for (JsonVariant grp : groups) {
    if (g >= 12) break;
    save.allGroups[g].complete = grp["complete"] | false;
    JsonArray results = grp["results"].as<JsonArray>();
    int r = 0;
    for (JsonVariant res : results) {
      if (r >= 6) break;
      save.allGroups[g].results[r].team1Goals = res["g1"] | 0;
      save.allGroups[g].results[r].team2Goals = res["g2"] | 0;
      save.allGroups[g].results[r].played = res["p"] | false;
      r++;
    }
    g++;
  }

  // Bracket laden
  JsonArray bracket = doc["bracket"].as<JsonArray>();
  int b = 0;
  for (JsonVariant t : bracket) {
    if (b >= 32) break;
    save.bracketTeams[b] = t.as<int>();
    b++;
  }

  JsonArray winners = doc["winners"].as<JsonArray>();
  int w = 0;
  for (JsonVariant t : winners) {
    if (w >= 31) break;
    save.bracketWinners[w] = t.as<int>();
    w++;
  }

  JsonArray scores = doc["scores"].as<JsonArray>();
  int s = 0;
  for (JsonVariant sc : scores) {
    if (s >= 31) break;
    save.bracketScores[s][0] = sc[0] | 0;
    save.bracketScores[s][1] = sc[1] | 0;
    s++;
  }

  Serial.printf("WorldCupSave: Loaded player=%d group=%c round=%d\n",
    save.playerTeamIndex, 'A' + save.playerGroupIndex, save.knockoutRound);

  return save;
}

bool ProfileManager::saveWorldCup(int profileId, const WorldCupSave& save) {
  if (!_sd || !_sd->isReady()) return false;

  // Saves-Verzeichnis erstellen falls nötig
  _sd->createDir("/profiles/saves");

  JsonDocument doc;

  // Grunddaten
  doc["mode"] = static_cast<int>(save.mode);
  doc["playerTeam"] = save.playerTeamIndex;
  doc["playerGroup"] = save.playerGroupIndex;
  doc["groupMatch"] = save.groupMatchIndex;
  doc["groupComplete"] = save.groupPhaseComplete;
  doc["showTable"] = save.showGroupTable;
  doc["koRound"] = save.knockoutRound;
  doc["koMatch"] = save.knockoutMatchIndex;
  doc["koOpponent"] = save.knockoutOpponentIndex;
  doc["spectator"] = save.isSpectator;

  // Alle Gruppen speichern
  JsonArray groups = doc["groups"].to<JsonArray>();
  for (int g = 0; g < 12; g++) {
    JsonObject grp = groups.add<JsonObject>();
    grp["complete"] = save.allGroups[g].complete;
    JsonArray results = grp["results"].to<JsonArray>();
    for (int r = 0; r < 6; r++) {
      JsonObject res = results.add<JsonObject>();
      res["g1"] = save.allGroups[g].results[r].team1Goals;
      res["g2"] = save.allGroups[g].results[r].team2Goals;
      res["p"] = save.allGroups[g].results[r].played;
    }
  }

  // Bracket speichern
  JsonArray bracket = doc["bracket"].to<JsonArray>();
  for (int i = 0; i < 32; i++) {
    bracket.add(save.bracketTeams[i]);
  }

  JsonArray winners = doc["winners"].to<JsonArray>();
  for (int i = 0; i < 31; i++) {
    winners.add(save.bracketWinners[i]);
  }

  JsonArray scores = doc["scores"].to<JsonArray>();
  for (int i = 0; i < 31; i++) {
    JsonArray sc = scores.add<JsonArray>();
    sc.add(save.bracketScores[i][0]);
    sc.add(save.bracketScores[i][1]);
  }

  String output;
  serializeJson(doc, output);

  String path = getWorldCupSavePath(profileId);
  Serial.printf("WorldCupSave: Saving to %s\n", path.c_str());

  return _sd->writeFile(path.c_str(), output.c_str());
}

bool ProfileManager::deleteWorldCupSave(int profileId) {
  if (!_sd || !_sd->isReady()) return false;

  String path = getWorldCupSavePath(profileId);
  Serial.printf("WorldCupSave: Deleting %s\n", path.c_str());

  return _sd->deleteFile(path.c_str());
}

// ========== Intro-Status ==========

bool ProfileManager::hasSeenIntro(int profileId, uint16_t introType) {
  for (const Profile& p : _profiles) {
    if (p.id == profileId) {
      return (p.seenIntros & introType) != 0;
    }
  }
  return false;
}

bool ProfileManager::setIntroSeen(int profileId, uint16_t introType) {
  for (auto& p : _profiles) {
    if (p.id == profileId) {
      if ((p.seenIntros & introType) == 0) {
        p.seenIntros |= introType;
        Serial.printf("ProfileManager: Set intro %d seen for profile %d\n",
          introType, profileId);
        return saveIndex();
      }
      return true;  // Schon gesehen
    }
  }
  return false;
}
