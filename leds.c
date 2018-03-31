#include "inc/leds.h"
#include "inc/constants.h"
#include <pigpiod_if2.h>

const int LED_PINS[3] = {LED_GRAY, LED_BROWN, LED_ORANGE};
static int pi;

int Led_init(int pifd) {
	pi = pifd;

	for (int i = 0; i < 3; i++) {
		set_mode(pi, LED_PINS[i], PI_OUTPUT);
	}

	return 0;
}

// orange = green
// gray = blue
// brown = red
void Led_setColor(int red, int green, int blue) {
	set_PWM_dutycycle(pi, LED_BROWN, 255-red);
	set_PWM_dutycycle(pi, LED_ORANGE, 255-green);
	set_PWM_dutycycle(pi, LED_GRAY, 255-blue);
}