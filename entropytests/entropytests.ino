// Various forms of entropy generation.

void setup() {
  int x;
  uint8_t accum = 0;
  uint16_t temp = 0;
  int y;

  Serial.begin(9600);

  Serial.println("Unconnected ADC pin, RST config");

  for(x = 0; x < 256; x++)
  {
    temp = analogRead(1);
    Serial.print(millis());
    Serial.print(',');
    Serial.println(temp);
    accum += (temp & 0x01);
  }

  Serial.print("After 256 cycles, got: ");
  Serial.println(accum);

  accum = 0;

  Serial.println("Now testing 1024 cycles, 256 times");

  for(x = 0; x < 256; x++)
  {
    accum = 0;
    for(y = 0; y < 1024; y++)
    {
      accum += (analogRead(1) & 0x01);
    }
    Serial.print(x);
    Serial.print(',');
    Serial.println(accum);
  }

    Serial.println("Unconnected ADC pin, Adjacent pins driven low");
    pinMode(A0,OUTPUT);
    pinMode(A2,OUTPUT);
    digitalWrite(A0,LOW);
    digitalWrite(A2,LOW);

  for(x = 0; x < 256; x++)
  {
    temp = analogRead(1);
    Serial.print(millis());
    Serial.print(',');
    Serial.println(temp);
    accum += (temp & 0x01);
  }

  Serial.print("After 256 cycles, got: ");
  Serial.println(accum);

  accum = 0;

  Serial.println("Now testing 1024 cycles, 256 times");

  for(x = 0; x < 256; x++)
  {
    accum = 0;
    for(y = 0; y < 1024; y++)
    {
      accum += (analogRead(1) & 0x01);
    }
    Serial.print(x);
    Serial.print(',');
    Serial.println(accum);
  }

    Serial.println("Unconnected ADC pin, Adjacent pins driven high");
    pinMode(A0,OUTPUT);
    pinMode(A2,OUTPUT);
    digitalWrite(A0,HIGH);
    digitalWrite(A2,HIGH);

  for(x = 0; x < 256; x++)
  {
    temp = analogRead(1);
    Serial.print(millis());
    Serial.print(',');
    Serial.println(temp);
    accum += (temp & 0x01);
  }

  Serial.print("After 256 cycles, got: ");
  Serial.println(accum);

  accum = 0;

  Serial.println("Now testing 1024 cycles, 256 times");

  for(x = 0; x < 256; x++)
  {
    accum = 0;
    for(y = 0; y < 1024; y++)
    {
      accum += (analogRead(1) & 0x01);
    }
    Serial.print(x);
    Serial.print(',');
    Serial.println(accum);
  }

    Serial.println("Temperature pin, RST config");

  for(x = 0; x < 256; x++)
  {
    temp = get_temp();
    Serial.print(millis());
    Serial.print(',');
    Serial.println(temp);
    accum += (temp & 0x01);
  }

  Serial.print("After 256 cycles, got: ");
  Serial.println(accum);

  accum = 0;

  Serial.println("Now testing 1024 cycles, 256 times");

  for(x = 0; x < 256; x++)
  {
    accum = 0;
    for(y = 0; y < 1024; y++)
    {
      accum += (get_temp() & 0x01);
    }
    Serial.print(x);
    Serial.print(',');
    Serial.println(accum);
  }

 Serial.println("Temperature pin, ADC clock prescaler = 8");

  for(x = 0; x < 256; x++)
  {
    temp = get_temp_badly();
    Serial.print(millis());
    Serial.print(',');
    Serial.println(temp);
    accum += (temp & 0x01);
  }

  Serial.print("After 256 cycles, got: ");
  Serial.println(accum);

  accum = 0;

  Serial.println("Now testing 1024 cycles, 256 times");

  for(x = 0; x < 256; x++)
  {
    accum = 0;
    for(y = 0; y < 1024; y++)
    {
      accum += (get_temp_badly() & 0x01);
    }
    Serial.print(x);
    Serial.print(',');
    Serial.println(accum);
  }

}

// From https://playground.arduino.cc/Main/InternalTemperatureSensor
uint16_t get_temp()
{
  
   ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  //delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  return ADCW;
}

uint16_t get_temp_badly()
{
  
   ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
   ADCSRA = 0b100; // Set prescaler to 8
  ADCSRA |= _BV(ADEN);  // enable the ADC

  //delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  return ADCW;
}


void loop() {
  // put your main code here, to run repeatedly:

}
