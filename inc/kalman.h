/*----------------------------------------------------------------------------*/
/* kalman.h 																  */
/* Controls the Kalman filter for position and orientation of the robot       */
/*----------------------------------------------------------------------------*/

#ifndef KALMAN_H
#define KALMAN_H

/* Kalman filter for estimating position */

// THIS NEEDS SOME MORE POLISHING ~DC
void init();

/**
 * update with new sensor values
 * send 8 distance sensors, 4 encoders, accx / accy / gyro,
 * attempted vx/vy/spin from control, address to write position
 **/
void update(double *distances, double *encoders, double *imu, double *control, double *pos);

// x position/velocity in mm with the corner of the maze 0
double kalman_getX();
double kalman_getVx();

// y position/velocity in mm with the corner of the maze 0
double kalman_getY();
double kalman_getVy();

// robot's current rotation/velocity (theta) in radians
double kalman_getT();
double kalman_getVt();



#endif
