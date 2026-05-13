#include <string>
#include "Connection.h"
#include "TimerDevice.h"
#include "AnimTypes.h"

std::string scrollGenerator(AnimState& state){

}

void scrollText(std::string inp, unsigned long scroll_del, bool red_only) {
    int width = red_only ? 4 : 6;
    std::string pad(width, ' ');
    std::string padded_text = pad + inp + pad;
    for (int i = 0; i < (int)padded_text.length() - width; i++) {
        std::string frame = red_only ? "  " : "";
        frame += padded_text.substr(i, width);
        Connection_enqueue("XM?" + frame, scroll_del);
    }
}

void scrollTextRev(std::string inp, unsigned long scroll_del, bool red_only) {
    int width = red_only ? 4 : 6;
    std::string pad(width, ' ');
    std::string padded_text = pad + inp + pad;
    for (int i = (int)padded_text.length()-width; i >= 0; i--) {
        std::string frame = red_only ? "  " : "";
        frame += padded_text.substr(i, width);
        Connection_enqueue("XM?" + frame, scroll_del);
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