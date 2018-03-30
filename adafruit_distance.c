/**************************************************************************/
/*! 
    @file     Adafruit_VL6180X.cpp
    @author   Limor Fried (Adafruit Industries)
    @license  BSD (see license.txt)
    
    This is a library for the Adafruit VL6180 ToF Sensor breakout board
    ----> http://www.adafruit.com/products/3316
    
    Adafruit invests time and resources providing this open source code, 
    please support Adafruit and open-source hardware by purchasing 
    products from Adafruit!

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#include <adafruit_distance.h>
#include <pigpiod_if2.h>
#include <stdio.h>

int pi_handle = 0;
int current_handle = 0;

void adafruit_distance_set_pi_handle(int pi_handle_1) {
  pi_handle = pi_handle_1;
}


/**************************************************************************/
/*! 
    @brief  I2C low level interfacing
*/
/**************************************************************************/

// Read 1 byte from the VL6180X at 'address'
int adafruit_distance_read8(int address)
{
  char dataWrite[2];
  char dataRead[1];

  dataWrite[0] = (address >> 8) & 0xFF;
  dataWrite[1] = address & 0xFF;
  int w = i2c_write_device(pi_handle, current_handle, dataWrite, 2);
  int r = i2c_read_device(pi_handle, current_handle, dataRead, 1);

  if(w != 0) printf("Write Code: %d\n", w);
  if(r != 1) printf("Read Code: %d\n", r);

  return (int)dataRead[0];
}


// Read 2 byte from the VL6180X at 'address'
int adafruit_distance_read16(int address)
{
  return i2c_read_word_data(pi_handle, current_handle, address);
}

// write 1 byte
void adafruit_distance_write8(int address, int data)
{
  int test;
  char data_write[3];
  data_write[0] = (address >> 8) & 0xFF;; // MSB of register address
  data_write[1] = address & 0xFF; // LSB of register address
  data_write[2] = data & 0xFF;
  test = i2c_write_device(pi_handle, current_handle, data_write, 3);
  if (test != 0) printf("write returned %d\n", test);
}


// write 2 bytes
void adafruit_distance_write16(int address, int data)
{
  int i;
  i2c_write_word_data(pi_handle, current_handle, address, data);
  printf("Writing data %x to address %x returned %d\n", data, address, i);
}

void adafruit_distance_change_address(int old_handle, int new_address){
  current_handle = old_handle;
  adafruit_distance_write8(VL6180X_REG_I2C_SLAVE_DEVICE_ADDR, new_address);
}


/**************************************************************************/
/*! 
    @brief  Load the settings for ranging
*/
/**************************************************************************/

void adafruit_distance_loadSettings(void) {
  // load settings!

  // private settings from page 24 of app note
  adafruit_distance_write8(0x0207, 0x01);
  adafruit_distance_write8(0x0208, 0x01);
  adafruit_distance_write8(0x0096, 0x00);
  adafruit_distance_write8(0x0097, 0xfd);
  adafruit_distance_write8(0x00e3, 0x00);
  adafruit_distance_write8(0x00e4, 0x04);
  adafruit_distance_write8(0x00e5, 0x02);
  adafruit_distance_write8(0x00e6, 0x01);
  adafruit_distance_write8(0x00e7, 0x03);
  adafruit_distance_write8(0x00f5, 0x02);
  adafruit_distance_write8(0x00d9, 0x05);
  adafruit_distance_write8(0x00db, 0xce);
  adafruit_distance_write8(0x00dc, 0x03);
  adafruit_distance_write8(0x00dd, 0xf8);
  adafruit_distance_write8(0x009f, 0x00);
  adafruit_distance_write8(0x00a3, 0x3c);
  adafruit_distance_write8(0x00b7, 0x00);
  adafruit_distance_write8(0x00bb, 0x3c);
  adafruit_distance_write8(0x00b2, 0x09);
  adafruit_distance_write8(0x00ca, 0x09);
  adafruit_distance_write8(0x0198, 0x01);
  adafruit_distance_write8(0x01b0, 0x17);
  adafruit_distance_write8(0x01ad, 0x00);
  adafruit_distance_write8(0x00ff, 0x05);
  adafruit_distance_write8(0x0100, 0x05);
  adafruit_distance_write8(0x0199, 0x05);
  adafruit_distance_write8(0x01a6, 0x1b);
  adafruit_distance_write8(0x01ac, 0x3e);
  adafruit_distance_write8(0x01a7, 0x1f);
  adafruit_distance_write8(0x0030, 0x00);

  // Recommended : Public registers - See data sheet for more detail
  adafruit_distance_write8(0x0011, 0x10);       // Enables polling for 'New Sample ready'
  // when measurement completes
  adafruit_distance_write8(0x010a, 0x30);       // Set the averaging sample period
  // (compromise between lower noise and
  // increased execution time)
  adafruit_distance_write8(0x003f, 0x46);       // Sets the light and dark gain (upper
  // nibble). Dark gain should not be
  // changed.
  adafruit_distance_write8(0x0031, 0xFF);       // sets the # of range measurements after
  // which auto calibration of system is
  // performed
  adafruit_distance_write8(0x0040, 0x63);       // Set ALS integration time to 100ms
  adafruit_distance_write8(0x002e, 0x01);       // perform a single temperature calibration
  // of the ranging sensor

  // Optional: Public registers - See data sheet for more detail
  adafruit_distance_write8(0x001b, 0x09);       // Set default ranging inter-measurement
  // period to 100ms
  adafruit_distance_write8(0x003e, 0x31);       // Set default ALS inter-measurement period
  // to 500ms
  adafruit_distance_write8(0x0014, 0x24);       // Configures interrupt on 'New Sample
  // Ready threshold event'
}


/**************************************************************************/
/*! 
    @brief  Single shot ranging
*/
/**************************************************************************/

int adafruit_distance_readRange(int sensor_handle) {
  current_handle = sensor_handle;

  int readOutput;
  while (! ((readOutput=adafruit_distance_read8(VL6180X_REG_RESULT_RANGE_STATUS)) & 0x01)) {
    printf("readOutput = %x", readOutput); fflush(stdout);
  }

  // Start a range measurement
  adafruit_distance_write8(VL6180X_REG_SYSRANGE_START, 0x01);

  // Poll until bit 2 is set
  while (! (adafruit_distance_read8(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO) & 0x04));

  // read range in mm
  int range = adafruit_distance_read8(VL6180X_REG_RESULT_RANGE_VAL);

  // clear interrupt
  adafruit_distance_write8(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);

  return range;
}


/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
int adafruit_distance_begin(int sensor_handle) {
  int count = 0;
  current_handle = sensor_handle;


  if (adafruit_distance_read8(VL6180X_REG_IDENTIFICATION_MODEL_ID) != 0xB4) {
    return 0;
  }

  //if (adafruit_distance_read8(VL6180X_REG_SYSTEM_FRESH_OUT_OF_RESET) == 0x01) {
  adafruit_distance_loadSettings();
  //}

  adafruit_distance_write8(VL6180X_REG_SYSTEM_FRESH_OUT_OF_RESET, 0x00);

  return 1;
}


/**************************************************************************/
/*! 
    @brief  Error message (retreive after ranging)
*/
/**************************************************************************/

int adafruit_distance_readRangeStatus(int sensor_handle) {
  current_handle = sensor_handle;
  return (adafruit_distance_read8(VL6180X_REG_RESULT_RANGE_STATUS) >> 4);
}


/**************************************************************************/
/*! 
    @brief  Single shot ranging
*/
/**************************************************************************/

float adafruit_distance_readLux(int sensor_handle, int gain) {
  current_handle = sensor_handle;
  int reg;

  reg = adafruit_distance_read8(VL6180X_REG_SYSTEM_INTERRUPT_CONFIG);
  reg &= ~0x38;
  reg |= (0x4 << 3); // IRQ on ALS ready
  adafruit_distance_write8(VL6180X_REG_SYSTEM_INTERRUPT_CONFIG, reg);

  // 100 ms integration period
  adafruit_distance_write8(VL6180X_REG_SYSALS_INTEGRATION_PERIOD_HI, 0);
  adafruit_distance_write8(VL6180X_REG_SYSALS_INTEGRATION_PERIOD_LO, 100);

  // analog gain
  if (gain > VL6180X_ALS_GAIN_40) {
    gain = VL6180X_ALS_GAIN_40;
  }
  adafruit_distance_write8(VL6180X_REG_SYSALS_ANALOGUE_GAIN, 0x40 | gain);

  // start ALS
  adafruit_distance_write8(VL6180X_REG_SYSALS_START, 0x1);

  // Poll until "New Sample Ready threshold event" is set
  while (4 != ((adafruit_distance_read8(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO) >> 3) & 0x7));

  // read lux!
  float lux = adafruit_distance_read16(VL6180X_REG_RESULT_ALS_VAL);

  // clear interrupt
  adafruit_distance_write8(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);

  lux *= 0.32; // calibrated count/lux
  switch(gain) {
  case VL6180X_ALS_GAIN_1:
    break;
  case VL6180X_ALS_GAIN_1_25:
    lux /= 1.25;
    break;
  case VL6180X_ALS_GAIN_1_67:
    lux /= 1.76;
    break;
  case VL6180X_ALS_GAIN_2_5:
    lux /= 2.5;
    break;
  case VL6180X_ALS_GAIN_5:
    lux /= 5;
    break;
  case VL6180X_ALS_GAIN_10:
    lux /= 10;
    break;
  case VL6180X_ALS_GAIN_20:
    lux /= 20;
    break;
  case VL6180X_ALS_GAIN_40:
    lux /= 20;
    break;
  }
  lux *= 100;
  lux /= 100; // integration time in ms


  return lux;
}

