#ifndef SRC_PROFILEMANAGER_H
#define SRC_PROFILEMANAGER_H

#include <Arduino.h>
#include <vector>
#include "SDManager.h"
#include "Profile.h"

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

  private:
    SDManager* _sd;
    std::vector<Profile> _profiles;
    int _nextId;

    bool loadIndex();
    bool saveIndex();
};

#endif
