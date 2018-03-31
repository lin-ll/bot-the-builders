#include <pigpiod_if2.h>
#include "inc/constants.h"
#include "inc/leds.h"

int pi = 0;

int main() {
	pi = pigpio_start(NULL, NULL);
	Led_init(pi);

	int val = 255;
	int time = 2000000;
	int sleep = 500000;

	// red
	Led_setColor(val, 0, 0);
	usleep(time);
	Led_setColor(0, 0, 0);
	usleep(sleep);

	// green
	Led_setColor(0, val, 0);
	usleep(time);
	Led_setColor(0, 0, 0);
	usleep(sleep);

	// blue
	Led_setColor(0, 0, blue);
	usleep(time);
	Led_setColor(0, 0, 0);
	usleep(sleep);

	// red + green = yellow
	Led_setColor(val, val, 0);
	usleep(time);
	Led_setColor(0, 0, 0);
	usleep(sleep);

	// red + blue = magenta
	Led_setColor(val, 0, val);
	usleep(time);
	Led_setColor(0, 0, 0);
	usleep(sleep);

	// green + blue = cyan
	Led_setColor(0, val, val);
	usleep(time);
	Led_setColor(0, 0, 0);
	usleep(sleep);

	// red + green + blue = white
	Led_setColor(val, val, val);
	usleep(time);
	Led_setColor(0, 0, 0);
	usleep(sleep);
}
