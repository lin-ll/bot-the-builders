#include <stdio.h>
#include <pigpiod_if2.h>

	#define ADDR 0x04
	//						addr 	addr 	start 	read 	1 byte 	stop	end
	const char commands[] = {0x04, 	ADDR,	0x02,	0x06,	0x01,	0x03, 	0x00}
	const int lenCommands = 7;
int main(){
        int pi = pigpio_start(0, 0);

        char outbuf[2] = {0x42, 0x43};
        int handle = i2c_open(pi, 1, 0x4, 0);
        //int read = i2c_read_byte(pi, handle);

        int open = bb_i2c_open(pi, SDA, SCL, 1000);
        int zip = bb_i2c_zip(pi, SDA, commands, len_commands, outbuf, 1);
        int close = bb_i2c_close(pi, SDA);

        printf("open\tzip\tclose\tdata\tcheck\n");
        printf("%d\t%d\t%d\t%d\t%d\n", open, zip, close, outbuf[0], outbuf[1]);


        printf("%d\n", read);

        /*
        printf("Error %d is <%s>\n", -83, pigpio_error(-83));
        int pi = pigpio_start(0, 0);
        Sensor_initTiny(pi);
        Sensor_getEncoders();
        return 0;
        */

}
