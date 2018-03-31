#include <pigpiod_if2.h>
#include <unistd.h>
#include <stdio.h>
#include "constants.h"

int forward[4] = {UPPER_LEFT_FORWARD, UPPER_RIGHT_FORWARD, LOWER_LEFT_FORWARD, LOWER_RIGHT_FORWARD};
int backward[4] = {UPPER_LEFT_BACKWARD, UPPER_RIGHT_BACKWARD, LOWER_LEFT_BACKWARD, LOWER_RIGHT_BACKWARD};

int pi = 0;


void set_forward(int speed) {
	set_PWM_dutycycle(pi, forward[0], speed);
	set_PWM_dutycycle(pi, backward[0], 0);
	set_PWM_dutycycle(pi, forward[1], speed);
	set_PWM_dutycycle(pi, backward[1], 0);
	set_PWM_dutycycle(pi, forward[2], speed);
	set_PWM_dutycycle(pi, backward[2], 0);
	set_PWM_dutycycle(pi, forward[3], speed);
	set_PWM_dutycycle(pi, backward[3], 0);
}

void set_backward(int speed) {
	set_PWM_dutycycle(pi, forward[0], 0);
	set_PWM_dutycycle(pi, backward[0], speed);
	set_PWM_dutycycle(pi, forward[1], 0);
	set_PWM_dutycycle(pi, backward[1], speed);
	set_PWM_dutycycle(pi, forward[2], 0);
	set_PWM_dutycycle(pi, backward[2], speed);
	set_PWM_dutycycle(pi, forward[3], 0);
	set_PWM_dutycycle(pi, backward[3], speed);
}

void set_right(int speed) {
	set_PWM_dutycycle(pi, forward[0], speed);
	set_PWM_dutycycle(pi, backward[0], 0);
	set_PWM_dutycycle(pi, forward[1], 0);
	set_PWM_dutycycle(pi, backward[1], speed);
	set_PWM_dutycycle(pi, forward[2], 0);
	set_PWM_dutycycle(pi, backward[2], speed);
	set_PWM_dutycycle(pi, forward[3], speed);
	set_PWM_dutycycle(pi, backward[3], 0);
}

void set_left(int speed) {
	set_PWM_dutycycle(pi, forward[0], 0);
	set_PWM_dutycycle(pi, backward[0], speed);
	set_PWM_dutycycle(pi, forward[1], speed);
	set_PWM_dutycycle(pi, backward[1], 0);
	set_PWM_dutycycle(pi, forward[2], speed);
	set_PWM_dutycycle(pi, backward[2], 0);
	set_PWM_dutycycle(pi, forward[3], 0);
	set_PWM_dutycycle(pi, backward[3], speed);
}

void set_off() {
	set_PWM_dutycycle(pi, forward[0], 0);
	set_PWM_dutycycle(pi, backward[0], 0);
	set_PWM_dutycycle(pi, forward[1], 0);
	set_PWM_dutycycle(pi, backward[1], 0);
	set_PWM_dutycycle(pi, forward[2], 0);
	set_PWM_dutycycle(pi, backward[2], 0);
	set_PWM_dutycycle(pi, forward[3], 0);
	set_PWM_dutycycle(pi, backward[3], 0);
}


int main(){
	pi = pigpio_start(NULL, NULL);
	int speed = 255;
	int time = 2000000;
	
	set_forward(speed);
	usleep(time);
	set_off();
	usleep(500000);

	set_right(speed);
	usleep(time);
	set_off();
	usleep(500000);

	set_backward(speed);
	usleep(time);
	set_off();
	usleep(500000);

	set_left(speed);
	usleep(time);
	set_off();
	usleep(500000);




	/*
	
	for(int i=0; i<1000; i++){
		for(int j=0; j<3; j++){
			printf("%d forward\n", j);
			set_PWM_dutycycle(pi, forward[j], 127);
			set_PWM_dutycycle(pi, backward[j], 0);
			usleep(1000000);

			set_PWM_dutycycle(pi, forward[j], 255);
			set_PWM_dutycycle(pi, backward[j], 0);
			usleep(1000000);

			set_PWM_dutycycle(pi, forward[j], 0);
			set_PWM_dutycycle(pi, backward[j], 0);
			usleep(1000000);



			printf("%d backward\n", j);
			set_PWM_dutycycle(pi, forward[j], 0);
			set_PWM_dutycycle(pi, backward[j], 127);
			usleep(1000000);

			set_PWM_dutycycle(pi, forward[j], 0);
			set_PWM_dutycycle(pi, backward[j], 255);
			usleep(1000000);

			set_PWM_dutycycle(pi, forward[j], 0);
			set_PWM_dutycycle(pi, UPPER_LEFT_BACKWARD, 0);
			usleep(1000000);
		}
		*/
	return 0;
}
