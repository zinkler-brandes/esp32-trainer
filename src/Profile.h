#ifndef SRC_PROFILE_H
#define SRC_PROFILE_H

#include <Arduino.h>

struct Profile {
  int id;
  String name;

  Profile() : id(0), name("") {}
  Profile(int _id, const String& _name) : id(_id), name(_name) {}
};

#endif
