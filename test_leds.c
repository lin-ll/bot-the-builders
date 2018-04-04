#include <pigpiod_if2.h>
#include "leds.h"
#include "constants.h"
#include <unistd.h>

int pi = 0;

int main() {
	pi = pigpio_start(NULL, NULL);
	Led_init(pi);

	int time = 2000000;
	int sleep = 500000;

	// red
	Led_setColor(MAX_COLOR, 0, 0);
	usleep(time);
	Led_off();
	usleep(sleep);

	// green
	Led_setColor(0, MAX_COLOR, 0);
	usleep(time);
	Led_off();
	usleep(sleep);

	// blue
	Led_setColor(0, 0, MAX_COLOR);
	usleep(time);
	Led_off();
	usleep(sleep);

	// red + green = yellow
	Led_setColor(MAX_COLOR, MAX_COLOR, 0);
	usleep(time);
	Led_off();
	usleep(sleep);

	// red + blue = magenta
	Led_setColor(MAX_COLOR, 0, MAX_COLOR);
	usleep(time);
	Led_off();
	usleep(sleep);

	// green + blue = cyan
	Led_setColor(0, MAX_COLOR, MAX_COLOR);
	usleep(time);
	Led_off();
	usleep(sleep);

	// red + green + blue = white
	Led_setColor(MAX_COLOR, MAX_COLOR, MAX_COLOR);
	usleep(time);
	Led_off();
	usleep(sleep);
}
