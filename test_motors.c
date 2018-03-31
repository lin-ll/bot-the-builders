#include <pigpiod_if2.h>
#include <unistd.h>
#include <stdio.h>


#define UPPER_LEFT_FORWARD 21
#define UPPER_RIGHT_FORWARD 23
#define UPPER_LEFT_BACKWARD 22
#define UPPER_RIGHT_BACKWARD 24
#define LOWER_LEFT_FORWARD 27
#define LOWER_RIGHT_FORWARD 31
#define LOWER_LEFT_BACKWARD 28
#define LOWER_RIGHT_BACKWARD 32

int forward[4] = {UPPER_LEFT_FORWARD, UPPER_RIGHT_FORWARD, LOWER_LEFT_FORWARD, LOWER_RIGHT_FORWARD};
int backward[4] = {UPPER_LEFT_BACKWARD, UPPER_RIGHT_BACKWARD, LOWER_LEFT_BACKWARD, LOWER_RIGHT_BACKWARD};

int pi = 0;

int main(){
	pi = pigpio_start(NULL, NULL);

	
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

		double reading = Sensor_getShort(3);
		printf("reading: %f\n", reading);
		usleep(100000);
	}
	return 0;
}