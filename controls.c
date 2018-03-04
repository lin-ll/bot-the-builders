// controls

#include <stdio.h>
#include <pigpio.h>

int mazeSize = 16;

int findDirection(int oldNode, int newNode) {
    int oldRow = oldNode / mazeSize;
    int oldCol = oldNode % mazeSize;
    int newRow = newNode / mazeSize;
    int newCol = newNode % mazeSize;

    if (newRow - oldRow == 1) {
        return 2; // down
    }
    if (newRow - oldRow == -1) {
        return 0; // up
    }
    if (newCol - oldCol == 1) {
        return 3; // right
    }
    if (newCol - oldCol == -1) {
        return 1; // left
    }
}

void moveTo(int oldNode, int newNode, int prevDirection) {
    int direction = findDirection(oldNode, newNode);
    if (direction == prevDirection) {
        // dont't change anything
    }
    if ((direction - prevDirection) % 4 == 1) {
        stop();
        turn(-90);
        move(1);
    }
    if ((direction - prevDirection) % 4 == 2) {
        stop();
        turn(180);
        move(1);
    }
    if ((direction - prevDirection) % 4 == 3) {
        stop();
        turn(90);
        move(1);
    }
}

void stop() {

}

void turn(int degrees) {

}

void move(int distance, ) {

}
