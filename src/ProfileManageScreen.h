#ifndef SRC_PROFILEMANAGESCREEN_H
#define SRC_PROFILEMANAGESCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include "ProfileManager.h"

#define MANAGE_VISIBLE_PROFILES 3

class ProfileManageScreen {
  public:
    ProfileManageScreen();

    void init();
    void setProfileManager(ProfileManager* pm);
    void draw();

    // handleTouch gibt zurück:
    // >0: Profil-ID zum Löschen ausgewählt
    // -1: "Reset" gedrückt
    // -2: "Zurück" gedrückt
    // 0: nichts
    int handleTouch(int16_t x, int16_t y);

  private:
    TFT_eSPI tft;

    ProfileManager* _profileManager;
    Button* profileButtons[MANAGE_VISIBLE_PROFILES];
    Button* btnReset;
    Button* btnBack;
    Button* btnScrollUp;
    Button* btnScrollDown;

    int _scrollOffset;
    int _visibleCount;

    void updateButtons();
};

#endif
