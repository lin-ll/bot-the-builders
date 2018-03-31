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

  controls_setup();

	// from danstan
	float destX;
	float destY;

	int controls_finished = 1;




	while(!maze.isExplored()) {
		// check buttons

		// do controls thing
		controls_finished = controls.update();
		if (controls_finished) {
			int walls[4] = {0, 0, 0, 0};
			Sensor_findWalls(walls);
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
