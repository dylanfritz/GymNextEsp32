#pragma once
#include "Event.h"
#include "AnimTypes.h"
#include <string>

class AnimEvent : public Event {
  public:
  AnimEvent(EventPriority p, AnimGenerator g, std::string t, unsigned long d) : Event(p, EventType::ANIMATION), _gen(g) {
    _state.text = t;
    _state.frameDelay = d;
  }

  void update() override{

  } 

  void promote() override {

  }

  private:
  AnimState _state;
  AnimGenerator _gen;
};