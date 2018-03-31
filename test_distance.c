#include "inc/adafruit_distance.h"
#include <pigpiod_if2.h>
#include <unistd.h>
#include <stdio.h>
#include "inc/constants.h"

int pi = 0;
const int SHORT_SHUTDOWN_PINS[4] = {SHORT_PIN_FRONT, SHORT_PIN_BACK, SHORT_PIN_LEFT, SHORT_PIN_RIGHT};
const int LONG_SHUTDOWN_PINS[4] = {LONG_PIN_FRONT, LONG_PIN_BACK, LONG_PIN_LEFT, LONG_PIN_RIGHT};

int main(){
  pi = pigpio_start(NULL, NULL);
  for(int i=0; i<4; i++){
    int short_pin = SHORT_SHUTDOWN_PINS[i];
    int long_pin = LONG_SHUTDOWN_PINS[i];
    printf("doing write to %d and %d\n", short_pin, long_pin);
    if(short_pin != -1) gpio_write(pi, short_pin, 0);
    if(long_pin != -1) gpio_write(pi, long_pin, 0);
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
