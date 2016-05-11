#include "robo_main.h"
#include "robo_custom.h"

/********************************
*    Main Arduino Functions     *
********************************/

void setup() {
  recieved_data = (int*) calloc(32, sizeof(int));
}

void loop() {
  decode_recieved_data();
  set_motors();
  
}

/********************************
*    Functions                  *
********************************/

void decode_recieved_data(){
  
}

void set_motors(int val_l, int val_r){
  
}

void set_leds(unsigned int val){
  
}















