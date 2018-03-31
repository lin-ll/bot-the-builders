#include "buttons.h"
#include "constants.h"
#include "leds.h"
#include <pigpiod_if2.h>
#include <unistd.h>

//-------------------

const int BUTTON_PINS[4] = {BUTTON_RED, BUTTON_GREEN, BUTTON_BLUE, BUTTON_YELLOW};
static int pi;

int Button_init(int pifd) {
	pi = pifd;

	for (int i = 0; i < 4; i++) {
		// set the mode and if it fails, return 1
		if(set_mode(pi, BUTTON_PINS[i], PI_INPUT))
			return 1;
	}

	return 0;
}

int Button_update() {
	for (int i = 0; i < 4; i++) {
		if (gpio_read(pi, BUTTON_PINS[i]) == 0)
			return action(BUTTON_PINS[i]);
	}
	return -1;
}

int action(int which_button) {
	if (which_button == BUTTON_BLUE) {
		Led_setColor(0, 0, MAX_COLOR);
		// TODO: action
	} else if (which_button == BUTTON_GREEN) {
		Led_setColor(0, MAX_COLOR, 0);
		// TODO: action
	} else if (which_button == BUTTON_YELLOW) {
		Led_setColor(MAX_COLOR, MAX_COLOR, 0);
		// TODO: action
	}
	usleep(2000000);
	Led_off();
	return which_button;
}

