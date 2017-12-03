#ifndef MOTOR_H
#define MOTOR_H

/* Motor Driver */

/**
 * Initialize Motors, clears motor drivers.
 * Return 0 on Success.
 * Return 1 on Failure
 **/
int Motor_init(int pifd);

/* returns the percent of maximum power that is being used
   by each motor */
double Motor_getLeft();
double Motor_getRight();

/* takes a value "left" or "right" which should range from -100 to 100, 
   and is a percent of maximum power (negative is backwards) */
int Motor_setLeft(double left);

int Motor_setRight(double right);

/* Utility Function, to input array */
int Motor_set(int* motors);

/* Any Cleanup */
void Motor_free();


#endif
