#ifndef BUTTON_H
#define BUTTON_H

/**
 * Initialize buttons.
 * Return 0 on Success.
 * Return 1 on Failure
 **/
int Button_init(int pifd);

/* Return the button being pressed. Return -1 if all else fails */
int Button_update();

#endif