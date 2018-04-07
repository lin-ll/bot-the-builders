#define I2C_SLAVE_ADDR  0x4
#include <Wire.h>
byte data;

int SLAVE_RESET_PIN = 3;

void setup() {
  Wire.begin();
  Wire.setClock(1000L);  // 400kHz
  Serial.begin(9600);
}

void loop() {
  while(Serial.available())
  {
    Serial.println("Requested");
    char c = Serial.read();
    if(c == 'H')
    {
        Serial.println("Begin Transmission");
        Wire.beginTransmission(I2C_SLAVE_ADDR);
      
        //int availableBytes = Wire.requestFrom(4,1); //Request 1 byte from slave at 4
      
        //if(availableBytes == 1)
        Wire.requestFrom(4,1);
        
        while(Wire.available())
        {
          data = Wire.read();
          Serial.println(data);
          
        }
      }
  }
  delay(500);
}
