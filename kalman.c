#include "inc/kalman.h"
#include "inc/matrixInverse.h"
#include <math.h> // for using NAN
#include <time.h>
#include "sensors.h"
#include <stdio.h>
#include "controls.h"

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
double Q_diag[NUM] = {25.0, 25.0, 0.0081, 9.0, 9.0, .0289};

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

double Kalman_getX() {
		return x[0];
}

double Kalman_getY() {
		return x[1];
}

double Kalman_getVx() {
		return x[3];
}

double Kalman_getVy() {
		return x[4];
}

double Kalman_getT() {
		return x[2];
}

double Kalman_getVt() {
		return x[5];
}

void print() {
	double x  = Kalman_getX();
	double y  = Kalman_getY();
	double t  = Kalman_getT();
	double vx = Kalman_getVx();
	double vy = Kalman_getVy();
	double vt = Kalman_getVt();

	printf("%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t", x, y, t, vx, vy, vt);
}

void print_vec(double *vec, int len){
	for(int i=0; i<len; i++){
		printf("%.1f\t", vec[i]);
	}
	printf("\n");
}

void print_distances(double *distances) {
	for(int i=0; i<8; i++){
		printf("%.1f\t", distances[i]);
	}
	printf("\n");
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

void Kalman_init(){
	x[0] = 90.0; // x
	x[1] = 90.0; // y
	x[2] = 0.0; // t
	x[3] = 0.0; // vx
	x[4] = 0.0; // vy
	x[5] = 0.0; // vt

	for(int i=0; i<NUM*NUM; i++){
		P[i] = 0.0;
	}
	P[0] = 1.0; // (2.0mm)^2
	P[7] = 1.0; // (2.0mm)^2
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

void interpret_distance_aux(double pos, double dist, double *estimate, int *count){
	if(!isnan(dist)){
		*estimate += snap_to_wall(pos, dist);
		*count += 1;
	}
}

/* given what the distance sensors see, interpret where we think we are */
/* Distance sensors are +y,-y,+x,-x for short, then those four for long */
void interpret_distance(double *distances, double *x_pos, double *y_pos){
	// TODO: take into account the spin of the vehicle?
	double x_estimate = 0;
	int x_count = 0;
	interpret_distance_aux(x[0],  distances[3], &x_estimate, &x_count); //  x, short
	interpret_distance_aux(x[0], -distances[2], &x_estimate, &x_count); // -x, short
	interpret_distance_aux(x[0],  distances[7], &x_estimate, &x_count); //  x, long
	interpret_distance_aux(x[0], -distances[6], &x_estimate, &x_count); // -x, long
	if(x_count == 0){
		// we got no readings for x!
		x_estimate = NAN;
	} else{
		x_estimate /= x_count;
	}

	double y_estimate = 0;
	int y_count = 0;
	interpret_distance_aux(x[1],  distances[0], &y_estimate, &y_count); //  y, short
	interpret_distance_aux(x[1], -distances[1], &y_estimate, &y_count); // -y, short
	interpret_distance_aux(x[1],  distances[4], &y_estimate, &y_count); //  y, long
	interpret_distance_aux(x[1], -distances[5], &y_estimate, &y_count); // -y, long
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

double combine_vx(double encoder, double control){
	// TODO do better than this if we fix encoders
	return control;
}

double combine_vy(double encoder, double control){
	// TODO do better than this if we fix encoders
	return control;
}

double combine_vt(double encoder, double control, double gyro){
	// TODO do better than this if we fix encoders
	return .1*control + .9*gyro;
}




void Kalman_update(double dt){

	// TODO :(
	double encoders[4];
	for(int i=0; i<3; i++){
		encoders[4] = NAN;
	}

	double distances[8];
	for(int i=0; i<4; i++){
		distances[i] = Sensor_getShort(i);
	}
	for(int i=0; i<4; i++){
		distances[4+i] = Sensor_getLong(i);
	}

	// TODO use gyro
	double gyro = Sensor_getGyro();
	printf("\t\t\t\tgyro: %.4f", gyro);
	double compass = NAN;

	double control[3];
	control[0] = Control_getForward();
	control[1] = Control_getRight();
	control[2] = Control_getTheta();

	Kalman_update_given_sensors(dt, encoders, distances, gyro, compass, control);

}


/**
 * update with new sensor values
 * send 8 distance sensors, 4 encoders, accx / accy / gyro,
 * attempted vx/vy/spin from control, address to write position
 *
 * NAN for any sensor values means we don't have a reading for whatever reason (no wall nearby)
 *
 * Distance sensors are +y,-y,+x,-x for short, then those four for long
 *
 **/
void Kalman_update_given_sensors(double dt, double *encoders, double *distances, double gyro, double compass, double *control){

	//printf("Distances, Controls\n");
	//print_distances(distances);
	printf("Control: ");
	print_vec(control, 3);

	//printf("Top of update\n");
	//print();



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
	/* we're working hard to do all this interpretation beforehand so the matrix that transforms our
	   sensor readings into the right dimensions (H in the tutorial) is just the identity */

	double dist_x,dist_y; // measured x and y location
	interpret_distance(distances, &dist_x, &dist_y);

	printf("Interpreted x,y: %.1f, %.1f\n", dist_x, dist_y);


	// TODO redo this if we fix encoders
	//double encoder_vx, encoder_vy, encoder_vt;
	//nterpret_encoders(encoders, &encoder_vx, &encoder_vy, &encoder_vt);
	double encoder_vx = NAN;
	double encoder_vy = NAN;
	double encoder_vt = NAN;

	double vx_estimate = combine_vx(encoder_vx, control[0]);
	double vy_estimate = combine_vy(encoder_vy, control[1]);
	double vt_estimate = combine_vt(encoder_vt, control[2], gyro);



	if (isnan(dist_x)) {
			z[0] = x_hat[0];
			R[NUM*0+0] = 40000;
	} else {
			z[0] = dist_x;
			R[NUM*0+0] = 4.0;
	}

	if (isnan(dist_y)) {
			z[1] = x_hat[1];
			R[NUM*1+1] = 40000;
	} else {
			z[1] = dist_y;
			R[NUM*1+1] = 4.0;
	}

	z[2] = 0; // TODO compass
	R[NUM*2+2] = 36.0; // 2pi squared?

	if (isnan(vx_estimate)) {
			z[3] = x_hat[3];
			R[NUM*3+3] = 110889;
	} else {
			z[3] = vx_estimate;
			R[NUM*3+3] = 10.0;
	}

	if (isnan(vy_estimate)) {
			z[4] = x_hat[4];
			R[NUM*4+4] = 110889;
	} else {
			z[4] = vy_estimate;
			R[NUM*3+3] = 10.0;
	}

	if (isnan(vt_estimate)) {
			z[5] = x_hat[5];
			R[NUM*5+5] = 10;
	} else {
			z[5] = vt_estimate;
			R[NUM*5+5] = 0.01;
	}



	printf("z and x_hat\n");
	print_vec(z, 6);
	print_vec(x_hat, 6);



	/* TODO: read the comment above. Create an R matrix for sensor noise covariances, and do the
	   actual kalman update step, equations 18 and 19 here
	   http://www.bzarg.com/p/how-a-kalman-filter-works-in-pictures/(but H=I so it's easy)

	   compile with "gcc kalman.c inc/kalman.h"*/

	// equation 19
	mat_add(P_hat, R, temp);
	invert(temp);
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

	printf("Bottom of update\n");
	print();
	printf("\n");
}
