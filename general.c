/*----------------------------------------------------------------------------*/
/* Basic robot motions and sensor reading.                                    */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include "inc/general.h"
#include "inc/motors.h"
#include "inc/sensors.h"
#include "inc/kalman.h"

// WRITING UNDER THE ASSUMED FRAMEWORK:
/* 
	1. kalman.c processes information from sensors.h and should pass information
	2. this file will then process the movement, checking back with kalman.c, 
		maybe even with sensors.c
	3. Communicate with motors.c to move the omniwheels
*/

// moves a given distance c forwards
void forwards(int c) {
	double currLeft, currRight, finalDist;

	finalDist = <insert current location method>;
	// one thing to note is deceleration, so we can't really stop at finalDist
	// we have to start slowing down at some point before finalDist (finalDist-offset)
	while (<insert condition from kalman> < finalDist - offset) {
		Motor_setUpperLeft(someVal);
		Motor_setUpperRight(someVal);
	}
	// start slowing down
	while (<insert condition from kalman> < finalDist) {
		currLeft = Motor_getUpperLeft();
		currRight = Motor_getUpperRight();
		Motor_setUpperLeft(currLeft - amount);
		Motor_setUpperRight(currRight - amount);
	}
}

// moves left distance c
void left(int c) {
	double currLeftT, currLeftB, finalDist;

	finalDist = <insert current location method>;
	// one thing to note is deceleration, so we can't really stop at finalDist
	// we have to start slowing down at some point before finalDist (finalDist-offset)
	while (<insert condition from kalman> < finalDist - offset) {
		Motor_setUpperLeft(someVal);
		Motor_setBottomLeft(someVal);
	}
	// start slowing down
	while (<insert condition from kalman> < finalDist) {
		currLeftT = Motor_getUpperLeft();
		currLeftB = Motor_getBottomLeft();
		Motor_setUpperLeft(currLeftT - amount);
		Motor_setBottomLeft(currLeftB - amount);
	}
}

// moves right distance c
void right(int c) {
	double currRightT, currRightB, finalDist;

	finalDist = <insert current location method>;
	// one thing to note is deceleration, so we can't really stop at finalDist
	// we have to start slowing down at some point before finalDist (finalDist-offset)
	while (<insert condition from kalman> < finalDist - offset) {
		Motor_setUpperRight(someVal);
		Motor_setLowerRight(someVal);
	}
	// start slowing down
	while (<insert condition from kalman> < finalDist) {
		currRightT = Motor_getUpperRight();
		currRightB = Motor_getLowerRight();
		Motor_setUpperRight(currRightT - amount);
		Motor_setLowerRight(currRightB - amount);
	}
}

// moves backwards distance c
void back(int c) {
	double currLeft, currRight, finalDist;

	finalDist = <insert current location method>;
	// one thing to note is deceleration, so we can't really stop at finalDist
	// we have to start slowing down at some point before finalDist (finalDist-offset)
	while (<insert condition from kalman> < finalDist - offset) {
		Motor_setLowerLeft(someVal);
		Motor_setLowerRight(someVal);
	}
	// start slowing down
	while (<insert condition from kalman> < finalDist) {
		currLeft = Motor_getLowerLeft();
		currRight = Motor_getLowerRight();
		Motor_setLowerLeft(currLeft - amount);
		Motor_setLowerRight(currRight - amount);
	}
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
void maintainLR() {
    // calls check Distance Front and Back
}

void maintainFB() {
    // calls check Distance Left and Right
}

// returns value read from sensor s
//double sensor(int s) {
    // something to distinguish short and long sensors?
    /* We don't really need this?????? Call from sensor.c directly -DC */
//}


// checks if there is Wall in moving direction
int checkWall() {

}


// for unit testing, comment out if need be because we can only 
// have one main 
/*int main(int argc, char *argv[]) {
}*/
