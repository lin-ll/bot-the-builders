/*----------------------------------------------------------------------------*/
/* Basic robot motions and sensor reading.                                    */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include "general.h"
#include "motors.h"
#include "sensors.h"
#include "kalman.h"
#include "constants.h"
#include "control.h"
#include "pid.h"

// WRITING UNDER THE ASSUMED FRAMEWORK:
/*
	1. kalman.c processes information from sensors.h and should pass information
	2. this file will then process the movement, checking back with kalman.c,
		maybe even with sensors.c
	3. Communicate with motors.c to move the omniwheels
*/

static PID_T pidLeftRight;
static PID_T pidUpDown;
static PID_T pidTheta;

static double speed;

int setup() {
    pidLeftRight = init(1.0, 10.0, 10.0);
    pidUpDown = init(1.0, 10.0, 10.0);
    pidTheta = init(1.0, 10.0, 10.0);
    setPoint(pidUpDown, 0.0);
    setPoint(pidLeftRight, 0.0);
    setPoint(pidTheta, 0.0);
    return 0;
}

// moves a given distance c forwards
void forwards() {
	// double currLeft, currRight, finalDist;

	// one thing to note is deceleration, so we can't really stop at finalDist
	// we have to start slowing down at some point before finalDist (finalDist-offset)
	Motor_setUpperLeft(speed);
	Motor_setUpperRight(speed);
  Motor_setLowerLeft(speed);
  Motor_setLowerRight(speed);
	// start slowing down
  /* Robot seems to stop quite well, deceleration does not seem necessary.
	if (<insert condition from kalman> < finalDist) {
		currLeft = Motor_getUpperLeft();
		currRight = Motor_getUpperRight();
		Motor_setUpperLeft(currLeft - amount);
		Motor_setUpperRight(currRight - amount);
	}
  */
}

void forwardsAdjust(double adjust) {
  Motor_setUpperLeft(adjust);
	Motor_setUpperRight(adjust);
  Motor_setLowerLeft(adjust);
  Motor_setLowerRight(adjust);
}

// moves left distance c
void left() {
  Motor_setUpperLeft(-speed);
	Motor_setUpperRight(speed);
  Motor_setLowerLeft(speed);
  Motor_setLowerRight(-speed);
}

void leftAdjust(double adjust) {
  Motor_setUpperLeft(-adjust);
	Motor_setUpperRight(adjust);
  Motor_setLowerLeft(adjust);
  Motor_setLowerRight(-adjust);
}

// moves right distance c
void right() {
  Motor_setUpperLeft(speed);
	Motor_setUpperRight(-speed);
  Motor_setLowerLeft(-speed);
  Motor_setLowerRight(speed);
}

void rightAdjust(double adjust) {
  Motor_setUpperLeft(adjust);
	Motor_setUpperRight(-adjust);
  Motor_setLowerLeft(-adjust);
  Motor_setLowerRight(adjust);
}

// moves backwards distance c
void back() {
  Motor_setUpperLeft(-speed);
	Motor_setUpperRight(-speed);
  Motor_setLowerLeft(-speed);
  Motor_setLowerRight(-speed);
}

void backAdjust(double adjust) {
  Motor_setUpperLeft(-adjust);
	Motor_setUpperRight(-adjust);
  Motor_setLowerLeft(-adjust);
  Motor_setLowerRight(-adjust);
}

// reads distance from long
double checkDistanceFront() {
	enum Dir_t targetDir = kFRONT;
	return Sensor_getLong(targetDir);
}

double checkDistanceBack() {
	enum Dir_t targetDir = kBACK;
	return Sensor_getLong(targetDir);
}

double checkDistanceLeft() {
	enum Dir_t targetDir = kLEFT;
	return Sensor_getLong(targetDir);
}

double checkDistanceRight() {
	enum Dir_t targetDir = kRIGHT;
	return Sensor_getLong(targetDir);
}

// maintains robot in center of path
// dynamically adjust the speed of the wheels
void maintainLR(double targetX) {
    // calls check Distance Front and Back
    double currX = kalman_getX();
		double xError = currX - targetX;
		update(pidLeftRight, xError, dt);
		double correction = getVal(pidLeftRight);
		if (xError > 0) {
				leftAdjust(correction);
		} else {
				rightAdjust(correction);
		}
}

void maintainFB(double targetY) {
    // calls check Distance Left and Right
		double currY = kalman_getY;
    double yError = currY - targetY;
		update(pidUpDown, yError, dt);
		double correction = getVal(pidUpDown);
		if (yError > 0) {
				forwardsAdjust(correction);
		} else {
				backAdjust(correction);
		}
}

//TODO: ensure robot stays in same orientation
void maintainTheta() {
		double radius = 7.0; // units mm
		double thetaError = Sensor_getGyro();
		update(pidTheta, thetaError, dt);
		double correction = getVal(pidTheta);
		correction = correction * radius;
		//TODO: put correction back into motors
		Motor_setUpperLeft(correction);
		Motor_setUpperRight(correction);
		Motor_setLowerLeft(correction);
		Motor_setLowerRight(correction);
}

// returns value read from sensor s
//double sensor(int s) {
    // something to distinguish short and long sensors?
    /* We don't really need this?????? Call from sensor.c directly -DC */
//}

// for unit testing, comment out if need be because we can only
// have one main
/*int main(int argc, char *argv[]) {
}*/
