#ifndef SRC_PROFILESELECTSCREEN_H
#define SRC_PROFILESELECTSCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "ProfileManager.h"

#define MAX_VISIBLE_PROFILES 3

class ProfileSelectScreen {
  public:
    ProfileSelectScreen();

    void init();
    void setProfileManager(ProfileManager* pm);
    void draw();

    // handleTouch gibt zurück:
    // >0: Profil-ID ausgewählt (direkt spielen!)
    // -1: "Neues Profil" gedrückt
    // -2: "Verwalten" gedrückt
    // 0: nichts
    int handleTouch(int16_t x, int16_t y);

  private:
    TFT_eSPI tft;

    ProfileManager* _profileManager;
    Button* profileButtons[MAX_VISIBLE_PROFILES];
    Button* btnNewProfile;
    Button* btnManage;
    Button* btnScrollUp;
    Button* btnScrollDown;

    int _scrollOffset;
    int _visibleCount;

    void updateButtons();
};

#endif
