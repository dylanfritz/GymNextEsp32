#include "SerialInterface.h"
#include "Connection.h"
#include "Animation.h"
#include "TimerDevice.h"
#include "AppState.h"
#include <unordered_map>
#include <ArduinoJson.h>

using CommandHandler = void(*)(const JsonDocument&); //Alias functionpointer for command handler types

static std::unordered_map<std::string, CommandHandler> dispatch;

static AppState* curr_state;

static std::string* current_msg;
static bool* manual_override;
static bool* red_only;

void adminHandler(const JsonDocument &doc){
  *manual_override = true;
  bool force = doc["force"] | false;
  if(force) clearQueue();
  timerMessageMode();
  timerMessage("ADMIN", 1000);
  timerMessage("     _");
}

void scrollHandler(const JsonDocument &doc){
  return;
}

void releaseHandler(const JsonDocument &doc){
  timerMessageMode();
  timerMessage("releas", 1000);
  *manual_override = false;
}

void changeMessageHandler(const JsonDocument &doc){
  bool force = doc["force"] | false;
  if(force) clearQueue();
  *current_msg = doc["text"] | " ";
}

void toggleBlueHandler(const JsonDocument &doc){
  clearQueue();
  *red_only = !(*red_only);
}

void sleepHandler(const JsonDocument &doc){
  clearQueue();
  *manual_override = true;
  timerSetWelcomeMsg("sleep");
  timerWelcomeMode();
  timerSetWelcomeMsg("      ");
  setReconnOverride();
  timerRestart(true);
}

void sleepWakeHandler(const JsonDocument &doc){
  clearReconnOverride();
}

void disconnectHandler(const JsonDocument &doc){
  *manual_override = true;
  clearQueue();
  setReconnOverride();
  connDisconnect();
}

void reconnectHandler(const JsonDocument &doc){
  clearReconnOverride();
}


void SerialInterface_init(AppState* state){
  curr_state = state;
  current_msg = &(state->current_msg);
  manual_override = &(state->manual_override);
  red_only = &(state->red_only);

  dispatch["SCROLL"] = scrollHandler;
  dispatch["ADMIN"]  = adminHandler;
  dispatch["RELEASE"] = releaseHandler;
  dispatch["CHANGE_MSG"] = changeMessageHandler;
  dispatch["TOGGLE_BLUE"] = toggleBlueHandler;
  dispatch["SLEEP"] = sleepHandler;
  dispatch["SLEEP_WAKE"] = sleepWakeHandler;
  dispatch["DISCONNECT"] = disconnectHandler;
  dispatch["RECONNECT"] = reconnectHandler;
}

void SerialInterface_update(){
  
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    Serial.print("Received: ");
    Serial.println(input);

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, input);

    if (err) {
      timerRawCommand(std::string(input.c_str()));
      return;
    }

    std::string cmd = doc["cmd"] | "";
    auto it = dispatch.find(cmd);
    if (it != dispatch.end()) {
      it->second(doc);  // call the function pointer
    } else {
      Serial.println("Unknown cmd: " + String(cmd.c_str()));
    }
  }
}