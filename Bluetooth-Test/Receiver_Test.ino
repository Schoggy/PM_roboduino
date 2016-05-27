/*
  Simple test program for the bluetooth receiver

  commands:
  1 to turn the built-in LED on
  0 to turn the built-in LED off
  b to blink

  TODO: the serial buffer can hold 64 bytes , use Serial.overflow() to check if there was an overflow
*/

#define LED_PIN   13


void setup(void);
void loop(void);
void handle_data(void);
void blink(void);

bool isBlinking = true;
bool ledOn = false;
char junk;
String inputString = "";
int ledToggleCounter = 0;

unsigned long lastMillis = 0;
unsigned long newMillis = 0;

void setup()
{
  Serial.begin(9600);            // set the baud rate to 9600, (same should be set in the serial monitor if connected to usb)
  pinMode(LED_PIN, OUTPUT);      // pin 13 allows to control the built-in led of the arduino UNO
  Serial.println("Roboduino receiver test tool started");
}

void loop()
{
  handle_data();
  blink();
}

void handle_data()
{
  if (Serial.available() > 0) // if there is data in the serial receive buffer start reading
  {
    while (Serial.available())
    {
      char inChar = (char)Serial.read(); //read the input
      inputString += inChar;        //make a string of the characters coming on serial
    }
    Serial.println("Received: " + inputString); // send received string back to the transmitter, in order to check if the string was really received

    while (Serial.available() > 0)
    {
      junk = Serial.read() ;  // clear the serial buffer
    }
    if (inputString == "1")
    {
      isBlinking = false;
      digitalWrite(LED_PIN, HIGH); //in case of '1' turn the LED on
      Serial.print("LED On ");
      Serial.println(++ledToggleCounter);
    }
    else if (inputString == "0")
    {
      isBlinking = false;
      digitalWrite(LED_PIN, LOW); //incase of '0' turn the LED off
      Serial.print("LED Off ");
      Serial.println(++ledToggleCounter);
    }
    else if (inputString == "b")
    {
      isBlinking = !isBlinking;
      Serial.println(isBlinking ? "Blink mode enabled" : "Blink mode disabled");
    }
    
    inputString = ""; //clear the string
  }
}


void blink()
{
  if (isBlinking) // if enabled the led gets toggled every 500 ms
  {
    newMillis = millis();
    if (newMillis >= lastMillis + 500)
    {
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn);
      Serial.print(ledOn ? "LED On " : "LED Off ");
      Serial.println(++ledToggleCounter);
      lastMillis = newMillis;
    }
  }

}
