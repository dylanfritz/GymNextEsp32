#pragma once
#include <string>

void timerMessage(const std::string &txt, unsigned long ms = 0);

void timerSetWelcomeMsg(const std::string &txt, unsigned long ms = 0);

void timerWelcomeMode();

void timerMessageMode();

void timerTimerMode();

void timerClockMode();

void timerFlashText(const std::string &txt, unsigned int times);

void timerRestart(bool disconnect = false);

void timerRawCommand(const std::string &txt, unsigned long ms = 0);

void timerConLightOn();

void timerConLightOff();

//void timerSetTime(int utc);



