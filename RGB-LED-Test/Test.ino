#define LED_R 11
#define LED_G 10
#define LED_B 9


void setup() 
{
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void loop()
{
  /*
  setColor(255, 0, 0); // R
  delay(100);
  setColor(0, 255, 0); // G
  delay(100);
  setColor(0, 0, 255); // B
  delay(100);
  setColor(255, 0, 255); // Lila
  delay(100);
  */
  for(int i = 0; i < 256; i++)
  {
    setColor(i, 0, 255 - i);
    delay(10);
  }
  for(int i = 0; i < 256; i++)
  {
    setColor(255 - i, 0, i);
    delay(10);
  }
}

void setColor(byte r, byte g, byte b)
{
  analogWrite(LED_R, r);
  analogWrite(LED_G, g);
  analogWrite(LED_B, b);
}
