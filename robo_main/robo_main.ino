#include "robo_custom.h"
#include "robo_main.h"
#include "robo_pins.h"

#define PACKET_SIZE 8                          // number of bytes per packet
#define BUF_SIZE_B (rd_buf_size * PACKET_SIZE) // buffer size in bytes

/*********************************
 *    Global Variables           *
 *********************************/

// data transmission
char *rdata;               // data sent to the robot
unsigned int rdata_pos_r;  // position of the read pointer
unsigned int rdata_pos_w;  // position of the write pointer
unsigned int rd_buf_size;  // size of the received data buffer in packets
unsigned int data_pending; // number of data packets waiting to be decoded

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
void update_leds(char *input);
void update_motor_steering(char *input);

/*********************************
 *    Main Arduino Functions     *
 *********************************/

void setup() { // the Arduino will run this function once
               // when it is initially powered on

  init_pins();

  rd_buf_size = 1024;
  rdata = (char *)calloc(rd_buf_size * PACKET_SIZE, sizeof(char));
  rdata_pos_r = rdata_pos_w = 0;

  motor_speed_r = motor_speed_r = 0;

  leds_front = false;
  leds_status = true;

  // setup for automatic functions (function pointer array)
  // syntax: functions[function index (0 to 255)] = &function_name;
  // the first byte send to the robot is the function index.
  // the next 7 bytes data for the function.

  functions[0] = &set_leds;
  functions[1] = &update_motor_steering;
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
  while (data_pending > 0) {
    functions[*rdata[rdata_pos_r]](rdata[rdata_pos_r]); // call function

    rdata_pos_r = (rdata_pos_r + 8) % BUF_SIZE_B;
    data_pending--;
  }
}

void set_motors() {
  // set the speed of both motors
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
  // write data into buffer
  //*rdata[rdata_pos_w] = received_data;

  // advance write pointer by one byte in the ring buffer
  rdata_pos_w = (rdata_pos_w + 1) % BUF_SIZE_B;
  data_pending++;
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

void update_leds(char *input) {
  // update the state of the GPIO pins controlling the LEDS of the robot
}

void update_motor_steering(char *input) {
  // calculate new values for the motors
}
