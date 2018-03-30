//pid.h

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
PID_T init(double kP, double kI, double kD);

void setPoint(PID_T pid, double setpoint_value);

// reset prevMeasure, setPoint,integral, prevError, derivative
void reset(PID_T pid);

// update PID_T
void update(PID_T pid, double val, double dt);

// return PID value
double getVal(PID_T pid);
