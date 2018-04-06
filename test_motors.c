#include <pigpiod_if2.h>
#include <unistd.h>
#include "constants.h"
#include "motors.h"

const int FORWARD[4] = {UPPER_LEFT_FORWARD, UPPER_RIGHT_FORWARD, LOWER_LEFT_FORWARD, LOWER_RIGHT_FORWARD};
const int BACKWARD[4] = {UPPER_LEFT_BACKWARD, UPPER_RIGHT_BACKWARD, LOWER_LEFT_BACKWARD, LOWER_RIGHT_BACKWARD};
const int FORWARD = 0;
const int RIGHT = 1;
const int BACKWARD = 2;
const int LEFT = 3;

int pi = 0;

void set(int direction, int speed) {
	int speeds[4];
	switch (direction) {
		case FORWARD:
			speeds = {speed, speed, speed, speed};
			break;
		case BACKWARD:
			speeds = {-speed, -speed, -speed, -speed};
			break;
		case RIGHT:
			speeds = {speed, -speed, -speed, speed};
			break;
		case LEFT:
			speeds = {-speed, speed, speed, -speed};
			break;
	}
	Motor_set(speeds);
}

int main(){
	pi = pigpio_start(NULL, NULL);
	Motor_init(pi);
	int speed = 255;
	int time = 2000000;

	for (int i = 0; i < 4; i++) {
		set(i, speed);
		usleep(time);
		Motor_off();
		usleep(500000);
	}

	return 0;
}
