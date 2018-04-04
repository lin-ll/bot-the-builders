#include "i2cutil.h"
#include <pigpiod_if2.h>

int I2cutil_read8(int handle, int address) {
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

void I2cutil_write8(int handle, int address, int data) {
	char data_write[3];
	data_write[0] = (address >> 8) & 0xFF;; // MSB of register address
	data_write[1] = address & 0xFF; // LSB of register address
	data_write[2] = data & 0xFF;

	int write = i2c_write_device(pi, handle, data_write, 3);
	if (write != 0)
		printf("Write returned %d\n", write);

	int result = I2cutil_read8(handle, address);
	printf("Wrote %d and read back %d\n", data, result);
}
