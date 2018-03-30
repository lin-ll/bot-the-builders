/**************************************************************************/
/*! 
    @file     Adafruit_VL6180X.h
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

//#define I2C_DEBUG

#define VL6180X_REG_IDENTIFICATION_MODEL_ID    0x000
#define VL6180X_REG_SYSTEM_INTERRUPT_CONFIG    0x014
#define VL6180X_REG_SYSTEM_INTERRUPT_CLEAR     0x015
#define VL6180X_REG_SYSTEM_FRESH_OUT_OF_RESET  0x016
#define VL6180X_REG_SYSRANGE_START             0x018
#define VL6180X_REG_SYSALS_START               0x038
#define VL6180X_REG_SYSALS_ANALOGUE_GAIN       0x03F
#define VL6180X_REG_SYSALS_INTEGRATION_PERIOD_HI  0x040
#define VL6180X_REG_SYSALS_INTEGRATION_PERIOD_LO  0x041
#define VL6180X_REG_RESULT_ALS_VAL             0x050
#define VL6180X_REG_RESULT_RANGE_VAL           0x062
#define VL6180X_REG_RESULT_RANGE_STATUS        0x04d
#define VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO       0x04f
#define VL6180X_REG_I2C_SLAVE_DEVICE_ADDR      0x212

#define VL6180X_ALS_GAIN_1         0x06
#define VL6180X_ALS_GAIN_1_25      0x05
#define VL6180X_ALS_GAIN_1_67      0x04
#define VL6180X_ALS_GAIN_2_5       0x03
#define VL6180X_ALS_GAIN_5         0x02
#define VL6180X_ALS_GAIN_10        0x01
#define VL6180X_ALS_GAIN_20        0x00
#define VL6180X_ALS_GAIN_40        0x07

#define VL6180X_ERROR_NONE         0
#define VL6180X_ERROR_SYSERR_1     1
#define VL6180X_ERROR_SYSERR_5     5
#define VL6180X_ERROR_ECEFAIL      6
#define VL6180X_ERROR_NOCONVERGE   7
#define VL6180X_ERROR_RANGEIGNORE  8
#define VL6180X_ERROR_SNR          11
#define VL6180X_ERROR_RAWUFLOW     12
#define VL6180X_ERROR_RAWOFLOW     13
#define VL6180X_ERROR_RANGEUFLOW   14
#define VL6180X_ERROR_RANGEOFLOW   15

int adafruit_distance_begin(int sensor_handle);
int adafruit_distance_readRange(int sensor_handle);
float   adafruit_distance_readLux(int sensor_handle, int gain);
int adafruit_distance_readRangeStatus(int sensor_handle);
void adafruit_distance_change_address(int old, int new);
void adafruit_distance_set_pi_handle(int handle);
void adafruit_distance_write8(int address, int handle);
