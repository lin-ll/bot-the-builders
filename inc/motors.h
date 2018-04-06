#ifndef MOTOR_H
#define MOTOR_H

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

// sets all motors to 0
void Motor_off();

// bring all motors to a complete stop
void Motor_completeStop();

#endif
