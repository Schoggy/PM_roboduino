#include "robo_custom.h"
#include "robo_main.h"
#include "robo_pins.h"
#include "transmit.h"

#define STACK_LENGTH 64

/*********************************
 *    Global Variables           *
 *********************************/

// function call array

RCDATA *call_stack_top;
RCDATA *exec_ptr;
RCDATA *rcv_ptr;

// current status
int motor_speed_r, motor_speed_l;
bool leds_front;
bool leds_status;

// function array: all functions have the return type of void and a char pointer
// as the parameter

typedef void (*fptr_arr)(char *);
fptr_arr functions[256] = {NULL};

/*********************************
 *    Function Prototypes        *
 *********************************/

void setup(void);
void loop(void);
void decode_received_data(void);
void set_motors(void);
void set_leds(void);
void send_data(char *data);
void receive_data(void);
void update_leds(RCDATA *rd);
void update_motor_steering(RCDATA *rd);

/*********************************
 *    Main Arduino Functions     *
 *********************************/

void setup() { // the Arduino will run this function once
               // when it is initially powered on

  init_pins();

  call_stack_top = (RCDATA *)calloc(STACK_LENGTH, sizeof(RCDATA));

  exec_ptr = rcv_ptr = call_stack_top;

  leds_front = false;
  leds_status = true;

  // setup for automatic functions (function pointer array)
  // syntax: functions[function index (0 to 255)] = &function_name;
  // the first byte send to the robot is the function index.
  // the next 15 bytes are data for the function.

  functions[0] = &set_leds;
  functions[1] = &update_motor_steering;

  functions[FNR_TEST] = &led_test;
}

void loop() { // the Arduino will loop this function forever
  decode_received_data();
  set_motors();
  set_leds();
}

/*********************************
 *    Functions                  *
 *********************************/

void decode_received_data() { // read and decode data packets sent to the robot
  while (rcv_ptr != exec_ptr) {

    functions[exec_ptr->funcnr](exec_ptr);
    exec_ptr =
        call_stack_top + (((exec_ptr - call_stack_top) + 1) % STACK_LENGTH);
  }
}

void send_data(char *data) {
  // send data back to the controller
}

void set_leds() {
  // set the state of all leds on the robot

  digitalWrite(LED_STATUS, leds_status);
  digitalWrite(LED_FRONT, leds_front);
}

/*********************************
 *    Interrupt Service Routines *
 *********************************/

void receive_data() {
  RCDATA data * = rcv_ptr;
  // add received data to the new struct

  // advance
  rcv_ptr = call_stack_top + (((rcv_ptr - call_stack_top) + 1) % STACK_LENGTH);
}

/*********************************
 *    Automatic Functions        *
 *********************************/

/* these functions need some setting up, but can then be called automatically,
 * when the robot receives a certain data packet.
 * all automatic functions must have the return type void and a single char* as
 * their parameter
 * when called they will be given the address of the data packet that called
 * them in the char*,
 * so they can access the data that called them.
 */

void update_leds(RCDATA *rd) {
  // update the state of the GPIO pins controlling the LEDS of the robot
}

void update_motor_steering(RCDATA *rd) {

  short motor_l = 0, motor_r = 0;

  motor_l = *rd->data[0];
  motor_l = motor_l << 8;
  motor_l = *rd->data[1];

  // forward

  if (motor_l > 0 || motor_r > 0) {
    digitalWrite(MOTOR_L_BACKWARD, LOW);
    digitalWrite(MOTOR_R_BACKWARD, LOW);

    // Kompensation Fehlertoleranz Joystick
    if (motor_l > -2 && motor_r > -2) {
      digitalWrite(MOTOR_L_FORWARD, LOW);
      digitalWrite(MOTOR_R_FORWARD, LOW);
      digitalWrite(MOTOR_L_BACKWARD, LOW);
      digitalWrite(MOTOR_R_BACKWARD, LOW);
    }

    analogWrite(MOTOR_L_FOWARD, motor_l);
    analogWrite(MOTOR_R_FOWARD, motor_r);
  }
  // backward
  else {
    digitalWrite(MOTOR_L_FOWARD, LOW);
    digitalWrite(MOTOR_R_FOWARD, LOW);

    // Kompensation Fehlertoleranz Joystick
    if (motor_l > -2 && motor_r > -2) {
      digitalWrite(MOTOR_L_FORWARD, LOW);
      digitalWrite(MOTOR_R_FORWARD, LOW);
      digitalWrite(MOTOR_L_BACKWARD, LOW);
      digitalWrite(MOTOR_R_BACKWARD, LOW);
    }

    analogWrite(MOTOR_L_BACKWARD, (motor_l * (-1)));
    analogWrite(MOTOR_R_BACKWARD, (motor_r * (-1)));
  }
}

void led_test(RDATA *rd) {}
