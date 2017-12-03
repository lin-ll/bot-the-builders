#include <math.h>
#include <sensors.h>
#include <pigpiod_if2.h>
#include <adafruit_distance.h>
#include <stdio.h>

// const int SDA_A = 1000
// const int SCL_A = 1000
// const int SDA_B = 1000
// const int SCL_B = 1000

/* Some quick bus info
0x6B gyro
0x1E Magnetometer (if that's wrong, try 1f)
0x29 Short distance
0x52 Long distance
BUS 0 (Pi pins 27 SDA, 28 SCL)
Gyro/a/m, long, short
BUS 1 (Pi pins 3 SDA, 5 SCL)
long, short, short
*/

#define TWO_PI (2*3.1415926536)
#define DISTANCE_OFF 0
#define DISTANCE_ON 1

const int BUS = 1;
const int GYRO_ADDR = 0x6B; // 7 bit 1101011;
const int GYRO_BUS = 1; // check http://abyz.co.uk/rpi/pigpio/python.html#i2c_open
const int COMPASS_ADDR = 0x1D; // if it's wrong, try 1F
const int COMPASS_BUS = 1;
//const int SHORT_ADDR_A = 0x29; // The original address
//const int SHORT_ADDR_B = 0x30;
//const int SHORT_ADDR_C = 0x31;
//const int LONG_ADDR_A = -1;  // The original address
//const int LONG_ADDR_B = -1;

const int DIST_ADDRS[5] = {0x2D, 0x2C, 0x2B, 0x2A, 0x29};
const int DIST_SHUTDOWNS[5] = {-1, 24, 23, 22, 27};

//const int SHORT_SHUTDOWN_A = -1;
//const int SHORT_SHUTDOWN_B = -1;
//const int LONG_SHUTDOWN_A = -1;


// The datasheet gives 8.75mdps/digit for default sensitivity
const double RPS_PER_DIGIT = 0.00875*TWO_PI/360;

enum {     
  GYRO_REGISTER_OUT_X_L             = 0x28,   //            r
  GYRO_REGISTER_OUT_X_H             = 0x29,   //            r
  GYRO_REGISTER_OUT_Y_L             = 0x2A,   //            r
  GYRO_REGISTER_OUT_Y_H             = 0x2B,   //            r
  GYRO_REGISTER_OUT_Z_L             = 0x2C,   //            r
  GYRO_REGISTER_OUT_Z_H             = 0x2D,   //            r

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
//static int dist_A_handle = 0;
//static int dist_B_handle = 0;
//static int short_C_handle = 0;
//static int long_A_handle = 0;
//static int long_B_handle = 0;
static int dist_handles[5] = {0, 0, 0, 0, 0};
//static enum Orient_t currentOrient = kNORTH;
static int pi;

/* Local Functions */
static double getCompassRaw() {
  int temp = 0;
  int16_t xRaw; // = i2c_read_byte_data(pi, compass_handle, COMPASS_REGISTER_OUT_X_L_M);
  int16_t yRaw; // = i2c_read_byte_data(pi, compass_handle, COMPASS_REGISTER_OUT_Y_L_M);
  double angle;

  char buffer[32]; // Do we need unsigned here?
  buffer[6] = 42; // Sentinel to check whether it's being touched
  temp = i2c_read_i2c_block_data(pi, compass_handle, COMPASS_REGISTER_OUT_X_H_M | 0x80, buffer, 6);
  if (temp != 6) {
    printf("ERROR: read_block_data returns %d\n", temp);
  }

  xRaw = ((int16_t)(buffer[5]) << 8) + (int16_t)(buffer[4]);
  yRaw = ((int16_t)(buffer[1]) << 8) + (int16_t)(buffer[0]);

  angle = atan2(yRaw, xRaw);
  return angle;
}

/**
 * Initialize Sensors
 * Return 1 on Success.
 * Return 0 on Failure
 **/
int Sensor_init(int pifd) {
  pi = pifd;
  int CTRL1 = 0x20;
  int CTRL4 = 0x23;
  int LOWODR = 0x39;
  int CTRL5 = 0x24;
  int CTRL6 = 0x25;
  int CTRL7 = 0x26;
  int i, j, success;
  char *temp = "i'm doomed";

  adafruit_distance_set_pi_handle(pi);
  
  //short_A_handle = i2c_open(pi, BUS, SHORT_ADDR_A, 0);
  //short_B_handle = i2c_open(pi, BUS, SHORT_ADDR_B, 0);
  //short_C_handle = i2c_open(pi, BUS, SHORT_ADDR_C, 0);
  //long_A_handle = i2c_open(pi, BUS, )
  for(i=0; i<5; i++) {
    dist_handles[i] = i2c_open(pi, BUS, DIST_ADDRS[i], 0);
    if(dist_handles[i] < 0) printf("Bad handle for dist %d: %d\n", i, dist_handles[i]);
    printf("handle %d is: %d  ", i, dist_handles[i]);
  }
  printf("\n");

  //gpio_write(pi, SHORT_SHUTDOWN_A, DISTANCE_OFF); 
  //gpio_write(pi, SHORT_SHUTDOWN_B, DISTANCE_OFF);
  //gpio_write(pi, LONG_SHUTDOWN_A, DISTANCE_OFF);
  //gpio_write(pi, LONG_SHUTDOWN_B, DISTANCE_OFF);

  for(i=0; i<5; i++) {
    if(DIST_SHUTDOWNS[i] != -1) {
      gpio_write(pi, DIST_SHUTDOWNS[i], DISTANCE_OFF); 
      printf("off %d\n", DIST_SHUTDOWNS[i]);
    }
  }
    

  /*
  while(1) {
    for(i=0; i<5; i++) {
      if(DIST_SHUTDOWNS[i] != -1) {
        gpio_write(pi, DIST_SHUTDOWNS[i], DISTANCE_OFF); 
        printf("off %d\n", DIST_SHUTDOWNS[i]);
      }
    }
    printf("all off!\n");
    time_sleep(3);
    for(i=0; i<5; i++) {
      if(DIST_SHUTDOWNS[i] != -1) {
        gpio_write(pi, DIST_SHUTDOWNS[i], DISTANCE_ON); 
        printf("on %d\n", DIST_SHUTDOWNS[i]);
      }
    }
    printf("all on\n");
    time_sleep(3);
  }
  */
  

  printf("all off\n");
  //time_sleep(10);
  
  // deal with short range
  for(i=0;i<3;i++) {
    printf("i=%d\n", i);
    if(DIST_SHUTDOWNS[i] != -1) gpio_write(pi, DIST_SHUTDOWNS[i], DISTANCE_ON); 
    //printf("changing address\n");
    adafruit_distance_change_address(dist_handles[4], DIST_ADDRS[i]);
    success = adafruit_distance_begin(dist_handles[i]);
    if(!success) printf("Distance sensor Error %d\n", i);

    for(j=0; j<100;j++){
      printf("%d: \t%d\n", i, adafruit_distance_readRange(dist_handles[i]));
    }
  }

  // Subtract these numbers from the thing
  // i 19
  // j 17
  // k 12.5

  // TODO: long distance

  /*
  adafruit_distance_change_address(short_A_handle, SHORT_ADDR_C);
  success = adafruit_distance_begin(short_C_handle)
  if(!success) printf("Distance sensor Error 1\n");
  
  gpio_write(pi, SHORT_SHUTDOWN_B, DISTANCE_ON);
  adafruit_distance_change_address(short_A_handle, SHORT_ADDR_B);
  success = adafruit_distance_begin(short_B_handle);
  if(!success) printf("Distance sensor Error 1\n");
  
  gpio_write(pi, SHORT_SHUTDOWN_A, 1);
  success = adafruit_distance_begin(short_A_handle);
  if(!success)
  */
  
  gyro_handle = i2c_open(pi, GYRO_BUS, GYRO_ADDR, 0);
  //acc_handle = i2c_open(ACC_BUS, ACC_ADDRESS);
  compass_handle = i2c_open(pi, COMPASS_BUS, COMPASS_ADDR, 0);

  // these init values are taken from the pololu library
  i2c_write_byte_data(pi, gyro_handle, CTRL1, 0x6F);
  i2c_write_byte_data(pi, gyro_handle, CTRL4, 0x00);
  i2c_write_byte_data(pi, gyro_handle, LOWODR, 0x00);

  i2c_write_byte_data(pi, compass_handle, CTRL5, 0x64);
  i2c_write_byte_data(pi, compass_handle, CTRL6, 0x20);
  i2c_write_byte_data(pi, compass_handle, CTRL7, 0x00);

  printf("compass_handle = %d\n", compass_handle);
  Sensor_calCompass(100);
  Sensor_calGyro(100);
  return 1; //success
}

/* Return angle in radians or radians/s. */

double Sensor_getGyro(){
  char buffer[32];
  int16_t raw;


  i2c_read_i2c_block_data(pi, gyro_handle, GYRO_REGISTER_OUT_X_L | 0x80, buffer, 6);
  raw = ((int16_t)(buffer[3]) << 8) + (int16_t)(buffer[2]);

  // needs to be 16 bits signed so the signs work out correctly

  return raw * RPS_PER_DIGIT - gyroOffset;
}

double Sensor_getCompass(){
  double raw = getCompassRaw();
  return fmod(((raw - compassOffset) + TWO_PI), TWO_PI);
}

/* Calibrates Current angle as "0", averaging over n readings */
void Sensor_calCompass(int n){
  int i;
  double center = getCompassRaw();
  double c;
  double offset = 0;
  for (i = 0; i < n; i++) {
    c = getCompassRaw();
    if (c - center > (TWO_PI/2)) {
      offset += c - TWO_PI;
    }
    else if (c - center < (-1*TWO_PI/2)) {
      offset += c + TWO_PI;
    }
    else {
      offset += c;
    }
  }
  compassOffset = offset / n;
}

void Sensor_calGyro(int n) {
  int i;
  double c = 0;

  for (i = 0; i < n; i++) {
    c += Sensor_getGyro();
  }

  gyroOffset = c / (double)n;
}

/* Return distance in cm */

double Sensor_getShort(int num){//Dir_t dir) {
  return adafruit_distance_readRange(dist_handles[num]);
}

double Sensor_getLong(enum Dir_t dir) {
  return 1;
}

/*
void Sensor_orient(enum Orient_t ori) {
  currentOrient = ori;
}
enum Orient_t Sensor_getOrient() {
  return currentOrient;
}
*/

/* Any Cleanup */
void Sensor_free(){
  int i;
  i2c_close(pi, gyro_handle);
  i2c_close(pi, compass_handle);

  // TODO: not sure whether it's a good idea to reset addresses before shutting down
  //i2c_close(pi, short_A_handle);
  //i2c_close(pi, short_B_handle);
  //i2c_close(pi, short_B_handle);
  for(i=0; i<5; i++){
    i2c_close(pi, dist_handles[i]);
  }
}
