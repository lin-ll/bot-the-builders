#include <stdio.h>
#include "kalman.h"
#include <math.h> // for NAN

void print() {
	double x  = Kalman_getX();
	double y  = Kalman_getY();
	double t  = Kalman_getT();
	double vx = Kalman_getVx();
	double vy = Kalman_getVy();
	double vt = Kalman_getVt();

	printf("%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t", x, y, t, vx, vy, vt);
}



/* TODO: we almost certainly need a control vector for vx so we can tell the 
   difference between driving straight while a little too far right and 
   constantly strafing left while being rotated too far right */
int main() {
	for(int i=0; i<100; i++){
		double simulatedY = i*1.0;
		double simulatedX = i>50? (i-50)*.01 : 0.0;

		double gyro = 0;
		double compass = NAN;

		double dt = 0.1;

		double encoders[4];
		for(int i=0; i<3; i++){
			encoders[4] = NAN;
		}

		double distances[8];
		distances[0] = NAN; // forward
		distances[1] = simulatedY; // back
		distances[2] = 90+simulatedX; // left
		distances[3] = 90-simulatedX; //right
		distances[4] = NAN;
		distances[5] = NAN;
		distances[6] = NAN;
		distances[7] = NAN;

		double *control = NULL; // for now!

		Kalman_update_given_sensors(dt, encoders, distances, gyro, compass, control);
	}
}