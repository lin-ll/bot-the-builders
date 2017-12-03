#ifndef SENSOR_H
#define SENSOR_H

/* Motor Dirver */

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

/* Linear Stuff */
enum Dir_t {
  kFRONT,
  kBACK,
  kLEFT,
  kRIGHT
};

enum Orient_t {kNORTH, kEAST, kSOUTH, kWEST};

/* Return distance in cm */

double Sensor_getShort();//Dir_t dir);
double Sensor_getLong(enum Dir_t dir);
double Sensor_orient(enum Dir_t dir);

/* Any Cleanup */
void Sensor_free();


#endif
