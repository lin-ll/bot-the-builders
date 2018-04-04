#include "sensors.h"
#include <pigpiod_if2.h>
#include <unistd.h>
#include <stdio.h>

int pi = 0;

void testShort() {
  for(int i=0; i<1000; i++){
    double reading0 = Sensor_getShort(0);
    double reading1 = Sensor_getShort(1);
    double reading2 = Sensor_getShort(2);
    double reading3 = Sensor_getShort(3);

    printf("%f\t%f\t%f\t%f\n", reading0, reading1, reading2, reading3);
  }
}

void testLong() {
  for(int i=0; i<10; i++){
    double reading0 = Sensor_getLong(0);
    double reading1 = Sensor_getLong(1);
    double reading2 = Sensor_getLong(2);
    double reading3 = Sensor_getLong(3);

    printf("%f\t%f\t%f\t%f\n", reading0, reading1, reading2, reading3);
  }
}

int main(){
  pi = pigpio_start(NULL, NULL);

  printf("error code is <%s>\n", pigpio_error(-2011));
  //printf(pigpio_error(-83));

  Sensor_init(pi);
  printf("DONE SENSOR INIT\n");

  // printf("Testing short distance sensors:\n");
  // testShort();

  printf("Testing long distance sensors:\n");
  testLong();

  return 0;
}
