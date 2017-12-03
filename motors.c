/* motors.c */

#include <motors.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pigpiod_if2.h>

//-------------------

#define LEFT_MOTOR_PIN 12 // PWM channel 0
#define RIGHT_MOTOR_PIN 13 // PWM channel 1
#define LEFT_FORWARD 16
#define LEFT_BACKWARD 25
#define RIGHT_FORWARD 6
#defineRIGHT_BACKWARD 5
#define MOTOR_RANGE 255

//-------------------

static double leftPercent = 0;
static double rightPercent = 0;

static int pi;

int Motor_init(int pifd) {
  pi = pifd;
  leftPercent = 0;
  rightPercent = 0;
  gpio_write(pi, LEFT_FORWARD, 0);
  gpio_write(pi, LEFT_BACKWARD, 0);
  gpio_write(pi, RIGHT_FORWARD, 0);
  gpio_write(pi, RIGHT_BACKWARD, 0);
  gpio_write(pi, LEFT_MOTOR_PIN, 0);
  gpio_write(pi, RIGHT_MOTOR_PIN, 0);
  set_PWM_range(pi, LEFT_MOTOR_PIN, MOTOR_RANGE);
  set_PWM_range(pi, RIGHT_MOTOR_PIN, MOTOR_RANGE);
  return 0;
}

double Motor_getLeft() {
  return leftPercent;
}

double Motor_getRight() {
  return rightPercent;
}


int Motor_setLeft(double left) {
  leftPercent = left;
  if (left >= 0) {
    gpio_write(pi, LEFT_BACKWARD, 0);
    gpio_write(pi, LEFT_FORWARD, 1);
    set_PWM_dutycycle(pi, LEFT_MOTOR_PIN, (left * MOTOR_RANGE / 100.0));
  }
  else {
    gpio_write(pi, LEFT_FORWARD, 0);
    gpio_write(pi, LEFT_BACKWARD, 1);
    set_PWM_dutycycle(pi, LEFT_MOTOR_PIN, (left * MOTOR_RANGE / -100.0));
  }
  return 0;
}

int Motor_setRight(double right) {
  rightPercent = right;
  if (right >= 0) {
    gpio_write(pi, RIGHT_BACKWARD, 0);
    gpio_write(pi, RIGHT_FORWARD, 1);
    set_PWM_dutycycle(pi, RIGHT_MOTOR_PIN, (right * MOTOR_RANGE / 100.0));
  }
  else {
    gpio_write(pi, RIGHT_FORWARD, 0);
    gpio_write(pi, RIGHT_BACKWARD, 1);
    set_PWM_dutycycle(pi, RIGHT_MOTOR_PIN, (right * MOTOR_RANGE / -100.0));
  }
  return 0;
}

/* Utility Function, to input array */
int Motor_set(int* motors) {
  int ret = Motor_setLeft(motors[0]);
  ret |= Motor_setRight(motors[1]);
  return ret;
}

void Motor_free() {

}
