#ifndef CONTROL_H
#define CONTROL_H

// initialized Controls2 pid and other instance variables
int Control_init();

// sets direction of movement
void Control_setDir(int dir);

// communicates with motors.c
// returns whether robot is at destination
int Control_update();

#endif
