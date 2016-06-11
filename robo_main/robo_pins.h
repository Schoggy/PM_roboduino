// Defines for pins go here

#define LED_STATUS 13
#define LED_FRONT_LR 2
#define LED_FRONT_LG 3
#define LED_FRONT_LB 4
#define LED_FRONT_RR 5
#define LED_FRONT_RG 6
#define LED_FRONT_RB 7
#define LED_USER 22
#define MOTOR_L_FOWARD 11
#define MOTOR_L_BACKWARD 10
#define MOTOR_R_FORWARD 9
#define MOTOR_R_BACKWARD 8

void init_pins(void) { // initialisation for pins; called from setup()
  pinMode(LED_STATUS, OUTPUT);
  pinMode(LED_FRONT_LR, OUTPUT);
  pinMode(LED_FRONT_LG, OUTPUT);
  pinMode(LED_FRONT_LB, OUTPUT);
  
  pinMode(LED_FRONT_RR, OUTPUT);
  pinMode(LED_FRONT_RG, OUTPUT);
  pinMode(LED_FRONT_RB, OUTPUT);
  
  pinMode(LED_USER, OUTPUT);
  pinMode(MOTOR_L_FORWARD, OUTPUT);
  pinMode(MOTOR_L_BACKWARD, OUTPUT);
  pinMode(MOTOR_R_FORWARD, OUTPUT);
  pinMode(MOTOR_R_BACKWARD, OUTPUT);
}
