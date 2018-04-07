#include <pigpiod_if2.h>
#include <unistd.h>
#include "constants.h"
#include "motors.h"

const int FORWARD[4] = {UPPER_LEFT_FORWARD, UPPER_RIGHT_FORWARD, LOWER_LEFT_FORWARD, LOWER_RIGHT_FORWARD};
const int BACKWARD[4] = {UPPER_LEFT_BACKWARD, UPPER_RIGHT_BACKWARD, LOWER_LEFT_BACKWARD, LOWER_RIGHT_BACKWARD};
const int F = 0;
const int R = 1;
const int B = 2;
const int L = 3;
const int S = 4;

int pi = 0;

void set(int direction, int speed) {
	int speeds[4] = {speed, speed, speed, speed};
	switch (direction) {
		case B:
			for (int i = 0; i < 4; i++) {
				speeds[i] = -speed;
			}
			break;
		case R:
			speeds[1] = -speed;
			speeds[2] = -speed;
			break;
		case L:
			speeds[0] = -speed;
			speeds[3] = -speed;
			break;
		case S:
			speeds[0] = -speed;
			speeds[2] = -speed;
			break;
	}
	Motor_set(speeds);
}

int main(){
	pi = pigpio_start(NULL, NULL);
	Motor_init(pi);
	int speed = 255;
	int time = 2000000;

	set(S, 255/2);
	usleep(3000000);
	Motor_off();

	/*
	for (int i = 0; i < 4; i++) {
		set(i, speed);
		usleep(time);
		Motor_off();
		usleep(500000);
	}
	*/

	return 0;
}
