// The central file for controlling the robot

#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "controls2.h"
#include "kalman.h"
#include "heap.h"
#include "buttons.h"
#include "constants.h"
#include "sensors.h"

void solveMaze(int goal) {
	Maze_assignPath(goal);

	int controls_finished = 1;

	while (true) {
		// check buttons
		int button_pressed = Button_update();
		if (button_pressed == BUTTON_RED) {
			Led_setColor(MAX_COLOR, 0, 0); // red
			Led_off();
			Motor_completeStop();
			Maze_reset();
			main();
			return;
		}

		int dir;

		// do controls thing
		controls_finished = Controls2_update();
		if (controls_finished == 0) {
			int *walls = Sensor_findWalls(walls);
			int up_wall = walls[0];
			int down_wall = walls[1];
			int left_wall = walls[2];
			int right_wall = walls[3];
			dir = Maze_followPath();
			if (dir == -1) break;
			Controls2_setDir(dir);
		}

		Controls2_update();
}

void main() {
	Led_setColor(255, 200, 0); //orange
	pi = pigpio_start(NULL, NULL);

	int button_pressed = -1;
	while (button_pressed != BUTTON_GREEN) {
			button_pressed = Button_update();
	}

	Button_init(pi);
	Sensor_init(pi);
	Controls2_init();
	Maze_init();

	Led_setColor(0, MAX_COLOR, 0); //green

	int dir;

	int controls_finished = 1;

	while (true) {
		// check buttons
		int button_pressed = Button_update();
		if (button_pressed == BUTTON_RED) {
			Led_setColor(MAX_COLOR, 0, 0); // red
			Led_off();
			Motor_completeStop();
			Maze_reset();
			main();
			return;
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
				if (Maze_isExplored()) break;
			} while (dir == -1);
			if (Maze_isExplored()) break;
			Controls2_setDir(dir);
		}

		Controls2_update();
	}

	// this shouldn't ever be executed if dfs works properly but just in case.
	if (!Maze_isAtStart()) {
			solveMaze(START_SPACE);
	}

	while (button_pressed != BUTTON_BLUE) {
			button_pressed = Button_update();
	}

	Led_setColor(0, 0, MAX_COLOR); // blue
	SolveMaze(GOAL_SPACE);
}
