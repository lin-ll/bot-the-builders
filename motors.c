/* motors.c */

#include "motors.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pigpiod_if2.h>
#include "pid.h"
#include "sensors.h"
#include "constants.h"

//-------------------

const int UPPER_LEFT_IDX = 0;
const int UPPER_RIGHT_IDX = 1;
const int LOWER_LEFT_IDX = 2;
const int LOWER_RIGHT_IDX = 3;
const int FORWARD[4] = {UPPER_LEFT_FORWARD, UPPER_RIGHT_FORWARD, LOWER_LEFT_FORWARD, LOWER_RIGHT_FORWARD};
const int BACKWARD[4] = {UPPER_LEFT_BACKWARD, UPPER_RIGHT_BACKWARD, LOWER_LEFT_BACKWARD, LOWER_RIGHT_BACKWARD};

static int desiredSpeeds[4] = {0, 0, 0, 0};
static PID_T pids[4];

static int pi;

int Motor_init(int pifd) {
  pi = pifd;
  desiredSpeeds[4] = {0, 0, 0, 0};

  for (int i = 0; i < 4; i++) {
    pids[i] = Pid_init(10.0, 10.0, 1.0);
    set_PWM_dutycycle(pi, FORWARD[i], 0);
    set_PWM_dutycycle(pi, BACKWARD[i], 0);
    set_PWM_range(pi, FORWARD[i], MOTOR_RANGE);
    set_PWM_range(pi, BACKWARD[i], MOTOR_RANGE);
  }
  return 0;
}

// Assume sensors.c has a method witch returns current speed of motor
// Call this motor motorSpeed();

int Motor_get(int motorPin) {
  switch (motorPin) {
    case UPPER_LEFT_FORWARD:
    case UPPER_LEFT_BACKWARD:
      return desiredSpeeds[UPPER_LEFT_IDX];
    case UPPER_RIGHT_FORWARD:
    case UPPER_RIGHT_BACKWARD:
      return desiredSpeeds[UPPER_RIGHT_IDX];
    case LOWER_LEFT_FORWARD:
    case LOWER_LEFT_BACKWARD:
      return desiredSpeeds[LOWER_LEFT_IDX];
    case LOWER_RIGHT_FORWARD:
    case LOWER_RIGHT_BACKWARD:
      return desiredSpeeds[LOWER_RIGHT_IDX];
  }
  return -1;
}

void Motor_updateMotors(double dt) {
  // TODO: Sensor_getMotorSpeed, what is this
  for (int i = 0; i < 4; i++) {
    PID_T currPid = pids[i];
    int forwardMotor = FORWARD[i];
    int backwardMotor = BACKWARD[i];
    Pid_setPoint(currPid, desiredSpeeds[i]);
    double encoderSpeed = Sensor_getMotorSpeed(forwardMotor);
    Pid_update(currPid, encoderSpeed, dt);
    double adjustSpeedDouble = Pid_getVal(currPid);
    int adjustSpeed = (int) (adjustSpeedDouble * MOTOR_RANGE / 100.0); // TODO: unsure if this is correct
    Motor_adjust(forwardMotor, backwardMotor, adjustSpeed);
  }
}

void Motor_resetPID() {
  for (int i = 0; i < 4; i++) {
    Pid_reset(pids[i]);
  }
}


void Motor_adjust(int forwardMotor, int backwardMotor, int speed) {
  if (speed >= 0) {
    set_PWM_dutycycle(pi, forwardMotor, speed);
    set_PWM_dutycycle(pi, backwardMotor, 0);
  } else {
    set_PWM_dutycycle(pi, forwardMotor, 0);
    set_PWM_dutycycle(pi, backwardMotor, speed);
  }
}

/* Utility Function, to input array */
void Motor_set(int* speeds) {
  for (int i = 0; i < 4; i++) {
    desiredSpeeds[i] = speeds[i];
  }
}

void Motor_off() {
  for (int i = 0; i < 4; i++) {
    Motor_adjust(FORWARD[i], BACKWARD[i], 0);
  }
}

void Motor_free() {

}
