/* motors.c */

#include "inc/motors.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pigpiod_if2.h>
#include "inc/pid.h"
#include "inc/sensors.h"
#include "inc/constants.h"

//-------------------

static double upperLeftPercent = 0;
static double upperRightPercent = 0;
static double lowerLeftPercent = 0;
static double lowerRightPercent = 0;

static PID_T pidUpperLeft;
static PID_T pidUpperRight;
static PID_T pidLowerLeft;
static PID_T pidLowerRight;

static int pi;

int Motor_init(int pifd) {
  pi = pifd;
  upperLeftPercent = 0;
  upperRightPercent = 0;
  lowerLeftPercent = 0;
  lowerRightPercent = 0;

  pidUpperLeft = init(10.0, 10.0, 1.0);
  pidUpperRight = init(10.0, 10.0, 1.0);
  pidLowerLeft = init(10.0, 10.0, 1.0);
  pidLowerRight = init(10.0, 10.0, 1.0);

  set_PWM_dutycycle(pi, UPPER_LEFT_FORWARD, 0.0);
  set_PWM_dutycycle(pi, UPPER_LEFT_BACKWARD, 0.0);
  set_PWM_dutycycle(pi, UPPER_RIGHT_FORWARD, 0.0);
  set_PWM_dutycycle(pi, UPPER_RIGHT_BACKWARD, 0.0);
  set_PWM_dutycycle(pi, LOWER_LEFT_FORWARD, 0.0);
  set_PWM_dutycycle(pi, LOWER_LEFT_BACKWARD, 0.0);
  set_PWM_dutycycle(pi, LOWER_RIGHT_FORWARD, 0.0);
  set_PWM_dutycycle(pi, LOWER_RIGHT_BACKWARD, 0.0);

  set_PWM_range(pi, UPPER_LEFT_FORWARD, MOTOR_RANGE);
  set_PWM_range(pi, UPPER_LEFT_BACKWARD, MOTOR_RANGE);
  set_PWM_range(pi, UPPER_RIGHT_FORWARD, MOTOR_RANGE);
  set_PWM_range(pi, UPPER_RIGHT_BACKWARD, MOTOR_RANGE);
  set_PWM_range(pi, LOWER_LEFT_FORWARD, MOTOR_RANGE);
  set_PWM_range(pi, LOWER_LEFT_BACKWARD, MOTOR_RANGE);
  set_PWM_range(pi, LOWER_RIGHT_FORWARD, MOTOR_RANGE);
  set_PWM_range(pi, LOWER_RIGHT_BACKWARD, MOTOR_RANGE);
  return 0;
}

// Assume sensors.c has a method witch returns current speed of motor
// Call this motor motorSpeed();

double Motor_getUpperLeft() {
  return upperLeftPercent;
}

double Motor_getUpperRight() {
  return upperRightPercent;
}

double Motor_getLowerLeft() {
  return lowerLeftPercent;
}

double Motor_getLowerRight() {
  return lowerRightPercent;
}

int Motor_setUpperLeft(double upperLeft) {
  upperLeftPercent = upperLeft;
  if (upperLeft >= 0) {
    set_PWM_dutycycle(pi, UPPER_LEFT_FORWARD, (upperLeft * MOTOR_RANGE / 100.0));
    set_PWM_dutycycle(pi, UPPER_LEFT_BACKWARD, 0.0);
  }
  else {
    set_PWM_dutycycle(pi, UPPER_LEFT_FORWARD, 0.0);
    set_PWM_dutycycle(pi, UPPER_LEFT_BACKWARD, (upperLeft * MOTOR_RANGE / -100.0));
  }
  return 0;
}

int Motor_adjustUpperLeft(double upperLeft) {
  if (upperLeft >= 0) {
    set_PWM_dutycycle(pi, UPPER_LEFT_FORWARD, (upperLeft * MOTOR_RANGE / 100.0));
    set_PWM_dutycycle(pi, UPPER_LEFT_BACKWARD, 0.0);
  }
  else {
    set_PWM_dutycycle(pi, UPPER_LEFT_FORWARD, 0.0);
    set_PWM_dutycycle(pi, UPPER_LEFT_BACKWARD, (upperLeft * MOTOR_RANGE / -100.0));
  }
  return 0;
}

int Motor_setUpperRight(double upperRight) {
  upperRightPercent = upperRight;
  if (upperRight >= 0) {
    set_PWM_dutycycle(pi, UPPER_RIGHT_FORWARD, (upperRight * MOTOR_RANGE / 100.0));
    set_PWM_dutycycle(pi, UPPER_RIGHT_BACKWARD, 0.0);
  }
  else {
    set_PWM_dutycycle(pi, UPPER_RIGHT_FORWARD, 0.0);
    set_PWM_dutycycle(pi, UPPER_RIGHT_BACKWARD, (upperRight * MOTOR_RANGE / -100.0));
  }
  return 0;
}

int Motor_adjustUpperRight(double upperRight) {
  if (upperRight >= 0) {
    set_PWM_dutycycle(pi, UPPER_RIGHT_FORWARD, (upperRight * MOTOR_RANGE / 100.0));
    set_PWM_dutycycle(pi, UPPER_RIGHT_BACKWARD, 0.0);
  }
  else {
    set_PWM_dutycycle(pi, UPPER_RIGHT_FORWARD, 0.0);
    set_PWM_dutycycle(pi, UPPER_RIGHT_BACKWARD, (upperRight * MOTOR_RANGE / -100.0));
  }
  return 0;
}

int Motor_setLowerLeft(double lowerLeft) {
  lowerLeftPercent = lowerLeft;
  if (lowerLeft >= 0) {
    set_PWM_dutycycle(pi, LOWER_LEFT_FORWARD, (lowerLeft * MOTOR_RANGE / 100.0));
    set_PWM_dutycycle(pi, LOWER_LEFT_BACKWARD, 0.0);
  }
  else {
    set_PWM_dutycycle(pi, LOWER_LEFT_FORWARD, 0.0);
    set_PWM_dutycycle(pi, LOWER_LEFT_BACKWARD, (lowerLeft * MOTOR_RANGE / -100.0));
  }
  return 0;
}

int Motor_adjustLowerLeft(double lowerLeft) {
  if (lowerLeft >= 0) {
    set_PWM_dutycycle(pi, LOWER_LEFT_FORWARD, (lowerLeft * MOTOR_RANGE / 100.0));
    set_PWM_dutycycle(pi, LOWER_LEFT_BACKWARD, 0.0);
  }
  else {
    set_PWM_dutycycle(pi, LOWER_LEFT_FORWARD, 0.0);
    set_PWM_dutycycle(pi, LOWER_LEFT_BACKWARD, (lowerLeft * MOTOR_RANGE / -100.0));
  }
  return 0;
}

int Motor_setLowerRight(double lowerRight) {
  lowerRightPercent = lowerRight;
  if (lowerRight >= 0) {
    set_PWM_dutycycle(pi, LOWER_RIGHT_FORWARD, (lowerRight * MOTOR_RANGE / 100.0));
    set_PWM_dutycycle(pi, LOWER_RIGHT_BACKWARD, 0.0);
  }
  else {
    set_PWM_dutycycle(pi, LOWER_RIGHT_FORWARD, 0.0);
    set_PWM_dutycycle(pi, LOWER_RIGHT_BACKWARD, (lowerRight * MOTOR_RANGE / -100.0));
  }
  return 0;
}

int Motor_adjustLowerRight(double lowerRight) {
  if (lowerRight >= 0) {
    set_PWM_dutycycle(pi, LOWER_RIGHT_FORWARD, (lowerRight * MOTOR_RANGE / 100.0));
    set_PWM_dutycycle(pi, LOWER_RIGHT_BACKWARD, 0.0);
  }
  else {
    set_PWM_dutycycle(pi, LOWER_RIGHT_FORWARD, 0.0);
    set_PWM_dutycycle(pi, LOWER_RIGHT_BACKWARD, (lowerRight * MOTOR_RANGE / -100.0));
  }
  return 0;
}

void updateMotors(double dt) {
  setPoint(pidUpperLeft, upperLeftPercent);
  double upperLeftSpeed = Sensor_getMotorSpeed(UPPER_LEFT_PIN);
  update(pidUpperLeft, upperLeftSpeed, dt); // what is dt defined as?
  double upperLeftAdjust = getVal(pidUpperLeft);
  Motor_adjustUpperLeft(upperLeftAdjust);

  setPoint(pidUpperRight, upperRightPercent);
  double upperRightSpeed = Sensor_getMotorSpeed(UPPER_RIGHT_PIN);
  update(pidUpperRight, upperRightSpeed, dt); // what is dt defined as?
  double upperRightAdjust = getVal(pidUpperRight);
  Motor_adjustUpperRight(upperRightAdjust);

  setPoint(pidLowerLeft, lowerLeftPercent);
  double lowerLeftSpeed = Sensor_getMotorSpeed(LOWER_LEFT_PIN);
  update(pidLowerLeft, lowerLeftSpeed, dt); // what is dt defined as?
  double lowerLeftAdjust = getVal(pidLowerLeft);
  Motor_adjustLowerLeft(lowerLeftAdjust);

  setPoint(pidLowerRight, lowerRightPercent);
  double lowerRightSpeed = Sensor_getMotorSpeed(LOWER_RIGHT_PIN);
  update(pidLowerRight, lowerRightSpeed, dt); // what is dt defined as?
  double lowerRightAdjust = getVal(pidLowerRight);
  Motor_adjustUpperLeft(lowerRightAdjust);
}

void resetPID() {
    reset(pidUpperLeft);
    reset(pidUpperRight);
    reset(pidLowerLeft);
    reset(pidLowerRight);
}

/* Utility Function, to input array */
int Motor_set(double* motors) {
  double ret = Motor_setUpperLeft(motors[0]);
  ret = ret || Motor_setUpperRight(motors[1]);
  ret = ret || Motor_setLowerRight(motors[2]);
  ret = ret || Motor_setLowerLeft(motors[3]);
  return ret;
}

void Motor_free() {

}
