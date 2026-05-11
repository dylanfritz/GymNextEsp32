#include "Connection.h"
#include "SerialInterface.h"
#include "Animation.h"
#include <string>

bool manual_override = false;
std::string current_msg = "Hello_World";

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE client...");
  Connection_init();
  SerialInterface_init(&current_msg, &manual_override); // link the global variables to be changed by the interface handler
}

void loop() {
  SerialInterface_update();
  if (queueEmpty()&&Connection_isConnected()&&!manual_override) {
    scrollText(current_msg, 200);
  }
  Connection_update();
}