//Defines for pins go here

#define LED_STATUS                  13
#define LED_FRONT                   22


void init_pins(void){ // initialisation for pins; called from setup()
  pinMode(LED_STATUS, OUTPUT);
  pinMode(LED_FRONT, OUTPUT);
  
}
