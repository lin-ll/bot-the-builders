#include <Wire.h>
void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  while(Serial.available())
  {
    Serial.println("Requested");
    Serial.flush();
    char c = Serial.read();
    if(c == 'H')
      {
        Serial.println("Begin Transmission");
        Serial.flush();    
        
        Wire.beginTransmission(4);
      
        int motors[4] = {0,0,0,0};
        int i,j;
        int availableBytes = Wire.requestFrom(4,16); //Request 16 bytes from slave at 4
      
        if(availableBytes == 16)
        {
          for(j=0;j<4; j++)   //Iterates through motors
          {
            for(i=0;i<4;i++)  //Iterates through bytes of each counter
            {
              motors[j] = (motors[j] << 8) | (Wire.read());
            }
            Serial.print("Motor ");
            Serial.print(j);
            Serial.print(" : ");
            Serial.println(motors[j]);
            Serial.flush();
          }
        }
        Wire.endTransmission();
        delay(500);
      }
  }
}
