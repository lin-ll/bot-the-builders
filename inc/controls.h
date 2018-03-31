/*----------------------------------------------------------------------------*/
/* controls.h 															      */
/* Control the high-level desired movement of the robot 					  */
/*----------------------------------------------------------------------------*/

#ifndef CTRL_H
#define CTRL_H

// initialize values
int Control_init();

// find the orientation of our robot
int Control_findDirection(int oldNode, int newNode);

// want to stop the robot
void Control_stop();

// returns if robot is in desired position
int Control_update();

// update the direction that we want the robot to move
void Control_moveTo(int direction);

// moves robot in pre-set direction
void Control_move();

#endif
