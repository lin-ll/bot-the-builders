#ifndef CONTROL_H
#define CONTROL_H

// initialized Control pid and other instance variables
int Control_init();

// sets direction of movement
void Control_setDir(int direction);

// communicates with motors.c
// returns whether robot is at destination
int Control_update(double dt);

#endif
