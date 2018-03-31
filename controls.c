#include <stdio.h>
#include "pigpio.h"
#include "motors.h"
#include "constants.h"
#include "pid.h"
#include "kalman.h"
#include "sensors.h"
#include "general.h"

static PID_T pidLeftRight;
static PID_T pidUpDown;
static PID_T pidTheta;

static double destX;
static double destY;
static double currDir;

int Control_init() {
	pidLeftRight = Pid_init(1.0, 10.0, 10.0);
	pidUpDown = Pid_init(1.0, 10.0, 10.0);
	pidTheta = Pid_init(1.0, 10.0, 10.0);
	Pid_setPoint(pidUpDown, 0.0);
	Pid_setPoint(pidLeftRight, 0.0);
	Pid_setPoint(pidTheta, 0.0);
	destX = SQUARE_SIZE / 2;
	destY = SQUARE_SIZE / 2;
	currDir = 0;
	return 0;
}

int Control_findDirection(int oldNode, int newNode) {
	int oldRow = oldNode / MAZE_SIZE;
	int oldCol = oldNode % MAZE_SIZE;
	int newRow = newNode / MAZE_SIZE;
	int newCol = newNode % MAZE_SIZE;

	if (newRow - oldRow == 1) {
		return DOWN; // down
	}
	if (newRow - oldRow == -1) {
		return UP; // up
	}
	if (newCol - oldCol == 1) {
		return RIGHT; // right
	}
	if (newCol - oldCol == -1) {
		return LEFT; // left
	}

	// not any direction??? TODO: check this
	return -1;
}

int Control_update() {
	double currX = kalman_getX();
	double currY = kalman_getY();
	return abs(destX - currX) < 0.1 && abs(destY - currY) < 0.1;
}

void Control_stop() {
	Motor_setUpperLeft(0.0);
	Motor_setUpperRight(0.0);
	Motor_setLowerLeft(0.0);
	Motor_setLowerRight(0.0);
	
	Pid_reset(pidUpDown);
	Pid_reset(pidLeftRight);
	Pid_reset(pidTheta);
}

void Control_moveTo(int direction) {
	if (direction == UP) {
		destY -= SQUARE_SIZE;
		currDir = UP;
	} else if (direction == LEFT) {
		destX -= SQUARE_SIZE;
		currDir = LEFT;
	} else if (direction == RIGHT) {
		destX += SQUARE_SIZE;
		currDir = RIGHT;
	} else  if (direction == DOWN) {
		destY += SQUARE_SIZE;
		currDir = DOWN;
	}
}
void Control_move() {
	if (currDir == UP) {
		forward();
		maintainLR(destX);
		maintainTheta();
	} else if (currDir == LEFT) {
		left();
		maintainFB(destY);
		maintainTheta();
	} else if (currDir == RIGHT) {
		right();
		maintainFB(destY);
		maintainTheta();
	} else if (currDir == DOWN) {
		back();
		maintainLR(destX);
		maintainTheta();
	}
}
