#ifndef CONTROL_H
#define CONTROL_H

// initialized Control pid and other instance variables
int Control_init();
int Control_reset();

// sets direction of movement
void Control_setDir(int direction);

// communicates with motors.c
// returns whether robot is at destination
int Control_update(double dt);

// Called by Kalman for control vector
double Control_getForward();
double Control_getRight();
double Control_getTheta();

#endif
