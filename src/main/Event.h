#pragma once


enum class EventType {ANIMATION, COMMAND, DISPLAY, OTHER};
enum class EventPriority {PREEMPT, PRIORITY, BASIC, IDLE};

class Event {
  private:
  static unsigned int eidCounter;
  
  public: 
  unsigned int eid; 
  EventType type;
  EventPriority priority;
  bool preemptible;
  bool running;
  int requeueCounter;
  bool requeueIndef;

  Event(EventType t, EventPriority p) : type(t), priority(p), eid(eidCounter++), preemptible(true), running(false), requeueCounter(0), requeueIndef(false) {}

  virtual void update() = 0;
  virtual void promote() = 0; // called by scheduler to start running event
  virtual ~Event() {}

};