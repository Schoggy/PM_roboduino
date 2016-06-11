/* Remote Control for Roboduino */

#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Struct designed for transmitting data
struct data_packet {
  char funcnr;
  char data[7];
};

/*********************************
 *    Global Variables           *
 *********************************/

//IC2: SDA => A4; SCL => A5
//Button is on D2

int read_x = A0;
int read_y = A1;
int motor_l = 0;
int motor_r = 0;
int ledsw = 0;
data_packet ddata;
int ledmode = 0;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);

/*********************************
 *    Function Prototypes        *
 *********************************/

void Write_information(void);
void Update_values(void);
void Update_motor_steering(void);
void Change_led(void);

/*********************************
 *    Main Arduino Functions     *
 *********************************/

void setup() {
  /*Setup display library for 20x4 display */
  lcd.begin(20, 4);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  //attachInterrupt(digitalPinToInterrupt(2),Changeled,RISING)
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  /* Setup input pins for analog digital converter */
  pinMode(read_x, INPUT);
  pinMode(read_y, INPUT);
  //pinMode(sw, INPUT);

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
  
  // init for bluetooth module
  Serial.begin(9600);
}

void loop() {

  Update_values();
  Write_information();
  Update_motor_steering();
}

/*********************************
 *    Functions                  *
 *********************************/
 
 
void Changeled() {
 /*change LED state on Roboduino */ 
 
 if(ledmode > 4)
 {
   ledmode = 0;
 } 
 
 if(ledmode = 0)
 {
     ddata.funcnr = 0;
     ddata.data[0] = 1;
     ddata.data[1] = 0;
     ddata.data[2] = 0;
     ddata.data[3] = 0;
     ddata.data[4] = 0;
     ddata.data[5] = 0;
     ddata.data[6] = 0;     Serial.write((uint8_t *)&ddata, sizeof(ddata));
 }
 if(ledmode = 1)
 {
     ddata.funcnr = 0;
     ddata.data[0] = 0;
     ddata.data[1] = 1;
     ddata.data[2] = 0;
     ddata.data[3] = 0;
     ddata.data[4] = 0;
     ddata.data[5] = 0;
     ddata.data[6] = 0;     Serial.write((uint8_t *)&ddata, sizeof(ddata));
 }
 if(ledmode = 2)
 {
     ddata.funcnr = 0;
     ddata.data[0] = 0;
     ddata.data[1] = 0;
     ddata.data[2] = 1;
     ddata.data[3] = 0;
     ddata.data[4] = 0;
     ddata.data[5] = 0;
     ddata.data[6] = 0;     Serial.write((uint8_t *)&ddata, sizeof(ddata));
 }
 if(ledmode = 3)
 {
     ddata.funcnr = 0;
     ddata.data[0] = 0;
     ddata.data[1] = 1;
     ddata.data[2] = 1;
     ddata.data[3] = 0;
     ddata.data[4] = 0;
     ddata.data[5] = 0;
     ddata.data[6] = 0;
     Serial.write((uint8_t *)&ddata, sizeof(ddata));
 }
 if(ledmode = 4)
 {
     ddata.funcnr = 0;
     ddata.data[0] = 0;
     ddata.data[1] = 0;
     ddata.data[2] = 0;
     ddata.data[3] = 1;
     ddata.data[4] = 0;
     ddata.data[5] = 0;
     ddata.data[6] = 0;
     Serial.write((uint8_t *)&ddata, sizeof(ddata));
 } 
 
  led_mode++; 
}

void Update_motor_steering()
{
 /* Send motor steering signals to Roboduino */ 
 
 ddata.funcnr = 1;
 
 if(motor_l > 0)
 {
   ddata.data[0] = 1;
   ddata.data[1] = (char) motor_l;
 }
 else
 {
   ddata.data[0] = 0;
   ddata.data[1] = (char) (motor_l *-1);
 }
 
 if(motor_r > 0)
 {
   ddata.data[2] = 1;
   ddata.data[3] = (char) motor_r;
 }
 else
 {
   ddata.data[2] = 0;
   ddata.data[3] = (char) (motor_r * -1);
 }

 Serial.write((uint8_t *)&ddata, sizeof(ddata));
  
}
 

void Write_information() {
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

void Update_values() {

  /* Read the Joystick Position and normalise them between -255 and 255 */
  motor_l = (analogRead(read_x) - 513) / 2;
  if (motor_l < -254) {
    motor_l = -254;
  }
  if (motor_l > 255) {
    motor_l = 255;
  }

  delay(1);

  motor_r = (analogRead(read_y) - 520) / 2;
  if (motor_r < -254) {
    motor_r = -254;
  }
  if (motor_r > 255) {
    motor_r = 255;
  }

  /* Error correction because the potentiometer isn't 100% precise */
  if (-2 < motor_r && 2 > motor_r) {
    motor_r = 0;
  }
  if (-2 < motor_l && 2 > motor_l) {
    motor_l = 0;
  }
  
}
