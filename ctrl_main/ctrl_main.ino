/* Remote Control for Roboduino */

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>


/* Defining needed Variablees */
int read_x  = A0;
int read_y  = A1;
int motor_l = 0;
int motor_r = 0;

LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7);

void setup()
{
  /* Setup Display Library for 20x4 Display */
  lcd.begin (20,4);
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);

  /* Setup Input Pins for Analos Digital Conversion */
  pinMode(read_x, INPUT);
  pinMode(read_y, INPUT);

  /* Write static Text on Display */
  lcd.home (); // set cursor to 0,0
  lcd.print("--------------------");
  lcd.setCursor (0,1);
  lcd.print("***Motor  Control***"); 
  lcd.setCursor (0,2);
  lcd.print("--------------------");
  lcd.setCursor (0,3);
  lcd.print("Left:");  
  lcd.setCursor (10,3);
  lcd.print("Right:");
}


void write_information()
{
  /* Write Motor Control Information to Display */
  
  lcd.setCursor (5,3);
  lcd.print("    ");
  lcd.setCursor (16,3);
  lcd.print("    ");

  delay (1);
  
  lcd.setCursor (5,3);  
  lcd.print(motor_l);

  lcd.setCursor (16,3);
  lcd.print(motor_r);

  delay (30);
}


void loop()
{

  /* Read the Joystick Position and normalise them between -255 and 255 */
  motor_l = (analogRead(read_x) - 521) / 2;
  if(motor_l < -255)
  {
    motor_l = -255;
  }
  if(motor_l > 255)
  {
    motor_l = 255;
  }
  
  delay(1);
  motor_r = (analogRead(read_y) - 512) / 2;
  if(motor_r < -255)
  {
    motor_r = -255;
  }
  if(motor_r > 255)
  {
    motor_r = 255;
  }
  

  /* Error Correction because Potentiometer isn't 100% precise */
  if(-2 < motor_r && 2 > motor_r)
  {
    motor_r = 0;
  };
  if(-2 < motor_l && 2 > motor_l)
  {
    motor_l = 0;
  }
  

  write_information();
}
