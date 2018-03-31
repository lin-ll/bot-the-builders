// The central file for controlling the robot

#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "controls.h"
#include "kalman.h"
#include "heap.h"
#include "buttons.h"
#include "constants.h"
#include "sensors.h"

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
		moveTo(currNode, nextNode); // TODO: what is moveTo???????
		currNode = nextNode;
	}
}

void returnToStart() {
		maze.assignPath(START_SPACE);
		while (1) {
				// check buttons

				// do controls thing
				controls_finished = Control_update();

				if(controls_finished){
						dir = maze.followPath();
						if (dir == -1) {
								Control_stop();
								return;
						}
						Control_moveTo(dir);
				}


				Control_move();

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
				controls_finished = Control_update();

				if(controls_finished){
						dir = maze.followPath();
						if (dir == -1) {
								Control_stop();
								return;
						}
						Control_moveTo(dir);
				}


				Control_move();

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
	Control_init();

	// from danstan
	float destX;
	float destY;

	int controls_finished = 1;

	while(!maze.isExplored()) {
		// check buttons
		int button_pressed = Button_update();
		if (button_pressed == BUTTON_RED) {
			Led_setColor(MAX_COLOR, 0, 0);
			// TODO: action for red button
			Led_off();
		}

		// do controls thing
		controls_finished = Control_update();
		if (controls_finished) {
			int *walls = Sensor_findWalls(walls);
			int up_wall = walls[0];
			int down_wall = walls[1];
			int left_wall = walls[2];
			int right_wall = walls[3];
			do {
				dir = maze.dfs(up_wall, down_wall, left_wall, right_wall);
			} while (dir == -1);

			Control_moveTo(dir);
		}

		Control_move();

		//// do Kalman thing
		// get all sensor values
		// call Kalman with sensor values
	}
	Control_stop();
}
