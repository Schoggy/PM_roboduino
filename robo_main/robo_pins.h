// Defines for pins go here

#define LED_STATUS 13
#define LED_FRONT 22
#define MOTOR_L_FOWARD 11
#define MOTOR_L_BACKWARD 10
#define MOTOR_R_FORWARD 9
#define MOTOR_R_BACKWARD 8

void init_pins(void) { // initialisation for pins; called from setup()
  pinMode(LED_STATUS, OUTPUT);
  pinMode(LED_FRONT, OUTPUT);
  pinMode(MOTOR_L_FORWARD, OUTPUT);
  pinMode(MOTOR_L_BACKWARD, OUTPUT);
  pinMode(MOTOR_R_FORWARD, OUTPUT);
  pinMode(MOTOR_R_BACKWARD, OUTPUT);
}
