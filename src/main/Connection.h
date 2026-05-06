#pragma once
#include "BLEDevice.h"
#include <string>

// Call once in setup()
void Connection_init();

// Call every loop() iteration
void Connection_update();

void Connection_enqueue(std::string payload, unsigned long delayMs = 0);

bool Connection_isConnected();
bool queueEmpty();