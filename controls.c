#include <stdio.h>
#include "inc/pigpio.h"
#include "inc/motors.h"
#include "inc/constants.h"
#include "inc/pid.h"
#include "inc/kalman.h"
#include "inc/sensors.h"
#include "inc/general.h"

static PID_T pidLeftRight;
static PID_T pidUpDown;
static PID_T pidTheta;

static double destX;
static double destY;
static double currDir;

int controls_setup() {
    pidLeftRight = init(1.0, 10.0, 10.0);
    pidUpDown = init(1.0, 10.0, 10.0);
    pidTheta = init(1.0, 10.0, 10.0);
    setPoint(pidUpDown, 0.0);
    setPoint(pidLeftRight, 0.0);
    setPoint(pidTheta, 0.0);
    destX = SQUARE_SIZE / 2;
    destY = SQUARE_SIZE / 2;
    currDir = 0;
    return 0;
}

int findDirection(int oldNode, int newNode) {
    int oldRow = oldNode / mazeSize;
    int oldCol = oldNode % mazeSize;
    int newRow = newNode / mazeSize;
    int newCol = newNode % mazeSize;

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

int update() {
    double currX = kalman_getX();
    double currY = kalman_getY();
    return abs(destX - currX) < 0.1 && abs(destY - currY) < 0.1;
}

void stop() {
    double motors[4] = {0.0};
    Motor_set(motors);
    reset(pidUpDown);
    reset(pidLeftRight);
    reset(pidTheta);
}

void moveTo(int direction) {
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
void move() {
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
