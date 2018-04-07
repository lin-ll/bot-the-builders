#include "motors.h"
#include <pigpiod_if2.h>
#include "sensors.h"
#include "constants.h"

//-------------------

const int FORWARD_PINS[4] = {UPPER_LEFT_FORWARD, UPPER_RIGHT_FORWARD, LOWER_LEFT_FORWARD, LOWER_RIGHT_FORWARD};
const int BACKWARD_PINS[4] = {UPPER_LEFT_BACKWARD, UPPER_RIGHT_BACKWARD, LOWER_LEFT_BACKWARD, LOWER_RIGHT_BACKWARD};

static int desiredSpeeds[4];
static int pi;

//const double MOTOR_CORRECTION[4] = {1,1,1,1};
const double MOTOR_CORRECTION[4] = {0.60, 1.0, 1.0, .85};

int Motor_init(int pifd) {
  pi = pifd;

  for (int i = 0; i < 4; i++) {
    desiredSpeeds[i] = 0;
    set_PWM_dutycycle(pi, FORWARD_PINS[i], 0);
    set_PWM_dutycycle(pi, BACKWARD_PINS[i], 0);
    set_PWM_range(pi, FORWARD_PINS[i], MOTOR_RANGE);
    set_PWM_range(pi, BACKWARD_PINS[i], MOTOR_RANGE);
  }
  return 0;
}

int Motor_get(int motorPin) {
  int idx = 0;
  while (FORWARD_PINS[idx] != motorPin && BACKWARD_PINS[idx] != motorPin) {
    idx++;
  }
  return desiredSpeeds[idx];
}

void Motor_adjust(int forwardMotor, int backwardMotor, int speed) {
  if (speed >= 0) {
    set_PWM_dutycycle(pi, forwardMotor, speed);
    set_PWM_dutycycle(pi, backwardMotor, 0);
  } else {
    set_PWM_dutycycle(pi, forwardMotor, 0);
    set_PWM_dutycycle(pi, backwardMotor, -speed);
  }
}

void Motor_set(int* speeds) {
  for (int i = 0; i < 4; i++) {
    desiredSpeeds[i] = speeds[i];
    Motor_adjust(FORWARD_PINS[i], BACKWARD_PINS[i], MOTOR_CORRECTION[i]*speeds[i]);
  }
}

void Motor_off() {
  for (int i = 0; i < 4; i++) {
    Motor_adjust(FORWARD_PINS[i], BACKWARD_PINS[i], 0);
  }
}

// bring all motors to a complete stop
void Motor_completeStop(){
  int speeds[4] = {0, 0, 0, 0};
  Motor_set(speeds);
  Motor_off();
}
