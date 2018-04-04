#ifndef I2CUTIL_H
#define I2CUTIL_H

int I2cutil_read8(int handle, int address);
void I2cutil_write8(int handle, int address, int data);

#endif