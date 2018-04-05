#include "motors.h"
#include <pigpiod_if2.h>
#include "pid.h"
#include "sensors.h"
#include "constants.h"

//-------------------

const int FORWARD_PINS[4] = {UPPER_LEFT_FORWARD, UPPER_RIGHT_FORWARD, LOWER_LEFT_FORWARD, LOWER_RIGHT_FORWARD};
const int BACKWARD_PINS[4] = {UPPER_LEFT_BACKWARD, UPPER_RIGHT_BACKWARD, LOWER_LEFT_BACKWARD, LOWER_RIGHT_BACKWARD};

static int desiredSpeeds[4];
static PID_T pids[4];
static int pi;

int Motor_init(int pifd) {
  pi = pifd;

  for (int i = 0; i < 4; i++) {
    desiredSpeeds[i] = 0;
    pids[i] = Pid_init(1.0, 0.0, 0.0);
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

/* COmmented out as long as we don't have encoders working
void Motor_updateMotors(double dt) {
  // TODO: Sensor_getMotorSpeed, what is this
  for (int i = 0; i < 4; i++) {
    PID_T currPid = pids[i];
    int forwardMotor = FORWARD_PINS[i];
    int backwardMotor = BACKWARD_PINS[i];
    Pid_setPoint(currPid, desiredSpeeds[i]);
    double encoderSpeed = Sensor_getMotorSpeed(forwardMotor);
    Pid_update(currPid, encoderSpeed, dt);
    double adjustSpeedDouble = Pid_getVal(currPid);
    int adjustSpeed = (int) (adjustSpeedDouble * MOTOR_RANGE / MOTOR_MAX_SPEED); // TODO: unsure if this is correct
    Motor_adjust(forwardMotor, backwardMotor, adjustSpeed);
  }
}
*/

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
    set_PWM_dutycycle(pi, backwardMotor, -speed);
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
    Motor_adjust(FORWARD_PINS[i], BACKWARD_PINS[i], 0);
  }
}

// bring all motors to a complete stop
// also resets PID, no reason the old integral should carry over
void Motor_completeStop(){
  Motor_resetPID();
  int speeds[4] = {0,0,0,0};
  Motor_set(speeds);
  Motor_off();
}

void Motor_free() {

}
