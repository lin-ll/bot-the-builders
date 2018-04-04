// initialized Controls2 pid and other instance variables
int Controls2_init();

// sets direction of movement
void Controls2_setDir();

// communicates with motors.c
// returns whether robot is at destination
int Controls2_update();
