#include "leds.h"
#include "constants.h"
#include <pigpiod_if2.h>

const int LED_PINS[3] = {LED_BLUE, LED_RED, LED_GREEN};
static int pi;

int Led_init(int pifd) {
	pi = pifd;

	for (int i = 0; i < 3; i++) {
		set_mode(pi, LED_PINS[i], PI_OUTPUT);
	}

	return 0;
}

void Led_setColor(int red, int green, int blue) {
	set_PWM_dutycycle(pi, LED_RED, 255-red);
	set_PWM_dutycycle(pi, LED_GREEN, 255-green);
	set_PWM_dutycycle(pi, LED_BLUE, 255-blue);
}

void Led_off() {
	Led_setColor(0, 0, 0);
}