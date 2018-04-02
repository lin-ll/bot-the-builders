#include <stdio.h>
#include "matrixInverse.h"


double a[36] = {
	1,2,3,4,5,6,
	7,8,9,8,7,6,
	5,4,3,2.5,1,2,
	3,4.5,5,6,7,8,
	9,8,7,6,5,4,
	3,2,1,2,3,4.5};

double a_inverse[36];
double b[36];


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

int main(){
	for(int i=0; i<36; i++){
		a_inverse[i] = a[i];
	}
	for(int i=0; i<6; i++){
		for(int j=0; j<6; j++){
			printf("%f\t", a_inverse[i*6+j]);
		}
		printf("\n");
	}

	invert(a_inverse);
	
	for(int i=0; i<6; i++){
		for(int j=0; j<6; j++){
			printf("%f\t", a_inverse[i*6+j]);
		}
		printf("\n");
	}


	mat_mult(a, a_inverse, b);

	for(int i=0; i<6; i++){
		for(int j=0; j<6; j++){
			printf("%f\t", b[i*6+j]);
		}
		printf("\n");
	}

	return 0;
}

