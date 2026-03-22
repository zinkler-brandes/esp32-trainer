#ifndef SRC_PROFILEDETAILSCREEN_H
#define SRC_PROFILEDETAILSCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "Profile.h"

class ProfileDetailScreen {
  public:
    ProfileDetailScreen();

    void init();
    void setProfile(const Profile& profile);
    void draw();

    // handleTouch gibt zurück:
    // 1: "Spielen" gedrückt
    // 2: "Löschen" gedrückt
    // 3: "Zurück" gedrückt
    // 0: nichts
    int handleTouch(int16_t x, int16_t y);

    Profile getProfile();

  private:
    TFT_eSPI tft;
    Profile _profile;

    Button* btnPlay;
    Button* btnDelete;
    Button* btnBack;
};

#endif
