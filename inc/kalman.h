#ifndef KALMAN_H
#define KALMAN_H

/* Kalman filter for estimating position */


void init();

/**
 * update with new sensor values
 * send 8 distance sensors, 4 encoders, accx / accy / gyro,
 * attempted vx/vy/spin from control, address to write position
 **/
void update(double *distances, double *encoders, double *imu, double *control, double *pos);



#endif