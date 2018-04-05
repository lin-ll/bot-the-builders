#include "inc/kalman.h"
#include "inc/matrixInverse.h"
#include <math.h> // for using NAN

/* Naming conventions and general info from here:
   http://www.bzarg.com/p/how-a-kalman-filter-works-in-pictures/   */

#define NUM 6
#define SQUARE_SIZE 180 // TODO is this right?

// x, y, t, vx, vy, vt
// units are mm, mm/s, rad clockwise with 0 at +y, rad/s
double x[NUM];
double z[NUM];
double x_hat[NUM];
double P[NUM*NUM];
double P_hat[NUM*NUM];
double temp[NUM*NUM];
double K[NUM*NUM];
double tempVec[NUM];

/* In each step, how much uncertainty is there in our prediction? */
/* I really really am unsure of these values; if it fails miserably try dividing them by 10 ?? */
double Q_diag[NUM] = {1.0, 1.0, 0.0081, 4.0, 4.0, .0289};

// initialize F with identity, other entries will be changed in update
double F[NUM*NUM] =
		   {1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0,
			0, 0, 1, 0, 0, 0,
			0, 0, 0, 1, 0, 0,
			0, 0, 0, 0, 1, 0,
			0, 0, 0, 0, 0, 1};

double R[NUM*NUM] =
			{4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
			 0.0, 4.0, 0.0, 0.0, 0.0, 0.0,
			 0.0, 0.0, 10.0, 0.0, 0.0, 0.0,
			 0.0, 0.0, 0.0, 10.0, 0.0, 0.0,
			 0.0, 0.0, 0.0, 0.0, 0.01, 0.0,
			 0.0, 0.0, 0.0, 0.0, 0.0, 0.01};

double kalman_getX() {
		return x[0];
}

double kalman_getY() {
		return x[1];
}

double kalman_getVx() {
		return x[3];
}

double kalman_getVy() {
		return x[4];
}

double kalman_getT() {
		return x[2];
}

double kalman_getVt() {
		return x[5];
}

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

void mat_add(double *A, double* B, double *dest) {
		for (int i = 0; i < NUM; i++) {
				for (int j = 0; j < NUM; j++) {
						int index = i*NUM + j;
						dest[index] = 0.0;
						dest[index] += A[index] + B[index];
				}
		}
}

void mat_subtract(double *A, double* B, double *dest) {
		for (int i = 0; i < NUM; i++) {
				for (int j = 0; j < NUM; j++) {
						int index = i*NUM + j;
						dest[index] = 0.0;
						dest[index] += A[index] - B[index];
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

// given the current location and distance to the wall, where is the nearby distance you
// must be so that the end of the measurement hits the wall exactly
// NOTE must make the dist negative if the sensor is pointed in the negative direction
double snap_to_wall(double pos, double dist){
	// 20.5 instead of .5 to avoid negative numbers even if we see a wall far in the negative
	double err = fmod(pos + dist + SQUARE_SIZE*20.5, SQUARE_SIZE)-SQUARE_SIZE*0.5;
	return pos - err;
}

double interpret_distance_aux(double pos, double dist, double *estimate, int *count){
	if(!isnan(dist)){
		*estimate += snap_to_wall(pos, dist);
		*count += 1;
	}
}

/* given what the distance sensors see, interpret where we think we are */
void interpret_distance(double *distances, double *x_pos, double *y_pos){
	// TODO: take into account the spin of the vehicle?
	double x_estimate = 0;
	int x_count = 0;
	interpret_distance_aux(x[0],  distances[0], &x_estimate, &x_count); //  x, short
	interpret_distance_aux(x[0], -distances[1], &x_estimate, &x_count); // -x, short
	interpret_distance_aux(x[0],  distances[4], &x_estimate, &x_count); //  x, long
	interpret_distance_aux(x[0], -distances[5], &x_estimate, &x_count); // -x, long
	if(x_count == 0){
		// we got no readings for x!
		x_estimate = NAN;
	} else{
		x_estimate /= x_count;
	}

	double y_estimate = 0;
	int y_count = 0;
	interpret_distance_aux(x[1],  distances[2], &y_estimate, &y_count); //  y, short
	interpret_distance_aux(x[1], -distances[3], &y_estimate, &y_count); // -y, short
	interpret_distance_aux(x[1],  distances[6], &y_estimate, &y_count); //  y, long
	interpret_distance_aux(x[1], -distances[7], &y_estimate, &y_count); // -y, long
	if(y_count == 0){
		// we got no readings for y!
		y_estimate = NAN;
	} else{
		y_estimate /= y_count;
	}

	*x_pos = x_estimate;
	*y_pos = y_estimate;
}

/* Puts the info you need in the arguments, returns how much the wheels slipped (that's bad) */
double interpret_encoders(double *encoders, double *vx, double *vy, double *vt){
	*vx = encoders[0]+encoders[1]-encoders[2]-encoders[3];
	*vy = encoders[0]-encoders[1]-encoders[2]+encoders[3];
	*vt = encoders[0]+encoders[1]+encoders[2]+encoders[3];
	return encoders[0]-encoders[1]+encoders[2]-encoders[3];
}
/**
 * update with new sensor values
 * send 8 distance sensors, 4 encoders, accx / accy / gyro,
 * attempted vx/vy/spin from control, address to write position
 *
 * NAN for any sensor values means we don't have a reading for whatever reason (no wall nearby)
 *
 * Distance sensors are +x,-x,+y,-y for short, then those four for long
 *
 **/
void update(double *distances, double *encoders, double *imu, double *control, double *pos){
	double dt = 0.05; // TODO use an actual timer

	/* First, the predict step. Without sensors, what do we guess the new x is?
	   We write a matrix that will take old x to new x */
	F[3] = dt; // x += vx*dt
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

	/* TODO: control vector stuff. Basically just set x values to what the controller wants
	   them to be, and update P with some variances (this is vague because I don't really
	   know how to do it). */


	/* Add general uncertainty */
	/* Q is a diagonal matrix, so we only save / use the diagonal entries for better performance */
	for(int i=0; i<NUM; i++){
		P_hat[NUM*i + i] += Q_diag[i];
	}


	/* Measurement step, */
	double dist_x,dist_y; // measured x and y location
	interpret_distance(distances, &dist_x, &dist_y);

	double encoder_vx, encoder_vy, encoder_vt;
	interpret_encoders(encoders, &encoder_vx, &encoder_vy, &encoder_vt);
	/* we're working hard to do all this interpretation beforehand so the matrix that transforms our
	   sensor readings into the right dimensions (H in the tutorial) is just the identity */
	if (isnan(dist_x)) {
			z[0] = x_hat[0];
			R[0][0] = 40000;
	} else {
			z[0] = dist_x;
			R[0][0] = 4.0;
	}

	if (isnan(dist_y)) {
			z[1] = x_hat[1];
			R[1][1] = 40000;
	} else {
			z[1] = dist_x;
			R[1][1] = 4.0;
	}

	z[2] = 0; // TODO compass

	if (isnan(encoder_vx)) {
			z[3] = x_hat[3];
			R[3][3] = 110889;
	} else {
			z[2] = dist_x;
			R[3][3] = 10.0
	}

	if (isnan(encoder_vy)) {
			z[4] = x_hat[4];
			R[4][4] = 110889;
	} else {
			z[4] = dist_x;
			R[3][3] = 10.0;
	}

	if (isnan(encoder_vt)) {
			z[5] = x_hat[5];
			R[5][5] = 10;
	} else {
			z[5] = dist_x;
			R[5][5] = 0.01;
	}



	/* TODO: read the comment above. Create an R matrix for sensor noise covariances, and do the
	   actual kalman update step, equations 18 and 19 here
	   http://www.bzarg.com/p/how-a-kalman-filter-works-in-pictures/(but H=I so it's easy)

	   compile with "gcc kalman.c inc/kalman.h"*/

	// equation 19
	mat_add(P_hat, R, temp);
	matrixInvert(temp);
	mat_mult(P_hat, temp, K);

	// equation 18
	mat_mult(K, P_hat, temp);
	mat_subtract(P_hat, temp, P);
	for (int i = 0; i < NUM; i++) {
			z[i] = z[i] - x_hat[i];
	}
	mat_vec_mult(K, z, tempVec);
	for (int i = 0; i < NUM; i++) {
			x[i] = x_hat[i] + tempVec[i];
	}
}
