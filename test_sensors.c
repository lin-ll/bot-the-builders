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
  printf("DONE SENSOR INIT\n");
  
  for(int i=0; i<1000; i++){
    double reading = Sensor_getShort(3);
    printf("reading: %f\n", reading);
    usleep(100000);
  }
  return 0;
}
