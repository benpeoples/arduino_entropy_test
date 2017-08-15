void setup() {
  int y;

  Serial.begin(9600);

  for(y = 0; y < 1000; y++)
  {
    Serial.print(y);
    Serial.print(',');
    Serial.println(get_seed(1));
  }

}

uint8_t get_seed(int pin)
{
  int x;
  uint8_t seed = 0;

  for(x = 0; x < 1024; x++)
  {
    seed += analogRead(pin);
  }

  return seed;
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
