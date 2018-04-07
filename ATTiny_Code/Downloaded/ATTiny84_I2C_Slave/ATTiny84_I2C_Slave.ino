/*
  Goal ... have the ATTiny84 join an I2C network as a slave 
  with an Arduino UNO as the I2C network master

  using the information from 
  http://thewanderingengineer.com/2014/02/17/attiny-i2c-slave/
  as the starting point.

*/

#define I2C_SLAVE_ADDRESS 0x3   //this slave address (0x1, 0x2, 0x3)
#include <TinyWireS.h>          //the ATTiny Wire library
int LEDPin = A0 ;                //indicator LED
int i=0;

void setup() {
  TinyWireS.begin(I2C_SLAVE_ADDRESS);
  TinyWireS.onRequest(requestEvent);
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  TinyWireS_stop_check();
}

void requestEvent()
{
  if(i%2==0)
  {digitalWrite(LEDPin, LOW);}
  else
  {digitalWrite(LEDPin, HIGH);}
  TinyWireS.send(i);
  i++;
}
