#include <stdio.h>
#include <pigpiod_if2.h>
#include <unistd.h>

#define ADDR 0x04
#define SCL 19 // green button
#define SDA 26 // yellow button


//			addr 	addr 	start 	write 	1 byte	junk 	start	read	1 byte	stop	end
char commands2[] = {	0x04, 	ADDR,	0x02,	0x07,	0x01,	0x55,	0x02, 	0x06,	0x01,	0x03,	0x00};
int lenCommands2 = 11;

//			addr 	addr 	start 	read	1 byte	stop	end
char commands[] = {	0x04, 	ADDR,	0x02,	0x06,	0x01,	0x03,	0x00};
int lenCommands = 7;

//			addr 	addr 	start 	write 	1 byte	junk 	start	read	1 byte	start	read	1 byte 	start	read 1 byte	stop	end
char commands3[] = {	0x04, 	ADDR,	0x02,	0x07,	0x01,	0x55,	0x02, 	0x06,	0x01,	0x02,	0x06,	0x01,	0x02,	0x06,	0x01,	0x03,	0x00};
int lenCommands3 = 17;

int main(){
        int pi = pigpio_start(0, 0);
	//gpio_write(pi, SDA, 1);
	//gpio_write(pi, SCL, 1);
	
	//sleep(1);

        char outbuf[2] = {0x42, 0x43};
        //int handle = i2c_open(pi, 1, 0x4, 0);
        //int read = i2c_read_byte(pi, handle);

	set_mode(pi, SCL, PI_INPUT);
	set_mode(pi, SDA, PI_INPUT);

	//sleep(1);


        int open = bb_i2c_open(pi, SDA, SCL, 50);
        int zip = bb_i2c_zip(pi, SDA, commands3, lenCommands3, outbuf, 1);
        int close = bb_i2c_close(pi, SDA);

        printf("open\tzip\tclose\tdata\tcheck\n");
        printf("%d\t%d\t%d\t%d\t%d\n", open, zip, close, outbuf[0], outbuf[1]);


        //printf("%d\n", read);

        /*
        printf("Error %d is <%s>\n", -83, pigpio_error(-83));
        int pi = pigpio_start(0, 0);
        Sensor_initTiny(pi);
        Sensor_getEncoders();
        return 0;
        */

}

