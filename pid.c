/*
This module creates PID loop objects.
*/

#include <stdlib.h>
#include "pid.h"
#include "constants.h"

typedef struct PID *PID_T;

// return a PID loop object
PID_T Pid_init(double kP, double kI, double kD) {
  struct PID *p = (PID *)malloc(sizeof(PID_T));
		p->kP = kP;
		p->kI = kI;
		p->kD = kD;
		p->integral = 0;
		p->setPoint = 0;
		p->derivative = 0;
		p->prevError = 0;
		p->isFirstUpdate = TRUE;
		return p;
};

void Pid_setPoint(PID_T pid, double setpoint_value) {
		pid->setPoint = setpoint_value;// TODO
}

// reset prevMeasure, setPoint,integral, prevError, derivative
void Pid_reset(PID_T pid) {
		pid->integral = 0;// TODO
		pid->setPoint = 0;
		pid->derivative = 0;
		pid->prevError = 0;
}

// update PID_T
void Pid_update(PID_T pid, double val, double dt) {
		double error = pid->setPoint - val;
		pid->integral = pid->integral + error * dt;
		pid->derivative = (error - pid->prevError) / dt;
		pid->prevError = error;
		pid->isFirstUpdate = FALSE;
}

// return PID value
double Pid_getVal(PID_T pid) {
		return (pid->kP * pid->prevError) + (pid->kI * pid->integral) + (pid->derivative * pid->kD);
}
