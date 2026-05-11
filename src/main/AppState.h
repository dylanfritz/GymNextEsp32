#pragma once
#include <string>

struct AppState{
  std::string current_msg = "Hello_World";
  bool manual_override = false;
  bool red_only = false;
};