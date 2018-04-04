#include "sensors.h"


int main(){
	printf("Error %d is <%s>\n", -83, pigpio_error(-83));
	int pi = pigpio_start(0, 0);
	Sensor_initTiny(pi);
	Sensor_getEncoders();
	return 0;
}
