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
	int currNode = Maze_getIntFromCoordinates(row, col);
	while (!Maze_isExplored()) {
		// gather information about walls from sensors
		int leftWall = 0;
		int rightWall = 0;
		int upWall = 0;
		int downWall = 0;
		int nextNode = Maze_dfs(row, col, leftWall, rightWall, upWall, downWall);
		moveTo(currNode, nextNode); // TODO: what is moveTo???????
		currNode = nextNode;
	}
}

void returnToStart() {
	Maze_assignPath(START_SPACE);
	while (1) {
		// check buttons

		// do controls thing
		controls_finished = Control_update();

		if(controls_finished){
			dir = Maze_followPath();
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
	Maze_assignPath(GOAL_SPACE);
	while (1) {
		// check buttons

		// do controls thing
		controls_finished = Control_update();

		if(controls_finished){
			dir = Maze_followPath();
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

	Led_setColor(255, 200, 0); //orange

	pi = pigpio_start(NULL, NULL);

	int button_pressed = -1;
	while (button_pressed != BUTTON_GREEN) {
			// do nothing
	}

	Button_init(pi);
	Sensor_init(pi);
	Controls2_init();
	Maze_init();

	// from danstan
	float destX;
	float destY;

	Led_setColor(0, MAX_COLOR, 0); //green

	int controls_finished = 1;
	while (!Maze_isExplored()) {
		// check buttons
		int button_pressed = Button_update();
		if (button_pressed == BUTTON_RED) {
			Led_setColor(MAX_COLOR, 0, 0); // red
			Led_off();
			Maze_reset();
		}

		// do controls thing
		controls_finished = Controls2_update();
		if (controls_finished == 0) {
			int *walls = Sensor_findWalls(walls);
			int up_wall = walls[0];
			int down_wall = walls[1];
			int left_wall = walls[2];
			int right_wall = walls[3];
			do {
				dir = Maze_dfs(up_wall, down_wall, left_wall, right_wall);
			} while (dir == -1);
			Controls2_setDir(dir);
		}

		Control_move();
	}

	Control_stop();
}
