#include "sensors.h"
#include <pigpiod_if2.h>
#include "constants.h"
#include "adafruit_distance.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>

/* Some quick bus info
0x6B gyro
0x1E Magnetometer (if that's wrong, try 1f)
0x29 Short distance
0x52 Long distance
*/

const int SHORT_DIST_ADDRS[4] = {0x2D, 0x2C, 0x2B, 0x2A}; // bogus addresses
const int LONG_DIST_ADDRS[4] = {0x31, 0x30, 0x2F, 0x2E}; // bogus addresses

int tinyHandle;

// NOTE: short_back must go first because it doesn't have a shutdown pin
const int SHORT_SHUTDOWN_PINS[4] = {SHORT_PIN_BACK, SHORT_PIN_FRONT, SHORT_PIN_LEFT, SHORT_PIN_RIGHT};
const int LONG_SHUTDOWN_PINS[4] = {LONG_PIN_FRONT, LONG_PIN_BACK, LONG_PIN_LEFT, LONG_PIN_RIGHT};

// The datasheet gives 8.75mdps/digit for default sensitivity
const double RPS_PER_DIGIT = 0.00875*TWO_PI/360;

enum { // TODO: Check these registers
  GYRO_REGISTER_OUT_X_L          = 0x28,
  GYRO_REGISTER_OUT_X_H          = 0x29,
  GYRO_REGISTER_OUT_Y_L          = 0x2A,
  GYRO_REGISTER_OUT_Y_H          = 0x2B,
  GYRO_REGISTER_OUT_Z_L          = 0x2C,
  GYRO_REGISTER_OUT_Z_H          = 0x2D,

  ACC_REGISTER_OUT_X_L_A         = 0x28,
  ACC_REGISTER_OUT_X_H_A         = 0x29,
  ACC_REGISTER_OUT_Y_L_A         = 0x2A,
  ACC_REGISTER_OUT_Y_H_A         = 0x2B,
  ACC_REGISTER_OUT_Z_L_A         = 0x2C,
  ACC_REGISTER_OUT_Z_H_A         = 0x2D,

  COMPASS_REGISTER_OUT_X_H_M     = 0x08, // HEY! if these values are wrong, try adding 5 to each
  COMPASS_REGISTER_OUT_X_L_M     = 0x09, // There's another version of this chip with those addresses
  COMPASS_REGISTER_OUT_Y_H_M     = 0x0A,
  COMPASS_REGISTER_OUT_Y_L_M     = 0x0B,
  COMPASS_REGISTER_OUT_Z_H_M     = 0x0C,
  COMPASS_REGISTER_OUT_Z_L_M     = 0x0D,
};

static double compassOffset = 0;
static double gyroOffset = 0;

static int gyro_handle = 0;
static int compass_handle = 0;
static int short_dist_handles[4] = {0, 0, 0, 0};
static int long_dist_handles[4] = {0, 0, 0, 0};
static int pi;

/* Local Functions */

int test_read8(int handle, int address)
{
  char dataWrite[2];
  char dataRead[1];

  dataWrite[0] = (address >> 8) & 0xFF;
  dataWrite[1] = address & 0xFF;
  int w = i2c_write_device(pi, handle, dataWrite, 2);
  int r = i2c_read_device(pi, handle, dataRead, 1);

  if(w != 0) printf("Write Code: %d\n", w);
  if(r != 1) printf("Read Code: %d\n", r);

  return (int)dataRead[0];
}

void test_write8(int handle, int address, int data){
    int test;
    char data_write[3];
    data_write[0] = (address >> 8) & 0xFF;; // MSB of register address
    data_write[1] = address & 0xFF; // LSB of register address
    data_write[2] = data & 0xFF;
    test = i2c_write_device(pi, handle, data_write, 3);
    if (test != 0) printf("write returned %d\n", test);

    int result = test_read8(handle, address);
    printf("wrote %d and read back %d\n", data, result);
  }

int test_readblock(int handle, int address, char *buffer, int len){
  for(int i=0; i<len; i++){
    buffer[i] = test_read8(handle, address);
    printf("Read byte %d: %d\n", i, buffer[i]);
  }
  return len; // TODO eh
}

static double getCompassRaw() {
  int temp = 0;
  int16_t xRaw;
  int16_t yRaw;
  double angle;

  char buffer[32];
  buffer[6] = 42; // Sentinel to check whether it's being touched
  //temp = i2c_read_i2c_block_data(pi, compass_handle, COMPASS_REGISTER_OUT_X_H_M | 0x80, buffer, 6);
  temp = test_readblock(compass_handle, COMPASS_REGISTER_OUT_X_H_M | 80, buffer, 6);
  if (temp != 6) {
    printf("ERROR: read_block_data returns %d\n", temp);
  }

  xRaw = ((int16_t)(buffer[5]) << 8) + (int16_t)(buffer[4]);
  yRaw = ((int16_t)(buffer[1]) << 8) + (int16_t)(buffer[0]);

  angle = atan2((double)yRaw, (double)xRaw);
  return angle;
}

/* Initialize gyro handle and settings */
static void initGyro() {
  // From datasheet of sensor
  int CTRL1 = 0x20;
  int CTRL4 = 0x23;
  int LOWODR = 0x39;

  gyro_handle = i2c_open(pi, BUS, GYRO_ADDR, 0);
  test_write8(gyro_handle, CTRL1, 0x6F);
  test_write8(gyro_handle, CTRL4, 0x00);
  test_write8(gyro_handle, LOWODR, 0x00);
  printf("gyro_handle: %d\n", gyro_handle);
  //Sensor_calGyro(100);
}

/* Initialize compass handle and settings */
static void initCompass() {
  // From datasheet of sensor
  int CTRL5 = 0x24;
  int CTRL6 = 0x25;
  int CTRL7 = 0x26;

  compass_handle = i2c_open(pi, BUS, COMPASS_ADDR, 0);
  i2c_write_byte_data(pi, compass_handle, CTRL5, 0x64);
  i2c_write_byte_data(pi, compass_handle, CTRL6, 0x20);
  i2c_write_byte_data(pi, compass_handle, CTRL7, 0x00);
  printf("compass_handle: %d\n", compass_handle);
  getCompassRaw();
  //Sensor_calCompass(100);
}

/**
 * Initialize Sensors
 * Return EXIT_SUCCESS (0) when done
 **/
int Sensor_init(int pifd) {
  pi = pifd;
  int i, j, success;

  adafruit_distance_set_pi_handle(pi);
  // Getting the handles for short and long distance
  for(i=0; i<4; i++) {
    short_dist_handles[i] = i2c_open(pi, BUS, SHORT_DIST_ADDRS[i], 0);
    if(short_dist_handles[i] < 0)
      printf("Bad handle for short distance sensor %d: %d\n", i, short_dist_handles[i]);
    printf("Short distance handle %d: %d\n", i, short_dist_handles[i]);
  }

  for(i=0; i<4; i++) {
    long_dist_handles[i] = i2c_open(pi, BUS, LONG_DIST_ADDRS[i], 0);
    if(long_dist_handles[i] < 0)
      printf("Bad handle for long distance sensor %d: %d\n", i, long_dist_handles[i]);
    printf("Long distance handle %d: %d\n", i, long_dist_handles[i]);
  }

  printf("GOT ALL HANDLES\n");

  // Shut all of the short and long distance pins down
  for(i=0; i<4; i++) {
    if(SHORT_SHUTDOWN_PINS[i] != -1) {
      gpio_write(pi, SHORT_SHUTDOWN_PINS[i], DISTANCE_OFF);
      printf("Turn off short distance sensor: %d\n", SHORT_SHUTDOWN_PINS[i]);
    }
  }

  for(i=0; i<4; i++) {
    if(LONG_SHUTDOWN_PINS[i] != -1) {
      gpio_write(pi, LONG_SHUTDOWN_PINS[i], DISTANCE_OFF);
      printf("Turn off long distance sensor: %d\n", LONG_SHUTDOWN_PINS[i]);
    }
  }

  printf("ALL OFF\n");

  //success = adafruit_distance_begin(short_dist_handles[3]);
  //printf("SUCCESS WAS %d\n", success);

  // One by one, turn on short distance sensors
  for(i=0; i<4; i++) {
    printf("Turning on short distance sensor: %d\n", i);
    if(SHORT_SHUTDOWN_PINS[i] != -1)
      gpio_write(pi, SHORT_SHUTDOWN_PINS[i], DISTANCE_ON);

    sleep(1);

    // Library thinks we're talking to the same sensor each time
    printf("Changing address for handle %x to addr %x\n", short_dist_handles[3],SHORT_DIST_ADDRS[i]);
    adafruit_distance_change_address(short_dist_handles[3], SHORT_DIST_ADDRS[i]);
    success = adafruit_distance_begin(short_dist_handles[i]);

    if(!success)
      printf("Short distance sensor error: %d\n", i);
  }

  // One by one, turn on long distance sensors
  for(i=0; i<4; i++) {
    printf("Turning on long distance sensor: %d\n", i);
    if(LONG_SHUTDOWN_PINS[i] != -1)
      gpio_write(pi, LONG_SHUTDOWN_PINS[i], DISTANCE_ON);

    sleep(1);

    // Library thinks we're talking to the same sensor each time
    printf("Changing address for handle %x to addr %x\n", long_dist_handles[3],LONG_DIST_ADDRS[i]);
    adafruit_distance_change_address(long_dist_handles[3], LONG_DIST_ADDRS[i]);
    // success = adafruit_distance_begin(long_dist_handles[i]);
    // if(!success)
    //   printf("Long distance sensor error: %d\n", i);

    // for(j=0; j<100;j++){
    //   //printf("%d:\t%d\n", i, adafruit_distance_readRange(long_dist_handles[i]));
    // }
  }
  

  printf("ALL ON\n");

  // TODO: subtract numbers from each of the sensors - if the calibration is wrong

  // TODO: figure out what to do with accelerometer
  //acc_handle = i2c_open(ACC_BUS, ACC_ADDRESS);

  //initGyro();
  //initCompass();
  return 0;
}

/* Return angle in radians or radians/s. */
double Sensor_getGyro(){
  char buffer[32];
  // needs to be 16 bits signed so the signs work out correctly
  int16_t raw;

  //int16_t a = adafruit_distance_read16(GYRO_REGISTER_OUT_X_L | 80);
  //printf("A is %d\n", a);

  i2c_read_i2c_block_data(pi, gyro_handle, GYRO_REGISTER_OUT_X_L | 0x80, buffer, 6);
  raw = ((int16_t)(buffer[3]) << 8) + (int16_t)(buffer[2]);

  return raw * RPS_PER_DIGIT - gyroOffset;
}

/* Return direction. */
double Sensor_getCompass(){
  double raw = getCompassRaw();
  return fmod(((raw - compassOffset) + TWO_PI), TWO_PI);
}

/* Calibrates current angle as "0", averaging over n readings */
void Sensor_calCompass(int n){
  int i;
  double center = getCompassRaw();
  double c;
  double offset = 0;
  for (i = 0; i < n; i++) {
    c = getCompassRaw();
    if (c - center > (TWO_PI/2)) {
      offset += c - TWO_PI;
    } else if (c - center < (-1*TWO_PI/2)) {
      offset += c + TWO_PI;
    } else {
      offset += c;
    }
  }
  compassOffset = offset / n;
}

/* Calibrate gyro */
void Sensor_calGyro(int n) {
  int i;
  double c = 0;

  for (i = 0; i < n; i++) {
    c += Sensor_getGyro();
  }

  gyroOffset = c / (double)n;
}

/* Return distance from short distance sensor in mm */
double Sensor_getShort(int num) {
  return (double)adafruit_distance_readRange(short_dist_handles[num]);
}

/* Return distance from long distance sensor in mm */
double Sensor_getLong(int num) {
  return (double)adafruit_distance_readRange(long_dist_handles[num]);
}

void Sensor_findWalls(int *walls) {
    if (Sensor_getShort(UP) > SQUARE_SIZE / 2) {
        walls[0] = 1;
    }
    if (Sensor_getShort(DOWN) > SQUARE_SIZE / 2) {
        walls[1] = 1;
    }
    if (Sensor_getShort(LEFT) > SQUARE_SIZE / 2) {
        walls[2] = 1;
    }
    if (Sensor_getShort(RIGHT) > SQUARE_SIZE / 2) {
        walls[3] = 1;
    }
}

/* Any cleanup */
void Sensor_free(){
  int i;
  i2c_close(pi, gyro_handle);
  i2c_close(pi, compass_handle);

  // TODO: not sure whether it's a good idea to reset addresses before shutting down
  for(i=0; i<4; i++){
    i2c_close(pi, short_dist_handles[i]);
    i2c_close(pi, long_dist_handles[i]);
  }
}

int Sensor_initTiny(int pi){
	tinyHandle = i2c_open(pi, 1, 0x4, 0);
	printf("Tiny Handle: %d\n", tinyHandle);
}

double Sensor_getEncoders(){
	char data[17];
	data[0] = 42;
	data[17] = 43;
	int ret = i2c_read_i2c_block_data(pi, tinyHandle, 1, data, 16);
	printf("ret= %d\n", ret);

	for(int i=0; i<17; i++){
		printf("Byte %d: %d", data[i]);
	}
	return 0;

}
