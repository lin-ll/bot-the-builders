#include "adafruit_distance.h"
#include <pigpiod_if2.h>
#include <unistd.h>
#include <stdio.h>

int pi = 0;
const int SHORT_SHUTDOWN_PINS[4] = {-1, 4, 17, 7}; // bogus pin numbers except for the -1
const int LONG_SHUTDOWN_PINS[4] = {-1, 5, 13, 21}; // bogus pin numbers except for the -1

int main(){
  pi = pigpio_start(NULL, NULL);
  for(int i=0; i<4; i++){
    if(i==0) break;
    gpio_write(pi, SHORT_SHUTDOWN_PINS[i], 0);
    gpio_write(pi, LONG_SHUTDOWN_PINS[i], 0);
  }

  printf("error code is <%s>\n", pigpio_error(-25));
  //printf(pigpio_error(-83));
  //Sensor_init(pi);

  int handle = i2c_open(pi, 1, 0x29, 0);
  printf("HANDLE %d\n\n", handle);
  int success = adafruit_distance_begin(handle);
    

  printf("SUCCESS WAS %d\n", success);
  printf("DONE SENSOR INIT\n");
  for(int i=0; i<600; i++){
    double reading = adafruit_distance_readRange(3);
    printf("reading: %f\n", reading);
    usleep(100000);
  }
  return 0;
}
