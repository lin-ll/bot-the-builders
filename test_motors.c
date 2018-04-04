#include <pigpiod_if2.h>
#include <unistd.h>
#include <stdio.h>
#include "constants.h"
#include "motors.h"

const int FORWARD[4] = {UPPER_LEFT_FORWARD, UPPER_RIGHT_FORWARD, LOWER_LEFT_FORWARD, LOWER_RIGHT_FORWARD};
const int BACKWARD[4] = {UPPER_LEFT_BACKWARD, UPPER_RIGHT_BACKWARD, LOWER_LEFT_BACKWARD, LOWER_RIGHT_BACKWARD};

int pi = 0;

void set_forward(int speed) {
	int speeds[4] = {speed, speed, speed, speed};
	for (int i = 0; i < 4; i++) {
		Motor_adjust(FORWARD[i], BACKWARD[i], speeds[i]);
	}
}

void set_backward(int speed) {
	int speeds[4] = {-speed, -speed, -speed, -speed};
	for (int i = 0; i < 4; i++) {
		Motor_adjust(FORWARD[i], BACKWARD[i], speeds[i]);
	}
}

void set_right(int speed) {
	int speeds[4] = {speed, -speed, -speed, speed};
	for (int i = 0; i < 4; i++) {
		Motor_adjust(FORWARD[i], BACKWARD[i], speeds[i]);
	}
}

void set_left(int speed) {
	int speeds[4] = {-speed, speed, speed, -speed};
	for (int i = 0; i < 4; i++) {
		Motor_adjust(FORWARD[i], BACKWARD[i], speeds[i]);
	}
}

int main(){
	pi = pigpio_start(NULL, NULL);
	int speed = 255;
	int time = 2000000;
	
	set_forward(speed);
	usleep(time);
	Motor_off();
	usleep(500000);

	set_right(speed);
	usleep(time);
	Motor_off();
	usleep(500000);

	set_backward(speed);
	usleep(time);
	Motor_off();
	usleep(500000);

	set_left(speed);
	usleep(time);
	Motor_off();
	usleep(500000);

	return 0;
}
