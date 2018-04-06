/*----------------------------------------------------------------------------*/
/* kalman.h 																  */
/* Controls the Kalman filter for position and orientation of the robot       */
/*----------------------------------------------------------------------------*/

#ifndef KALMAN_H
#define KALMAN_H

/* Kalman filter for estimating position */

// THIS NEEDS SOME MORE POLISHING ~DC
void Kalman_init();

/**
 * update with new sensor values
 * send 8 distance sensors, 4 encoders, accx / accy / gyro,
 * attempted vx/vy/spin from control, address to write position
 **/
void Kalman_update_given_sensors(double dt, double *encoders, double *distances, double gyro, double compass, double *control);
void Kalman_update(double dt);

// x position/velocity in mm with the corner of the maze 0
double Kalman_getX();
double Kalman_getVx();

// y position/velocity in mm with the corner of the maze 0
double Kalman_getY();
double Kalman_getVy();

// robot's current rotation/velocity (theta) in radians
double Kalman_getT();
double Kalman_getVt();


// temp for matrix test
void mat_mult(double *A, double *B, double *dest);



#endif
