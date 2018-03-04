/* motors.c */

#include <motors.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pigpiod_if2.h>

//-------------------

#define UPPER_LEFT_PIN 1
#define UPPER_RIGHT_PIN 2
#define LOWER_LEFT_PIN 3
#define LOWER_RIGHT_PIN 4
#define UPPER_LEFT_FORWARD 5
#define UPPER_RIGHT_FORWARD 6
#define UPPER_LEFT_BACKWARD 7
#define UPPER_RIGHT_BACKWARD 8
#define LOWER_LEFT_FORWARD 9
#define LOWER_RIGHT_FORWARD 10
#define LOWER_LEFT_BACKWARD 11
#define LOWER_RIGHT_BACKWARD 12
#define MOTOR_RANGE 255

//-------------------

static double upperLeftPercent = 0;
static double upperRightPercent = 0;
static double lowerLeftPercent = 0;
static double lowerRightPercent = 0;

static int pi;

int Motor_init(int pifd) {
  pi = pifd;
  upperLeftPercent = 0;
  upperRightPercent = 0;
  lowerLeftPercent = 0;
  lowerRightPercent = 0;

  gpio_write(pi, UPPER_LEFT_FORWARD, 0);
  gpio_write(pi, UPPER_LEFT_BACKWARD, 0);
  gpio_write(pi, UPPER_RIGHT_FORWARD, 0);
  gpio_write(pi, UPPER_RIGHT_BACKWARD, 0);
  gpio_write(pi, LOWER_LEFT_FORWARD, 0);
  gpio_write(pi, LOWER_LEFT_BACKWARD, 0);
  gpio_write(pi, LOWER_RIGHT_FORWARD, 0);
  gpio_write(pi, LOWER_RIGHT_BACKWARD, 0);

  set_PWM_range(pi, UPPER_LEFT_PIN, MOTOR_RANGE);
  set_PWM_range(pi, UPPER_RIGHT_PIN, MOTOR_RANGE);
  set_PWM_range(pi, LOWER_LEFT_PIN, MOTOR_RANGE);
  set_PWM_range(pi, LOWER_RIGHT_PIN, MOTOR_RANGE);
  return 0;
}

double Motor_getUpperLeft() {
  return upperLeftPercent;
}

double Motor_getUpperRight() {
  return upperRightPercent;
}

double Motor_getLowerRight() {
  return lowerLeftPercent;
}

double Motor_getUpperRight() {
  return lowerRightPercent;
}

int Motor_setUpperLeft(double upperLeft) {
  upperLeftPercent = upperLeft;
  if (upperLeft >= 0) {
    gpio_write(pi, UPPER_LEFT_BACKWARD, 0);
    gpio_write(pi, UPPER_LEFT_FORWARD, 1);
    set_PWM_dutycycle(pi, UPPER_LEFT_PIN, (upperLeft * MOTOR_RANGE / 100.0));
  }
  else {
    gpio_write(pi, UPPER_LEFT_FORWARD, 0);
    gpio_write(pi, UPPER_LEFT_BACKWARD, 1);
    set_PWM_dutycycle(pi, UPPER_LEFT_PIN, (upperLeft * MOTOR_RANGE / -100.0));
  }
  return 0;
}

int Motor_setUpperRight(double upperRight) {
  upperRightPercent = upperRight;
  if (upperRight >= 0) {
    gpio_write(pi, UPPER_RIGHT_BACKWARD, 0);
    gpio_write(pi, UPPER_RIGHT_FORWARD, 1);
    set_PWM_dutycycle(pi, UPPER_RIGHT_PIN, (upperRight * MOTOR_RANGE / 100.0));
  }
  else {
    gpio_write(pi, UPPER_RIGHT_FORWARD, 0);
    gpio_write(pi, UPPER_RIGHT_BACKWARD, 1);
    set_PWM_dutycycle(pi, UPPER_RIGHT_PIN, (upperRight * MOTOR_RANGE / -100.0));
  }
  return 0;
}

int Motor_setLowerLeft(double lowerLeft) {
  lowerLeftPercent = lowerLeft;
  if (lowerLeft >= 0) {
    gpio_write(pi, LOWER_LEFT_BACKWARD, 0);
    gpio_write(pi, LOWER_LEFT_FORWARD, 1);
    set_PWM_dutycycle(pi, LOWER_LEFT_PIN, (lowerLeft * MOTOR_RANGE / 100.0));
  }
  else {
    gpio_write(pi, LOWER_LEFT_FORWARD, 0);
    gpio_write(pi, LOWER_LEFT_BACKWARD, 1);
    set_PWM_dutycycle(pi, LOWER_LEFT_PIN, (lowerLeft * MOTOR_RANGE / -100.0));
  }
  return 0;
}

int Motor_setLowerRight(double lowerRight) {
  lowerRightPercent = lowerRight;
  if (lowerRight >= 0) {
    gpio_write(pi, LOWER_RIGHT_BACKWARD, 0);
    gpio_write(pi, LOWER_RIGHT_FORWARD, 1);
    set_PWM_dutycycle(pi, LOWER_RIGHT_PIN, (lowerRight * MOTOR_RANGE / 100.0));
  }
  else {
    gpio_write(pi, LOWER_RIGHT_FORWARD, 0);
    gpio_write(pi, LOWER_RIGHT_BACKWARD, 1);
    set_PWM_dutycycle(pi, LOWER_RIGHT_PIN, (lowerRight * MOTOR_RANGE / -100.0));
  }
  return 0;
}

/* Utility Function, to input array */
int Motor_set(int* motors) {
  int ret = Motor_setUpperLeft(motors[0]);
  ret |= Motor_setUpperight(motors[1]);
  ret |= Motor_setLowerLeft(motors[2]);
  ret |= Motor_setLowerRight(motors[3]);
  return ret;
}

void Motor_free() {

}
