// The central file for controlling the robot

#include <stdio.h>
#include <stdlib.h>
#include "inc/maze.h"
#include "inc/controls.h"
#include "inc/kalman.h"
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


				controls.move();

				// do Kalman thing
				// get all sensor values
				// call Kalman with sensor values
		}

}

void solveMaze(Graph g) {
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

  controls_setup();

	// from danstan
	float destX;
	float destY;

	int controls_finished = 1;




	while(!maze.isExplored()) {
		// check buttons

		// do controls thing
		controls_finished = controls.update();
		if(controls_finished){
			int *up_wall, *down_wall, *left_wall, *right_wall;
			contols.find_walls(up_wall, down_wall, left_wall, right_wall);

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
