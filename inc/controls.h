#ifndef CONTROL_H
#define CONTROL_H

// initialized Control pid and other instance variables
int Control_init();

// sets direction of movement
<<<<<<< HEAD
void Control_setDir(int dir);
=======
void Control_setDir(int direction);
>>>>>>> 75caa19c1c44f8b39767d7f1e00b4b01066077aa

// communicates with motors.c
// returns whether robot is at destination
int Control_update();

#endif
