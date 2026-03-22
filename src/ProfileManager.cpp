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
      _profiles.push_back(Profile(id, name));
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

Profile ProfileManager::getFirstProfile() {
  if (!_profiles.empty()) {
    return _profiles[0];
  }
  return Profile();
}
