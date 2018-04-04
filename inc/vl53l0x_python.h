#ifndef VL53L0X_PYTHON_H
#define VL53L0X_PYTHON_H

void startRanging(int object_number, int mode, uint8_t i2c_address, uint8_t TCA9548A_Device, uint8_t TCA9548A_Address);
int32_t getDistance(int object_number);
void stopRanging(int object_number);

#endif