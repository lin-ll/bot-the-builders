#define I2C_SLAVE_ADDRESS 0x4 // the 7-bit addresS
int debug = 10;

#include "TinyWireS.h" 

void setup() {
  //-----------------------
  // SETUP FOR THE i2c
  //-----------------------
  pinMode(debug, OUTPUT); // Debug PIN is HIGH when sending data
  digitalWrite(debug, LOW); 

  TinyWireS.begin(I2C_SLAVE_ADDRESS);
  TinyWireS.onRequest(requestEvent);
}

//ISR for i2c request from the PI
void requestEvent()
{  
    byte data = 50;
    digitalWrite(debug, HIGH); //Set the DEBUG Pin HIGH for Sending Data
    TinyWireS.send(data);   //Send all 16 bytes
}

void loop() {
  // This needs to be here
  TinyWireS_stop_check();  
 
}
