//pid.h
#ifndef PID_H
#define PID_H

struct PID {
		double kP ,kI, kD;
		double integral;
		double setPoint;
		double derivative;
		double prevError;
		int isFirstUpdate;
};

typedef struct PID *PID_T;

// return a PID loop object
PID_T Pid_init(double kP, double kI, double kD);

void Pid_setPoint(PID_T pid, double setpoint_value);

// reset prevMeasure, setPoint,integral, prevError, derivative
void Pid_reset(PID_T pid);

// update PID_T
void Pid_update(PID_T pid, double val, double dt);

// return PID value
double Pid_getVal(PID_T pid);
#endif

