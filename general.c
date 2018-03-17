/*----------------------------------------------------------------------------*/
/* Basic robot motions and sensor reading.                                    */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include "inc/general.h"
#include "inc/sensors.h"
#include "inc/motors.h"

// moves a given distance c forwards
void forwards(int c) {

}

// moves left distance c
void left(int c) {

}

// moves right distance c
void right(int c) {

}

// moves backwards distance c
void back(int c) {

}

// reads distance from long
double checkDistanceFront() {

}

double checkDistanceBack() {

}

double checkDistanceLeft() {

}

double checkDistanceRight() {

}

// maintains robot in center of path
void maintainLR() {
    // calls check Distance Front and Back
}

void maintiainFB() {
    // calls check Distance Left and Right
}

// returns value read from sensor s
double sensor(int s) {
    // something to distinguish short and long sensors?
}


// checks if there is Wall in moving direction
int checkWall() {

}


// for unit testing, comment out if need be because we can only 
// have one main 
int main(int argc, char *argv[]) {
}
