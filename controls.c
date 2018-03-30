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

int setup() {
    pidLeftRight = init(1.0, 10.0, 10.0);
    pidUpDown = init(1.0, 10.0, 10.0);
    pidTheta = init(1.0, 10.0, 10.0);
    setPoint(pidUpDown, 0.0);
    setPoint(pidLeftRight, 0.0);
    setPoint(pidTheta, 0.0);
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

void stop() {
    //TODO: Add braking
    double motors[4] = {0.0};
    Motor_set(motors);
    reset(pidUpDown);
    reset(pidLeftRight);
    reset(pidTheta);
}

// assuming forward orientation, positive translation defined to the right,
// and positive rotations defined in clockwise direction
void adjust(int direction, double mainVelocity, double translate,
    double rotate) {
    double motors[4] = {rotate};
    motors[direction] += mainVelocity - translate;
    motors[(direction + 1) % 4] += mainVelocity + translate;
    motors[(direction + 2) % 4] += translate - mainVelocity;
    motors[(direction + 3) % 4] += - mainVelocity - translate;
}

/* following method simply uses PID to move the robot
void pidCorrection {
    double xError = xOffset(newNode);
    double yError = yOffset(newNode);
    double thetaError = thetaOffset();
    double translate = 0;
    double rotate = 0;
    while (abs(xError) > 0.1 || abs(yError) > 0.1) {
    }
}
*/

double xOffset(int goalSpace) {
    // target stores ideal x position where furthest wall is 0 in mm.
    int targetRow = goalSpace / mazeSize;
    double target = 180.0 * targetRow + 90.0;
    double currX = kalmanX(); //TODO: calls kalman method to get X position
    return target - currX;
}

double yOffset(int goalSpace) {
    // same as xOffset
    int targetCol = goalSpace % mazeSize;
    double target = 180.0 * targetCol + 90.0;
    double currY = kalmanY(); //TODO: calls kalman method to get Y position
    return target - currY;
}

double thetaOffset() {
    return Sensor_getGyro();
}

double getStoppingDistance() {
    return 0.0;
}

void moveTo(int oldNode, int newNode) {
    int direction = findDirection(oldNode, newNode);
    double togo == 180.0;
    if (direction == UP) {
        togo = yOffset(newNode);
        while (togo < 0.0) {
            forward(2.0);
            maintainLR(newNode);
            maintainTheta();
        }
    } else if (direction == LEFT) {
        togo = xOffset(newNode);
        while (togo > 0.0) {
            left(2.0);
            maintainFB(newNode);
            maintainTheta();
        }
    } else if (direction == RIGHT) {
        togo = xOffset(newNode);
        while (togo < 0.0) {
            right(2.0);
            maintainFB(newNode);
            maintainTheta();
        }
    } else {
        togo = yOffset(newNode);
        while (togo > 0.0) {
            back(2.0);
            maintainLR(newNode);
            maintainTheta();
        }
    }
    // double xError = xOffset(newNode);
    // double yError = yOffset(newNode);
    // double thetaError = thetaOffset();
    // double translate = 0;
    // double rotate = 0;
    // double stoppingDistance = getStoppingDistance();
    // if (direction  % 2 == 0) {
    //     // moving Left-Right, so pidUpDown to stay straight
    //     while (xError > stoppingDistance) {
    //         update(pidUpDown, yError, 0.1);
    //         translate = getVal(pidUpDown);
    //         rotate = getVal(pidTheta);
    //         double mainVelocity = 50.0;
    //         adjust(direction, mainVelocity, translate, rotate);
    //     }
    // } else {
    //     while (yError > stoppingDistance) {
    //       update(pidLeftRight, xError, 0.1);
    //       translate = getVal(pidLeftRight);
    //       rotate = getVal(pidTheta);
    //       double mainVelocity = 50.0;
    //       adjust(direction, mainVelocity, translate, rotate);
    //     }
    // }
}
