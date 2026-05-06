#include "Connection.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE client...");
  Connection_init();
}

void loop() {
  Connection_update();
  delay(100);
}