#include "SerialInterface.h"
#include "Connection.h"
#include "Animation.h"
#include "TimerDevice.h"
#include <unordered_map>
#include <ArduinoJson.h>

using CommandHandler = void(*)(const JsonDocument&); //Alias functionpointer for command handler types

static std::unordered_map<std::string, CommandHandler> dispatch;

static std::string* current_msg;
static bool* manual_override;
static bool change_msg = false;

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

void SerialInterface_init(std::string* msg, bool* m_override){
  current_msg = msg;
  manual_override = m_override;

  dispatch["SCROLL"] = scrollHandler;
  dispatch["ADMIN"]  = adminHandler;
  dispatch["RELEASE"] = releaseHandler;
  dispatch["CHANGE_MSG"] = changeMessageHandler;
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