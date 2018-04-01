#include <pigpiod_if2.h>
#include "buttons.h"
#include <stdio.h>

int pi = 0;

int main() {
	pi = pigpio_start(NULL, NULL);
	Button_init(pi);

	while (1) {
		int which_button = Button_update();
		printf("Button: %d\n", which_button);
	}
}
