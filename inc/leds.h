#ifndef LED_H
#define LED_H

/**
 * Initialize LEDs.
 * Return 0 on Success.
 * Return 1 on Failure
 **/
int Led_init(int pifd);

/* Set LED pins to these RGB values */
void Led_setColor(int red, int green, int blue);

/* Turns all LED pins off */
void Led_off();

#endif