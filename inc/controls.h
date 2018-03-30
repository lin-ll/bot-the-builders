/*----------------------------------------------------------------------------*/
/* controls.h 															      */
/* Control the high-level desired movement of the robot 					  */
/*----------------------------------------------------------------------------*/

#ifndef CTRL_INCLUDED
#define CTRL_INCLUDED

// initialize PIDs
int setup();

// hold onto the size of the maze
int mazeSize;

// find the orientation of our robot
int findDirection(int oldNode, int newNode);

// assuming forward orientation, positive translation defined to the right,
// and positive rotations defined in clockwise direction
void adjust(int direction, double mainVelocity, double translate, double rotate);

// want to stop the robot
void stop();

// checks error of X
double xOffset();

// checks error of Y
double yOffset();

// checks error of theta
double thetaOffset();

// gets distance to stop at
double getStoppingDistance();

// move from the old node to the new node
void moveTo(int oldNode, int newNode);

#endif
