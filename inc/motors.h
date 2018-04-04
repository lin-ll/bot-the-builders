#ifndef MOTOR_H
#define MOTOR_H

/* Motor Driver */

#define MOTOR_MAX_SPEED 100.0

/**
 * Initialize Motors, clears motor drivers.
 * Return 0 on Success.
 * Return 1 on Failure
 **/
int Motor_init(int pifd);

/* returns speed that is being used by the motor pin */
int Motor_get(int motorPin);

/* sets the speeds that should be used by motors */
void Motor_set(int *speeds);

/* sets the speed of a specific motor */
void Motor_adjust(int forwardMotor, int backwardMotor, int speed);

// ensures motors are spinning at right speed.
void Motor_updateMotors(double dt);

// resets pid controllers.
void Motor_resetPID();

// sets all motors to 0
void Motor_off();

// bring all motors to a complete stop
// also resets PID, no reason the old integral should carry over
void Motor_completeStop();

/* Any Cleanup */
void Motor_free();

#endif
