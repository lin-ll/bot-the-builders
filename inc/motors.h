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
double Motor_getUpperLeft();
double Motor_getUpperRight();
double Motor_getLowerLeft();
double Motor_getLowerRight();

/* takes a value "left" or "right" which should range from -100 to 100,
   and is a percent of maximum power (negative is backwards) */
int Motor_setUpperLeft(double upperLeft);
int Motor_setUpperRight(double upperRight);
int Motor_setLowerLeft(double lowerLeft);
int Motor_setLowerRight(double lowerRight);

int Motor_adjustUpperLeft(double upperLeft);
int Motor_adjustUpperRight(double upperRight);
int Motor_adjustLowerLeft(double lowerLeft);
int Motor_adjustLowerRight(double lowerRight);

// ensures motors are spinning at right speed.
int update();

// resets pid controllers.
int reset();

/* Utility Function, to input array */
int Motor_set(double* motors);

/* Any Cleanup */
void Motor_free();


#endif
