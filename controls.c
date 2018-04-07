/*----------------------------------------------------------------------------*/
/* CONTROLS receives a direction from maze and sets motor desired values                 */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "controls.h"
#include "motors.h"
#include "kalman.h"
#include "leds.h"
#include "constants.h"
#include "pid.h"

// WRITING UNDER THE ASSUMED FRAMEWORK:
/*
	1. Kalman.c processes information from sensors.h and should pass information
	2. this file will then process the movement, checking back with Kalman.c,
		maybe even with sensors.c
	3. Communicate with motors.c to move the omniwheels
*/

const int START = SQUARE_SIZE / 2;

static PID_T pidRight;
static PID_T pidTheta;

static int speed = MOTOR_RANGE / 4; // TODO dynamically change this?
static double destX;
static double destY;
static int currDir;

double forward;
double right;
double theta;

#define MIN_SPEED_FRACTION .25 // slow down to this speed as we approach dest
#define SLOW_DIST 20 // mm, start to slow down when this far away
#define STOP_DIST 1 // mm, come to a stop when this far away

// matrix entry i,j is how much velocity j contributes to motor i
// j: UL, UR, LL, LR
// i: north, east, clockwise (should be counterclockwise?)
double matrix[3][4] = {
	{1, 1, 1, 1},
	{1, -1, -1, 1},
	{1, 1, -1, -1}};

int Control_init() {
	pidRight = Pid_init(1.0, 0.0, 0.0);
	pidTheta = Pid_init(1.0, 0.0, 0.0);
	destX = START;
	destY = START;
	Pid_setPoint(pidRight, 0.0);
	Pid_setPoint(pidTheta, 0.0);
	return 0;
}

/* These 3 are used for the control vector in Kalman */
double Control_getForward() {
	return forward * MOTOR_TO_MPS;
}
double Control_getRight() {
	return right * MOTOR_TO_MPS;
}
double Control_getTheta() {
	return theta * MOTOR_TO_RPS;
}

// Returns the desired forward speed, OR returns 0 to signify that we're at the destination
int getForwardSpeed() {
	double forwardDist;
	switch (currDir) {
		case NORTH:
			forwardDist = -(destY - Kalman_getY());
			break;
		case SOUTH:
			forwardDist = destY - Kalman_getY();
			break;
		case EAST:
			forwardDist = destX - Kalman_getX();
			break;
		case WEST:
			forwardDist = -(destX - Kalman_getX());
			break;
	}

	double desiredSpeed = speed;;
	if (forwardDist <= STOP_DIST) {
		// This means speed 0, but also signifies we've arrived
		Led_setColor(0, 0, MAX_COLOR); // Blue, means we're waiting for maze now
		desiredSpeed = 0;
	} else if (forwardDist <= SLOW_DIST) {
		double speedFraction = MIN_SPEED_FRACTION + (1-MIN_SPEED_FRACTION) * (forwardDist / SLOW_DIST);
		Led_setColor((int)(MAX_COLOR - MAX_COLOR * speedFraction), (int)(MAX_COLOR * speedFraction), 0); // transition green->red
		desiredSpeed = speed * speedFraction;
	}
	Led_setColor(MAX_COLOR, MAX_COLOR, 0); // Yellow
	return desiredSpeed;
}

// primitive error correction as backup
double getRight(double dt) {
		// FRONT, BACK, LEFT, RIGHT for Sensor order
		double error;
		switch (currDir) {
			case NORTH:
				error = Sensor_getShort(3) - 90.0;
				break;
			case SOUTH:
				error = Sensor_getShort(2) - 90.0;
				break;
			case EAST:
				error = Sensor_getShort(1) - 90.0;
				break;
			case WEST:
				error = Sensor_getShort(0) - 90.0;
				break;
		}

		return error / dt;
}

double getRightSpeed(double dt) {
	// positive error means right-hand-side of the line
	double error;
	switch (currDir) {
		case NORTH:
			error = Kalman_getX() - destX;
			break;
		case SOUTH:
			error = -(Kalman_getX() - destX);
			break;
		case EAST:
			error = -(Kalman_getY() - destY);
			break;
		case WEST:
			error = Kalman_getY() - destY;
			break;
	}

	Pid_update(pidRight, error, dt);
	double val = Pid_getVal(pidRight);
	printf("Right error %.2f, PID says %.2f\n", error, val);
	return val;
}

double getThetaSpeed(double dt) {
	double error = Kalman_getT();
	Pid_update(pidTheta, error, dt);
	return Pid_getVal(pidTheta);
}

void setMotors(double northSpeed, double eastSpeed, double thetaSpeed) {
	int motors[4];
	int largest = 0;
	for (int i=0; i<4; i++) {
		double motorSpeed = 0;
		motorSpeed += matrix[0][i]*northSpeed;
		motorSpeed += matrix[1][i]*eastSpeed;
		motorSpeed += matrix[2][i]*thetaSpeed;
		motors[i] = (int)motorSpeed;
		int mag = abs(motors[i]);
		if (mag > largest) {
			largest = mag;
		}
	}

	if (largest > MOTOR_RANGE) {
		// scale down all motors so largest is MOTOR_RANGE = 255
		for (int i=0; i<4; i++) {
			double motorSpeed = (double)MOTOR_RANGE/largest;
			motors[i] = motors[i]*motorSpeed;
			if (motors[i] > MOTOR_RANGE)
				motors[i] = MOTOR_RANGE;
		}
	}

	Motor_set(motors);
}

void Control_setDir(int dir) {
	currDir = dir;
	switch (currDir) {
		case NORTH:
			destY -= SQUARE_SIZE;
			break;
		case SOUTH:
			destY += SQUARE_SIZE;
			break;
		case EAST:
			destX += SQUARE_SIZE;
			break;
		case WEST:
			destX -= SQUARE_SIZE;
			break;
	}
}

int Control_update(double dt) {
	forward = getForwardSpeed();
	if (forward == 0.0) {
		// we're done with this destination
		Motor_completeStop();
		return 1;
	}

	right = getRightSpeed(dt);
	theta = getThetaSpeed(dt);

	switch (currDir) {
		case NORTH:
			setMotors(forward, right, theta);
			break;
		case SOUTH:
			setMotors(-forward, -right, theta);
			break;
		case EAST:
			setMotors(-right, forward, theta);
			break;
		case WEST:
			setMotors(right, -forward, theta);
			break;
	}

	return 0;
}
