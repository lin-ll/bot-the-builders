/*----------------------------------------------------------------------------*/
/* controls.h 															      */
/* Control the high-level desired movement of the robot 					  */
/*----------------------------------------------------------------------------*/

#ifndef CTRL_INCLUDED
#define CTRL_INCLUDED

// initialize values
int controls_setup();

// find the orientation of our robot
int findDirection(int oldNode, int newNode);

// want to stop the robot
void stop();

// returns if robot is in desired position
int update();

// move from the old node to the new node
void moveTo(int oldNode, int newNode);

// moves robot
void move();

#endif
