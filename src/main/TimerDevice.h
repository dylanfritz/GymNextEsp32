#pragma once
#include <string>

void timerMessage(const std::string &txt, unsigned long ms);

void timerSetWelcomeMsg(const std::string &txt);

void timerWelcomeMode();

void timerMessageMode();

void timerTimerMode();

void timerClockMode();

void timerFlashText(const std::string &txt, unsigned int times);

void timerRestart();

void timerRawCommand(const std::string &txt, unsigned long ms = 0);

//void timerSetTime(int utc);



