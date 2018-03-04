#include <stdio.h>
#include <pigpio.h>
#include <motors.h>

int mazeSize = 16;

int findDirection(int oldNode, int newNode) {
    int oldRow = oldNode / mazeSize;
    int oldCol = oldNode % mazeSize;
    int newRow = newNode / mazeSize;
    int newCol = newNode % mazeSize;

    if (newRow - oldRow == 1) {
        return 1; // down
    }
    if (newRow - oldRow == -1) {
        return 3; // up
    }
    if (newCol - oldCol == 1) {
        return 0; // right
    }
    if (newCol - oldCol == -1) {
        return 2; // left
    }
}

void stop() {
    double motors[4] = {0.0};
    Motor_set(motors);
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

void moveTo(int oldNode, int newNode) {
    int direction = findDirection(oldNode, newNode);
    double mainVelocity = 100.0;
    double translate = 10.0;
    double rotate = 5.0;
    adjust(direction, mainVelocity, translate, rotate);
}
