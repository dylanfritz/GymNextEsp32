#include "TimerDevice.h"
#include <string>
#include "Connection.h"

void timerMessage(const std::string &txt, unsigned long ms){
  Connection_enqueue("XM?" + txt, ms);
}

void timerSetWelcomeMsg(const std::string &txt, unsigned long ms){
  Connection_enqueue("WM?" + txt, ms);
}

void timerWelcomeMode(){
  Connection_enqueue("WE");
}

void timerMessageMode(){
  Connection_enqueue("ME");
}

void timerTimerMode(){
  Connection_enqueue("TI");
}

void timerClockMode(){
  Connection_enqueue("CL");
}

void timerFlashText(const std::string &txt, unsigned int times){
  Connection_enqueue("FM?" + std::to_string(times) + "," + txt);
}

void timerRestart(bool disconnect){
  Connection_enqueue("P2", 0, disconnect ? connDisconnect : nullptr);
}

void timerRawCommand(const std::string &txt, unsigned long ms){
  Connection_enqueue(txt, ms);
}

void timerConLightOn(){
  Connection_enqueue("I1");
}

void timerConLightOff(){
  Connection_enqueue("I0");
}

//void timerSetTime(int utc);