#pragma once
#include "BLEDevice.h"

// Call once in setup()
void Connection_init();

// Call every loop() iteration
void Connection_update();