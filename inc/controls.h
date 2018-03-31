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

// update the direction that we want the robot to move
void moveTo(int direction);

// moves robot in pre-set direction
void move();

#endif
