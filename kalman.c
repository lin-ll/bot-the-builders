#include "kalman.h"

#define NUM 6


// x, y, t, vx, vy, vt
// units are mm, mm/s, rad clockwise with 0 at +y, rad/s
double x[NUM];
double x_hat[NUM];
double P[NUM*NUM];
double P_hat[NUM*NUM];
double temp[NUM*NUM];

// initialize F with identity, other indices will be changed in update
double F[NUM*NUM] = 
		   {1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0,
			0, 0, 1, 0, 0, 0,
			0, 0, 0, 1, 0, 0,
			0, 0, 0, 0, 1, 0,
			0, 0, 0, 0, 0, 1};




void mat_mult(double *A, double *B, double *dest){
	for (int i = 0; i < NUM; i++) {
		for (int j = 0; j < NUM; j++) {
			int index = i*NUM + j;
			dest[index] = 0.0;
			for (int k = 0; k < NUM; k++) {
				dest[index] += A[NUM*i + k] * B[NUM*k + j];
			}
		}
	}
}

/* dest = A*transpose(B) */
void mat_mult_btrans(double *A, double *B, double *dest){
	for (int i = 0; i < NUM; i++) {
		for (int j = 0; j < NUM; j++) {
			int index = i*NUM + j;
			dest[index] = 0.0;
			for (int k = 0; k < NUM; k++) {
				dest[index] += A[NUM*i + k] * B[k + NUM*j]; // TODO double check this
			}
		}
	}
}

void mat_vec_mult(double *A, double *x, double *dest){
	for (int i = 0; i < NUM; i++) {
		dest[i] = 0;
		for (int j = 0; j < NUM; j++) {
			dest[i] += A[NUM*i + j] * x[j];
		}
	}
}

void init(){
	x[0] = 1.0; // x
	x[1] = 1.0; // y
	x[2] = 0.0; // t
	x[3] = 0.0; // vx
	x[4] = 0.0; // vy
	x[5] = 0.0; // vt

	for(int i=0; i<NUM*NUM; i++){
		P[i] = 0.0;
	}
	P[0] = 4.0; // (2.0mm)^2
	P[7] = 4.0; // (2.0mm)^2
	P[14] = 0.0289; // (0.17rad)^2

}

void update(double *distances, double *encoders, double *imu, double *control, double *pos){
	double dt = TODO;

	/* First, the predict step. Without sensors, what do we guess the new x is? 
	   We write a matrix that will take old x to new x */
	F[ 3] = dt; // x += vx*dt
	F[10] = dt; // y += vy*dt
	F[17] = dt; // t += vt*dt
	/* if the robot is turned clockwise, then velocity up also moves the robot slightly right.
	   This can't be captured with a constant in the matrix, so we copy the velocity up into 
	   the matrix. This means we are ignoring the uncertainty in the velocity in this calculation,
	   but we expect the % uncertainty in the angle to be much greater and we do take that into 
	   account. Note we are also linearizing sin around zero, so sin(t) becomes just t.*/
	F[2] =  dt * x[4]; // x += t*(dt*vy)
	F[8] = -dt * x[3]; // y -= t*(dt*vx)


	mat_vec_mult(F, x, x_hat);
	mat_mult(F, P, temp);
	mat_mult_btrans(temp, F, P_hat); // P_hat = F*P*transpost(F)




}


