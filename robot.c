// The central file for controlling the robot

#include <stdio.h>
#include <stdlib.h>
#include "inc/maze.h"
#include "inc/controls.h"
#include "inc/kalman.h"
<<<<<<< HEAD
#include "inc/heap.h"
#include "inc/buttons.h"
#include "inc/constants.h"

int explore() {
	int explorationVelocity = 10; // speed for exploration
	int row = 0;
	int col = 0;
	int currNode = getIntFromCoordinates(row, col);
	while (!isExplored()) {
		// gather information about walls from sensors
		int leftWall = 0;
		int rightWall = 0;
		int upWall = 0;
		int downWall = 0;
		int nextNode = dfs(row, col, leftWall, rightWall, upWall, downWall);
		moveTo(currNode, nextNode);
		currNode = nextNode;
	}
}
=======
#include "inc/sensors.h"

void returnToStart() {
		maze.assignPath(START_SPACE);
		while (1) {
				// check buttons

				// do controls thing
				controls_finished = controls.update();

				if(controls_finished){
						dir = maze.followPath();
						if (dir == -1) {
								controls.stop();
								return;
						}
						controls.moveTo(dir);
				}
>>>>>>> c632af1f11795d691e582fb689da7e623133ac89


				controls.move();

				// do Kalman thing
				// get all sensor values
				// call Kalman with sensor values
		}

}

void solveMaze() {
		maze.assignPath(GOAL_SPACE);
		while (1) {
				// check buttons

				// do controls thing
				controls_finished = controls.update();

				if(controls_finished){
						dir = maze.followPath();
						if (dir == -1) {
								controls.stop();
								return;
						}
						controls.moveTo(dir);
				}


				controls.move();

				// do Kalman thing
				// get all sensor values
				// call Kalman with sensor values
		}
}

void main() {
	explore();
	returnToStart();
	solveMaze();

	Button_init(/*TODO*/);
	controls_setup();

	// from danstan
	float destX;
	float destY;

	int controls_finished = 1;

	while(!maze.isExplored()) {
		// check buttons
		int button_pressed = Button_update();
		if (button_pressed == BUTTON_RED) {
			// TODO: action for red button
		}

		// do controls thing
		controls_finished = controls.update();
		if (controls_finished) {
			int *walls = Sensor_findWalls();
			int up_wall = walls[0];
			int down_wall = walls[1];
			int left_wall = walls[2];
			int right_wall = walls[3];
			do {
					dir = maze.dfs(up_wall, down_wall, left_wall, right_wall);
			} while (dir == -1);

			controls.moveTo(dir);
		}

		controls.move();

		//// do Kalman thing
		// get all sensor values
		// call Kalman with sensor values
	}
	controls.stop();
}
