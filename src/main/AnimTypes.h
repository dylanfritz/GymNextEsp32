#pragma once
#include <string>

struct AnimState {
  std::string text = "";
  std::string currFrame = "";
  unsigned long frameDelay = 0;
  int pos = 0;
  int phase = 0;
  bool done = false;
  bool redOnly = false;
};

using AnimGenerator = std::string(*)(AnimState&);