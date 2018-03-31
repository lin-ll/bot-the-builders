#ifndef BUTTON_H
#define BUTTON_H

/**
 * Initialize buttons.
 * Return 0 on Success.
 * Return 1 on Failure
 **/
int Button_init(int pifd);

/* Return the button being pressed */
int Button_update();

/* Do an action for the button being pressed 
   as long as the button is not red */
int action(int which_button);

#endif