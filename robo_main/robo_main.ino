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

// Store RGB-LED state
char rgb_data[6];

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

  init_pins(); // Set the pinmode for every pin

  // allocate memory for the local buffer of commands from the remote
  call_stack_top = (RCDATA *)calloc(STACK_LENGTH, sizeof(RCDATA));

  exec_ptr = rcv_ptr = call_stack_top;

  /* Setup for automatic functions (function pointer array)
   * Syntax: functions[function index (0 to 255)] = &function_name;
   * The first byte send to the robot is the function index number
   * (contained in the variable funcnr in the struct)
   * The next 7 bytes are data for the function.
   */

  functions[FNCNR_LED] = update_leds;
  functions[FNCNR_MOTORS] = update_motor_steering;

  Serial.begin(9600);  // Start debug serial inteface
  Serial1.begin(9600); // Start bluetooth serial1, communtcation with the remote
}

void loop() {     // the Arduino will loop this function forever
  receive_data(); // Wait for data from the remote
  decode_received_data();
  update_rgb_leds();
}

/*********************************
 *    Functions                  *
 *********************************/

void decode_received_data() {
  /* This function reads and decodes data packets sent to the robot.
   * There are two pointers, rcv_ptr, pointing to the last packet received,
   * and exec_ptr, pointing to the last packet read.
   * This function keeps reading data packets until the last packet read
   * is the last packet received.
   */

  while (rcv_ptr != exec_ptr) {
    functions[exec_ptr->funcnr](exec_ptr);
    exec_ptr =
        call_stack_top + (((exec_ptr - call_stack_top) + 1) % STACK_LENGTH);
  }
}

void send_data() {
  // Send data back to the controller
  // - placeholder -
}

void update_rgb_leds() {
  // Write the state stored in rgb_data to the RGB-LEDS
  analogWrite(LED_FRONT_RR, rgb_data[0]);
  analogWrite(LED_FRONT_RG, rgb_data[1]);
  analogWrite(LED_FRONT_RB, rgb_data[2]);
  analogWrite(LED_FRONT_LR, rgb_data[3]);
  analogWrite(LED_FRONT_LG, rgb_data[4]);
  analogWrite(LED_FRONT_LB, rgb_data[5]);
}

void receive_data() {
  /* This function reads all data packets from the buffer of the bluetooth
   * module
   * into the call stack and advances the rcv_ptr by one.
   */

  while (!Serial1.available()) {
  } // Wait until data arrives

  // Store all data from the buffer of the bluetooth module in the local buffer
  while (Serial1.available() >= sizeof(RCDATA)) {

    for (int i = 0; i < sizeof(RCDATA); i++) { // Write every byte
      *((byte *)rcv_ptr + i) = Serial1.read();
    }

    // Advance the receive pointer by the size of one struct
    rcv_ptr =
        call_stack_top + (((rcv_ptr - call_stack_top) + 1) % STACK_LENGTH);
  }
}

void s_receive_data(RCDATA *rc) {
  /* This function emulates reading a data packet from the buffer of the
   * bluetooth module into
   * the call stack and advances the rcv_ptr by one.
   */

  rcv_ptr->funcnr = rc->funcnr;
  memcpy(rcv_ptr->data, rc->data, 7);

  // Advance the receive pointer by the size of one struct
  rcv_ptr = call_stack_top + (((rcv_ptr - call_stack_top) + 1) % STACK_LENGTH);
}

/*********************************
 *    Automatic Functions        *
 *********************************/

/* These functions need some setting up, but can then be called automatically,
 * when the robot receives a certain data packet.
 * All automatic functions must have the return type void and a single RCDATA*
 * as
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

  if (rd->data[0] % 2) {
    digitalWrite(LED_STATUS, HIGH);
  } else {
    digitalWrite(LED_STATUS, LOW);
  }
  if (rd->data[0] & 0b00000010) {
    rgb_data[0] = rd->data[1];
    rgb_data[1] = rd->data[2];
    rgb_data[2] = rd->data[3];
  } else {
    rgb_data[0] = 0;
    rgb_data[1] = 0;
    rgb_data[2] = 0;
  }
  if (rd->data[0] & 0b00000100) {
    rgb_data[3] = rd->data[4];
    rgb_data[4] = rd->data[5];
    rgb_data[5] = rd->data[6];
  } else {
    rgb_data[3] = 0;
    rgb_data[4] = 0;
    rgb_data[5] = 0;
  }
  if (rd->data[0] & 0b00001000) {
    digitalWrite(LED_USER, HIGH);
  } else {
    digitalWrite(LED_USER, LOW);
  }
}

void update_motor_steering(RCDATA *rd) {
  /* This function updates the pins controlling the two motors.
   * The state of the two motors (forward or reverse) are given in byte 0 and byte 2.
   * The byte is 0 for reverse and 1 for forward
   * The values for the motor speed are given in byte 1 and byte 3. 
   */
   
  if (rd->data[0]) {
    digitalWrite(MOTOR_L_BACKWARD, LOW);
    analogWrite(MOTOR_L_FORWARD, rd->data[1]);
  } else {
    digitalWrite(MOTOR_L_FORWARD, LOW);
    analogWrite(MOTOR_L_BACKWARD, rd->data[1]);
  }

  if (rd->data[2]) {
    digitalWrite(MOTOR_R_BACKWARD, LOW);
    analogWrite(MOTOR_R_FORWARD, rd->data[3]);
  } else {
    digitalWrite(MOTOR_R_FORWARD, LOW);
    analogWrite(MOTOR_R_BACKWARD, rd->data[3]);
  }
}
