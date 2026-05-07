#include "Connection.h"
#include <string>

bool manual_override = false;
std::string current_msg = "Hello_World";

void scrollText(std::string inp, unsigned long scroll_del){
  std::string padded_text = "      " + inp + "      ";
  for(int i = 0; i < padded_text.length()-6; i++){
    Connection_enqueue("XM?"+ padded_text.substr(i, 6), scroll_del); 
  }
}

void serialInput(){
  static bool change_msg = false;
  
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();
    Serial.print("Received: ");
    Serial.println(input);

    if (change_msg) {
      current_msg = std::string(input.c_str());
      change_msg = false;
      Connection_enqueue("XM?SET", 1000);
      scrollText(current_msg, 200);
      Connection_enqueue("XM?     _");
    } else if (input == "<CHGMSG>" && manual_override) {
      Connection_enqueue("XM?CHGMSG", 1000);
      change_msg = true;
    } else if (input == "<INSPQ>" && manual_override){
      Connection_enqueue("XM? INSPQ", 1000);
      Connection_enqueue("XM?" + std::to_string(queueLen()), 1000);
      Connection_enqueue("XM?     _", 1000);
    } else if (input == "<FORCEADMIN>"){
      manual_override = true;
      clearQueue();
      Connection_enqueue("XM?ADMIN", 1000);
      Connection_enqueue("XM?     _");
    } else if (input == "<ADMIN>") {
      manual_override = true;
      Connection_enqueue("XM?ADMIN", 1000);
      Connection_enqueue("XM?     _");
    } else if (input == "<Release>") {
      Connection_enqueue("ME");
      Connection_enqueue("XM?Releas", 1000);
      manual_override = false;
    } else {
      Connection_enqueue(std::string(input.c_str()));
    }
  }
}



void bounceText(std::string inp, unsigned long scroll_del){
  std::string padded_text = "     " + inp + "     ";
  for(int i = 0; i < padded_text.length()-6; i++){
    Connection_enqueue("XM?"+ padded_text.substr(i, 6), scroll_del); 
  }
  for(int i = padded_text.length()-6; i > 0; i--){// only show the last frame once on the next loop to account for generation delay
    Connection_enqueue("XM?"+ padded_text.substr(i, 6), scroll_del); 
  }
}

void flashText(std::string inp, unsigned long flash_del){
  std::string empty_frame = "      ";
  int num_full_frames = inp.length()/6;
  int chars_in_last_frame = inp.length()%6;
  if (inp.length() >= 6) {
    for (int i = 0; i <= (int)inp.length() - 6; i += 6) {
      Connection_enqueue("XM?" + inp.substr(i, 6), flash_del / 2);
      Connection_enqueue("XM?" + empty_frame, flash_del / 2);
    }
  }
  if (chars_in_last_frame > 0) {
    std::string last_frame = "";
    for (int j = 0; j < 6 - chars_in_last_frame; j++) last_frame += " ";
    last_frame += inp.substr(num_full_frames * 6, chars_in_last_frame);
    Connection_enqueue("XM?" + last_frame, flash_del / 2);
    Connection_enqueue("XM?" + empty_frame, flash_del / 2);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE client...");
  Connection_init();
}

void loop() {
  serialInput();
  if (queueEmpty()&&Connection_isConnected()&&!manual_override) {
    scrollText(current_msg, 200);
  }
  Connection_update();
}