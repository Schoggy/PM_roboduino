/* Remote Control for Roboduino */

/*********************************
 *    Library includes           *
 * *******************************/

#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


/*********************************
 *    Global Variables           *
 *********************************/

/*
For I2C communication, the SCL port is connected to A5,
and SDA to A4. Those are Arduinos default I2C ports
*/

int read_x = A0;    //Joystick-connector X
int read_y = A1;    //Joystick-connector Y
int motor_l = 0;
int motor_r = 0;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);


/*********************************
 *    Function Prototypes        *
 *********************************/

void write_information(void);
void update_values(void);


/*********************************
 *    Arduino on start setup     *
 *********************************/

void setup() {
  /* Setup display library for 20x4 display */
  lcd.begin(20, 4);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);

  /* Setup input pins for analog digital converter */
  pinMode(read_x, INPUT);
  pinMode(read_y, INPUT);

  /* Write static text on display */
  lcd.home(); // set cursor to 0,0
  lcd.print("--------------------");
  lcd.setCursor(0, 1);
  lcd.print("***Motor  Control***");
  lcd.setCursor(0, 2);
  lcd.print("--------------------");
  lcd.setCursor(0, 3);
  lcd.print("Left:");
  lcd.setCursor(10, 3);
  lcd.print("Right:");

  // TODO:
  // init for bluetooth module
}


/*********************************
 *    Main loop where all        *
 *    Function calls are placed  *
 * *******************************/

void loop() {

  update_values();
  write_information();
  
}

/*********************************
 *    Functions                  *
 *********************************/

void write_information() {
  /* Write motor control information to display */

  lcd.setCursor(5, 3);
  lcd.print("    ");
  lcd.setCursor(16, 3);
  lcd.print("    ");

  delay(1);

  lcd.setCursor(5, 3);
  lcd.print(motor_l);

  lcd.setCursor(16, 3);
  lcd.print(motor_r);

  delay(30);
}

void update_values() {
  /*
  Read the Joystick Position and normalize them
  to fit between -255 and 255 
  */
  
  motor_l = (analogRead(read_x) - 521) / 2;
  if (motor_l < -255) {
    motor_l = -255;
 } 
  if (motor_l > 255) {
    motor_l = 255;
  }

  delay(1);   //necessary because ADC needs time

  motor_r = (analogRead(read_y) - 512) / 2;
  if (motor_r < -255) {
    motor_r = -255;
  }
  if (motor_r > 255) {
    motor_r = 255;
  }

  /* 
  Error correction because the potentiometer 
  isn't 100% precise 
  */
  
  if (-2 < motor_r && 2 > motor_r) {
    motor_r = 0;
  }
  if (-2 < motor_l && 2 > motor_l) {
    motor_l = 0;
  }
}
