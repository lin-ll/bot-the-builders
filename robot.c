// The central file for controlling the robot
#include "maze.h"
#include "controls.h"
#include <unistd.h>
#include "buttons.h"
#include "constants.h"
#include "sensors.h"
#include "leds.h"
#include <stdio.h>
#include <pigpiod_if2.h>

void solveMaze(int goal) {
	Maze_assignPath(goal);

	int controls_finished = 0;

	while (true) {
		// check buttons
		int button_pressed = Button_update();
		if (button_pressed == BUTTON_RED) {
			Led_setColor(MAX_COLOR, 0, 0); // red
			Motor_completeStop();
			Maze_partialReset();
			usleep(300000);
			Led_off();
			solveMaze(GOAL_SPACE);
			return;
		}

		int dir;

		// do controls thing
		controls_finished = Control_update();
		if (controls_finished) {
			dir = Maze_followPath();
			if (dir == -1) {
				break;
			}
			Control_setDir(dir);
		}

		Control_update();
	}
}

int explore() {
	int dir;
	int controls_finished = 0;

	while (true) {
		// check buttons
		int button_pressed = Button_update();
		if (button_pressed == BUTTON_RED) {
			Led_setColor(MAX_COLOR, 0, 0); // red
			Motor_completeStop();
			Maze_reset();
			usleep(300000);
			Led_off();
			main();
			return -1;
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
				dir = Maze_dfs(up_wall, down_wall, left_wall, right_wall);
			} while (dir == -1 && !(Maze_isExplored()));
			if (Maze_isExplored()) {
				break;
			}
			Control_setDir(dir);
		}

		Control_update();
	}
	return 0;
}

int main() {
	// initialize everything
	int pi = pigpio_start(NULL, NULL);
	Button_init(pi);
	Led_init(pi);
	Sensor_init(pi);
	Control_init();
	Maze_init();

	printf("IN MAIN\n");

	Led_setColor(MAX_COLOR, MAX_COLOR, 0); //orange

	// green button triggers explore
	int button_pressed = -1;
	while (button_pressed != BUTTON_GREEN) {
		button_pressed = Button_update();
	}
	Led_setColor(0, MAX_COLOR, 0); //green
	int result = explore();
	Led_off();
	if (result == -1) {
		return -1;
	}

	// this shouldn't ever be executed if dfs works properly but just in case.
	if (!Maze_isAtStart()) {
		Led_setColor(0, MAX_COLOR, MAX_COLOR); //cyan
		solveMaze(START_SPACE);
		Led_off();
	}

	button_pressed = -1;
	while (button_pressed != BUTTON_BLUE) {
		button_pressed = Button_update();
	}

	Led_setColor(0, 0, MAX_COLOR); //blue
	solveMaze(GOAL_SPACE);
	Led_off();
	return 0;
}
