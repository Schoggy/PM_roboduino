#include "robo_custom.h"
#include "robo_pins.h"
#include "transmit.h"

// This defines the amount of data packets that can be buffered by the robot
#define STACK_LENGTH 32

typedef data_packet RCDATA;

/*********************************
 *    Global Variables           *
 *********************************/

// Function call buffer pointers

RCDATA *call_stack_top;
RCDATA *exec_ptr;
RCDATA *rcv_ptr;

RCDATA *t_send_arr;


int t_send_nr;

// Function array: All functions have the return type of void and a RCDATA*
// as the parameter

typedef void (*fptr_arr)(RCDATA *);
fptr_arr functions[256] = {NULL};

/*********************************
 *    Function Prototypes        *
 *********************************/

void setup(void);
void loop(void);
void decode_received_data(void);
void send_data(void);
void receive_data(void);
void update_leds(RCDATA *rd);
void update_motor_steering(RCDATA *rd);

/*********************************
 *    Main Arduino Functions     *
 *********************************/

void setup() { // The Arduino will run this function once
               // when it is initially powered on.

  init_pins();

  call_stack_top = (RCDATA *)calloc(STACK_LENGTH, sizeof(RCDATA));

  exec_ptr = rcv_ptr = call_stack_top;

  /* Setup for automatic functions (function pointer array)
   * Syntax: functions[function index (0 to 255)] = &function_name;
   * The first byte send to the robot is the function index number 
   * (contained in the variable funcnr in the struct)
   * The next 7 bytes are data for the function.
   */

  t_send_nr = 0;

  functions[FNCNR_LED] = update_leds;
  functions[FNCNR_MOTORS] = update_motor_steering;
  
  
  
  Serial.begin(9600); // start bluetooth serial

}

void loop() { // the Arduino will loop this function forever
  
  //init_t_send(); // run test functions
  
  decode_received_data();
}

 /* TEST FUNCTIONS */
 
int timer;
void tick(){
  while(1){
    for(timer = 0; timer < 50; timer++){}
    t_send(5);
    for(timer = 0; timer < 10; timer++){}
    decode_received_data();
    for(timer = 0; timer < 500; timer++){}
  }
}

void t_send(int nr){
  for(int n = 0; n < nr; n++){
    t_receive_data(t_send_arr[t_send_nr + n]);
  }
}

void init_t_send(){
  t_send_arr = (RCDATA*) calloc(256, sizeof(RCDATA));
  RCDATA* arr_ptr = t_send_arr;
  for(char n = 0; n < 255; n++){
    RCDATA rc;
    rc.funcnr = 0;
    rc.data[0] = 0b00000010;
    rc.data[1] = n;
    rc.data[2] = 255 - n;
    rc.data[3] = 0;
  }
  
}

/*********************************
 *    Functions                  *
 *********************************/

void decode_received_data() { 
  /* This function reads and decodes data packets sent to the robot.
   * There are two pointers, rcv_ptr, pointing to the last packet received, and exec_ptr, pointing to the last packet read.
   * This function keeps reading data packets until the last packet read is the last packet received.
   */
   

   
  
  while (rcv_ptr != exec_ptr) {
    functions[exec_ptr->funcnr](exec_ptr);
    exec_ptr =
        call_stack_top + (((exec_ptr - call_stack_top) + 1) % STACK_LENGTH);
  }
}

void send_data() {
  // send data back to the controller
}


/*********************************
 *    Interrupt Service Routines *
 *********************************/

void receive_data() {
  /* This function reads a data packet from the bbuffer of the bluetooth module into the call stack 
   * and advances the rcv_ptr by one.
   */
   
   
  if(Serial.available() == sizeof(RCDATA))
  {
    for(int i = 0; i < sizeof(RCDATA); i++)
    {
       *((byte*)rcv_ptr + i) = Serial.read();
    }
  }
   
   
   
   
  RCDATA* data = rcv_ptr;
  // add received data to the new struct

  // advance
  rcv_ptr = call_stack_top + (((rcv_ptr - call_stack_top) + 1) % STACK_LENGTH);
}

void t_receive_data(RCDATA rc) {
  /* This function reads a data packet from the bbuffer of the bluetooth module into the call stack 
   * and advances the rcv_ptr by one.
   */
  
  free(rcv_ptr);
  rcv_ptr = &rc;
  // add received data to the new struct

  

  // advance
  rcv_ptr = call_stack_top + (((rcv_ptr - call_stack_top) + 1) % STACK_LENGTH);
}

/*********************************
 *    Automatic Functions        *
 *********************************/

/* These functions need some setting up, but can then be called automatically,
 * when the robot receives a certain data packet.
 * All automatic functions must have the return type void and a single RCDATA* as
 * their parameter.
 * When called they will be given the address of the data packet that called
 * them in the RCDATA*, so they can access the data that called them.
 */

void update_leds(RCDATA *rd) {
  /* update the state of the GPIO pins controlling the LEDS of the robot
   * The format of the Data is:
   * Byte 0: "Digital" state of all LEDs, on or off. 
   * Bit 0 is the status LED on the Arduino Mega
   * Bit 1 and 2 are the right and left LEDs in the front or the Roboduino
   * Bit 3 is the blue user LED on the breadboard
   * Byte 1 to 3 are the analog RGB values for left front LED
   * respectively byte 4 to 6 are the RGB values for the roght front LED
   */
   
  if(rd->data[0] % 2){
    digitalWrite(LED_STATUS, HIGH);
  } else {
    digitalWrite(LED_STATUS, LOW);
  }
  if(rd->data[0] & 0b00000010){
    analogWrite(LED_FRONT_RR, rd->data[1]);
    analogWrite(LED_FRONT_RG, rd->data[2]);
    analogWrite(LED_FRONT_RB, rd->data[3]);
  } else {
    analogWrite(LED_FRONT_RR, 0);
    analogWrite(LED_FRONT_RG, 0);
    analogWrite(LED_FRONT_RB, 0);
  }
  if(rd->data[0] & 0b00000100){
    analogWrite(LED_FRONT_LR, rd->data[4]);
    analogWrite(LED_FRONT_LG, rd->data[5]);
    analogWrite(LED_FRONT_LB, rd->data[6]);
  } else {
    analogWrite(LED_FRONT_LR, 0);
    analogWrite(LED_FRONT_LG, 0);
    analogWrite(LED_FRONT_LB, 0);
  }
  if(rd->data[0] & 0b00001000){
    digitalWrite(LED_USER, HIGH);
  } else {
    digitalWrite(LED_USER, LOW);
  }
}

#define MOTOR_VL ((rd->data[0] << 8) | rd->data[1])
#define MOTOR_VR ((rd->data[2] << 8) | rd->data[3])
void update_motor_steering(RCDATA *rd) {
  /* This function updates the pins controlling the two motors.
   * The two values with the motor speeds are given in the form of two short variables.
   * Byte 0 and byte 1 are part of the first short, containing the value for the left motor,
   * with byte 0 as the higher bits of the short.
   * Byte 2 and 3 contain the short for the right motor.
   */

  if (MOTOR_VL > 0) {
    digitalWrite(MOTOR_L_BACKWARD, LOW);
    analogWrite(MOTOR_L_FORWARD, MOTOR_VL);
  } else {
    digitalWrite(MOTOR_L_FORWARD, LOW);
    analogWrite(MOTOR_L_BACKWARD, MOTOR_VL);
  }
  
    if (MOTOR_VR > 0) {
    digitalWrite(MOTOR_R_BACKWARD, LOW);
    analogWrite(MOTOR_R_FORWARD, MOTOR_VR);
  } else {
    digitalWrite(MOTOR_R_FORWARD, LOW);
    analogWrite(MOTOR_R_BACKWARD, MOTOR_VR);
  }
}
