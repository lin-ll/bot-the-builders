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
        
        //Wire.beginTransmission(0x4);
        
        Serial.println("gave it 4");
        Serial.flush();    
      
        int motors[4] = {0,0,0,0};
        int i,j;
        int availableBytes = Wire.requestFrom(4,1); //Request 16 bytes from slave at 4
        
        Serial.println("did request");
        
        Serial.println(availableBytes);
        Serial.flush();    
      
        if(availableBytes == 1)
        {
          for(j=0;j<4; j++)   //Iterates through motors
          {
            for(i=0;i<4;i++)  //Iterates through bytes of each counter
            {
              delay(1);
              int temp = Wire.read();
              Serial.print(temp);
              motors[j] = (motors[j] << 8) | (temp);
            }
            Serial.print("Motor ");
            Serial.print(j);
            Serial.print(" : ");
            Serial.println(motors[j]);
            Serial.flush();
          }
        }
        
        Serial.println("out of if");
        Serial.flush();    
        Wire.endTransmission();
        delay(500);
      }
  }
}
