#include "Connection.h"
#include "SerialInterface.h"
#include "Animation.h"
#include "AppState.h"
#include <string>

AppState state;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE client...");
  Connection_init(&state);
  SerialInterface_init(&state); // link the global variables to be changed by the interface handler
}

void loop() {
  SerialInterface_update();
  if (queueEmpty()&&Connection_isConnected()&&!state.manual_override) {
    scrollText(state.current_msg, 200, state.red_only);
  }
  Connection_update();
}