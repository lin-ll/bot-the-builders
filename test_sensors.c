#include "sensors.h"
#include <pigpiod_if2.h>
#include <unistd.h>
#include <stdio.h>

int pi = 0;

int main(){
  pi = pigpio_start(NULL, NULL);
  

  printf("error code is <%s>\n", pigpio_error(-2011));
  //printf(pigpio_error(-83));

  Sensor_init(pi);

  
  for(int i=0; i<5; i++){
    double reading = Sensor_getLong(0);
    printf("reading: %f\n", reading);
    sleep(300);
  }
  return 0;
}
