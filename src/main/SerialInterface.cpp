#include "SerialInterface.h"
#include "Connection.h"
#include "Animation.h"
#include <ArduinoJson.h>

using CommandHandler = void(*)(const JsonDocument&); //Alias functionpointer for command handler types

static std::string* current_msg;
static bool* manual_override;
static bool change_msg = false;

void adminHandler(const JsonDocument& doc){
  
}

void SerialInterface_init(std::string* msg, bool* m_override){
  current_msg = msg;
  manual_override = m_override;
}

void SerialInterface_update(){
  
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();
    Serial.print("Received: ");
    Serial.println(input);

    if (change_msg) {
      *current_msg = std::string(input.c_str());
      change_msg = false;
      Connection_enqueue("XM?SET", 1000);
      scrollText(*current_msg, 200);
      Connection_enqueue("XM?     _");
    } else if (input == "<CHGMSG>" && *manual_override) {
      Connection_enqueue("XM?CHGMSG", 1000);
      change_msg = true;
    } else if (input == "<INSPQ>" && *manual_override){
      Connection_enqueue("XM? INSPQ", 1000);
      Connection_enqueue("XM?" + std::to_string(queueLen()), 1000);
      Connection_enqueue("XM?     _", 1000);
    } else if (input == "<FORCEADMIN>"){
      *manual_override = true;
      clearQueue();
      Connection_enqueue("XM?ADMIN", 1000);
      Connection_enqueue("XM?     _");
    } else if (input == "<ADMIN>") {
      *manual_override = true;
      Connection_enqueue("XM?ADMIN", 1000);
      Connection_enqueue("XM?     _");
    } else if (input == "<Release>") {
      Connection_enqueue("ME");
      Connection_enqueue("XM?Releas", 1000);
      *manual_override = false;
    } else {
      Connection_enqueue(std::string(input.c_str()));
    }
  }
}