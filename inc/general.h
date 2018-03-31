/*
* general.h has all the methods for making the command from maze.c
* to actually making the robot move using motors.c
*/
#ifndef GEN_INCLUDED
#define GEN_INCLUDED
// moves a given distance c forwards
void forwards(int c);

// moves left distance c
void left(int c);

// moves right distance c
void right(int c);

// moves backwards distance c
void back(int c);

// reads distance from long
double checkDistanceFront();

// reads distance from the back
double checkDistanceBack();

// reads distance of sensor on left
double checkDistanceLeft();

// reads distance of sensor on right
double checkDistanceRight();

// maintains robot in center of path and correct orientation
void maintainLR();

void maintainFB();

void maintainTheta();

// returns value read from sensor s
double sensor(int s);


// checks if there is Wall in moving direction
int checkWall();

#endif
