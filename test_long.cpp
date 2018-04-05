#include "inc/VL53L0X.h"
#include <stdio.h>
#include "constants.h"
#include <unistd.h>
#include <pigpiod_if2.h>

const int SHORT_SHUTDOWN_PINS[4] = {SHORT_PIN_FRONT, SHORT_PIN_BACK, SHORT_PIN_LEFT, SHORT_PIN_RIGHT};
const int LONG_SHUTDOWN_PINS[4] = {LONG_PIN_FRONT, LONG_PIN_BACK, LONG_PIN_LEFT, LONG_PIN_RIGHT};

const int SHORT_DIST_ADDRS[4] = {0x2D, 0x2C, 0x2B, 0x2A};
const int LONG_DIST_ADDRS[4] = {0x31, 0x30, 0x2F, 0x2E};

int main(){
	int pi = pigpio_start(NULL, NULL);

	for(int i=0; i<4; i++) {
	    gpio_write(pi, SHORT_SHUTDOWN_PINS[i], DISTANCE_OFF);
	    printf("Turn off short distance sensor: %d\n", SHORT_SHUTDOWN_PINS[i]);
	  }

	for(int i=0; i<4; i++) {
	  gpio_write(pi, LONG_SHUTDOWN_PINS[i], DISTANCE_OFF);
	  printf("Turn off long distance sensor: %d\n", LONG_SHUTDOWN_PINS[i]);
	}

	//gpio_write(pi, LONG_SHUTDOWN_PINS[0], DISTANCE_ON);

	usleep(50000);

	VL53L0X sensor(LONG_SHUTDOWN_PINS[0], ORIG_ADDR);

	sensor.init(pi, 0);
	sensor.setTimeout(200);

	sensor.setAddress(LONG_DIST_ADDRS[0]);



	for(int i=0; i<300; i++) {
		uint16_t distance = sensor.readRangeSingleMillimeters();
		printf("Distance: %d\n", distance);
		if(sensor.timeoutOccurred()){
			printf("But timeout occurred!");
		}
	}
}
