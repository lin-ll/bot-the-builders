#ifndef SENSOR_H
#define SENSOR_H

/* Motor Driver */

/**
 * Initialize Sensors
 * Return 1 on Success.
 * Return 0 on Failure
 **/
int Sensor_init(int pifd);

/* Return angle in radians or radians/s. */
double Sensor_getGyro();
double Sensor_getCompass();

/* Calibrates Current angle as "0" (averages over n readings)*/
void Sensor_calCompass(int n);
void Sensor_calGyro(int n);

/* Return speed of motors */
double Sensor_getMotorSpeed(int motor);

/* Linear Stuff */
enum Dir_t {
  kFRONT,
  kBACK,
  kLEFT,
  kRIGHT
};

enum Orient_t {kNORTH, kEAST, kSOUTH, kWEST};

/* Return distance in cm */

double Sensor_getShort(int num);//Dir_t dir);
double Sensor_getLong(enum Dir_t dir);
double Sensor_orient(enum Dir_t dir);

/* Any Cleanup */
void Sensor_free();

// finds all walls in square;
void Sensor_findWalls(int *walls);

int Sensor_initTiny(int pi);

double Sensor_getEncoders();


#endif
