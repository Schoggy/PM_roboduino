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
int ledmode;
bool transmit;
bool j_button_rst;
volatile bool j_button_pressed;
bool rgb_party;
int party_status;

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
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  attachInterrupt(digitalPinToInterrupt(2), j_button, LOW);
  
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  /* Setup input pins for analog digital converter */
  pinMode(read_x, INPUT);
  pinMode(read_y, INPUT);
  //pinMode(sw, INPUT);

  /* Write static text on display */
  lcd.home(); // set cursor to 0,0
  lcd.print("*Roboduino  Control*");
  lcd.setCursor(0, 1);
  lcd.print("--------------------");
  lcd.setCursor(0, 2);
  lcd.print("*** LED-MODE:");
  lcd.setCursor(17, 2);
  lcd.print("***");
  lcd.setCursor(0, 3);
  lcd.print("Left:");
  lcd.setCursor(10, 3);
  lcd.print("Right:");

  ledmode = 0;
  transmit = true; 
  j_button_pressed = false;
  j_button_rst = false;
  rgb_party = false;  
  party_status = 0;
  
  // init for bluetooth module
  Serial.begin(9600);
}

void loop() {
  if(j_button_pressed && !j_button_rst){
    changeled();
    Write_information();
    j_button_rst = true;
  }
  if(j_button_rst){
    if(digitalRead(2) == HIGH){
      j_button_rst = false;
      j_button_pressed = false;
    }
  }
  if(transmit){
    Update_values();
    Update_motor_steering();
    Write_information();
  } else {
    Update_values();
  }
  if(rgb_party){
    partytime();
  }
}

/*********************************
 *    Functions                  *
 *********************************/
 
void j_button(){
  j_button_pressed = true;
  //digitalWrite(13, HIGH);
}

void partytime(){
  ddata.funcnr = 0;
  ddata.data[0] = 0b00000110;
  ddata.data[1] = 0;
  ddata.data[2] = 0;
  ddata.data[3] = 0;
  ddata.data[4] = 0;
  ddata.data[5] = 0;
  ddata.data[6] = 0;
  
  if(party_status < 255){
    ddata.data[1] = ddata.data[4] = party_status;
  } 
    if(191 < party_status < 447) {
       ddata.data[2] = ddata.data[5] = party_status - 191;
    }
      if(383 < party_status < 639){
        ddata.data[3] = ddata.data[6] = party_status - 383;
      }
        if(party_status == 638){ 
        party_status = 0;
      }
    
  

  party_status++;
  
  Serial.write((uint8_t *)&ddata, sizeof(ddata));
}
 
void changeled() {
 /*change LED state on Roboduino */ 

  ddata.funcnr = 0;
  ddata.data[0] = 0b00000000;
  ddata.data[1] = 0;
  ddata.data[2] = 0;
  ddata.data[3] = 0;
  ddata.data[4] = 0;
  ddata.data[5] = 0;
  ddata.data[6] = 0;

 switch(ledmode){
  case 1: {
     ddata.data[0] = 0b00000001;   
  } break;
  case 2: {
     ddata.data[0] = 0b00000010;
     ddata.data[1] = 255;
     ddata.data[2] = 255;
     ddata.data[3] = 255;
  } break;
  case 3: {
     ddata.data[0] = 0b00000100;
     ddata.data[4] = 255;
     ddata.data[5] = 255;
     ddata.data[6] = 255;
  } break;
  case 4: {
     ddata.data[0] = 0b00000110;
     ddata.data[1] = 255;
     ddata.data[2] = 255;
     ddata.data[3] = 255;
     ddata.data[4] = 255;
     ddata.data[5] = 255;
     ddata.data[6] = 255;
  } break;
  case 5: {
     ddata.data[0] = 0b00001000;
  } break;
  case 6:{
     rgb_party = true;
  } break;
  default: {
     rgb_party = false;
     ledmode = 0;
  }
 }
  
  Serial.write((uint8_t *)&ddata, sizeof(ddata));
  ledmode++; 
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
  /* Write control information to display */

  lcd.setCursor(5, 3);
  lcd.print("    ");
  lcd.setCursor(16, 3);
  lcd.print("    ");
  lcd.setCursor(15, 2);
  lcd.print("  ");

  delay(1);

  lcd.setCursor(5, 3);
  lcd.print(motor_l);

  lcd.setCursor(16, 3);
  lcd.print(motor_r);

  lcd.setCursor(15, 2);
  lcd.print(ledmode);

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
  if(motor_l || motor_r) {
    transmit = true;
  } else {
    transmit = false;
  }
}
