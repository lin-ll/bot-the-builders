#include <Wire.h>
void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  while(Serial.available())
  {
    Serial.print("got character\n");
    char c = Serial.read();
    if(c == 'H')
      {
        Serial.print("Beginning transmission\n");
        Wire.beginTransmission(4);
      
        int motors[4] = {0,0,0,0};
        int i,j;
        int availableBytes = Wire.requestFrom(4,16); //Request 16 bytes from slave at 4

        Serial.print("Available bytes: ");
        Serial.print(+availableBytes);
        Serial.print("\n");
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
          }
        }
        Wire.endTransmission();
        delay(500);
      }
  }
}
