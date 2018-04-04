#include "buttons.h"
#include "constants.h"
#include "leds.h"
#include <pigpiod_if2.h>
#include <unistd.h> // TODO: remove

//-------------------

const int BUTTON_PINS[4] = {BUTTON_RED, BUTTON_GREEN, BUTTON_BLUE, BUTTON_YELLOW};
static int pi;

int action(int which_button) {
	switch (which_button) {
		case BUTTON_BLUE:
			Led_setColor(0, 0, MAX_COLOR);
			// TODO: action
			break;
		case BUTTON_GREEN:
			Led_setColor(0, MAX_COLOR, 0);
			// TODO: action
			break;
		case BUTTON_YELLOW:
			Led_setColor(MAX_COLOR, MAX_COLOR, 0);
			// TODO: action
			break;
		case BUTTON_RED:
			Led_setColor(MAX_COLOR, 0, 0);
			// TODO: action
			break;
	}
	usleep(2000000); // TODO: remove 
	Led_off();
	return which_button;
}

int Button_init(int pifd) {
	pi = pifd;
	for (int i = 0; i < 4; i++) {
		// set the mode and if it fails, return 1
		set_mode(pi, BUTTON_PINS[i], PI_INPUT);
		set_pull_up_down(pi, BUTTON_PINS[i], PI_PUD_UP);
	}
	set_mode(pi, 24, PI_OUTPUT);
	gpio_write(pi, 24, 0);
	return 0;
}

int Button_update() {
	for (int i = 0; i < 4; i++) {
		if (gpio_read(pi, BUTTON_PINS[i]) == 0)
			return action(BUTTON_PINS[i]);
	}
	return -1;
}
