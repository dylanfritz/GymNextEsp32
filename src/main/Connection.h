#pragma once
#include "BLEDevice.h"
#include "AppState.h"
#include <string>

using CommandCallback = void(*)();

// Call once in setup()
void Connection_init(AppState* state);

// Call every loop() iteration
void Connection_update();

void Connection_enqueue(std::string payload, unsigned long delayMs = 0, CommandCallback cb = nullptr);

bool Connection_isConnected();
bool queueEmpty();
void clearQueue();
int queueLen();

void setReconnOverride();
void clearReconnOverride();
void connDisconnect();