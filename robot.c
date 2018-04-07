// The central file for controlling the robot
#include "maze.h"
#include "controls.h"
#include <unistd.h>
#include "buttons.h"
#include "constants.h"
#include "sensors.h"
#include "leds.h"
#include "motors.h"
#include <stdio.h>
#include <pigpiod_if2.h>
#include "kalman.h"
#include <math.h>

int main();

clock_t prevTime;

void solveMaze(int goal) {
	Maze_assignPath(goal);

	int controls_finished = 0;

	clock_t prevTime = clock();
	while (1) {
		clock_t currentTime = clock();
		double dt = (double)(currentTime - prevTime)/CLOCKS_PER_SEC*4.454;
		prevTime = currentTime;

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
		controls_finished = Control_update(dt);
		if (controls_finished) {
			dir = Maze_followPath();
			if (dir == -1) {
				break;
			}
			Control_setDir(dir);
		}

		Kalman_update(dt, currentTime);
	}
}

int explore() {
	printf("Entering explore\n");
	// green button triggers explore
	int button_pressed = -1;
	while (button_pressed != BUTTON_GREEN) {
		button_pressed = Button_update();
	}
	Led_setColor(0, MAX_COLOR, 0); //green
	//printf("Explore\n");
	int dir;
	int controls_finished = 0;

	Control_reset();
	printf("Done control reset\n");
	//Maze_init();
	Kalman_init();
	printf("Done kalman init\n");

	clock_t prevTime = clock();
	while (1) {
		//printf("Top of while\n");
		clock_t currentTime = clock();
		double dt = (double)(currentTime - prevTime)/CLOCKS_PER_SEC*4.454;
		prevTime = currentTime;
		// check buttons
		int button_pressed = Button_update();
		if (button_pressed == BUTTON_RED) {
			Led_setColor(MAX_COLOR, 0, 0); // red
			Motor_completeStop();
			Maze_reset();
			usleep(300000);
			return explore();
		}

		clock_t diff = clock() - currentTime;
		printf("A %d\n", diff);

		//printf("Debug A\n");
		// do controls thing
		controls_finished = Control_update(dt);

		diff = clock() - currentTime;
		printf("B %d\n", diff);
		if (controls_finished) {

			//printf("Debug B\n");
			int walls[4];
			Sensor_findWalls(walls);
			int up_wall = walls[0];
			int down_wall = walls[1];
			int left_wall = walls[2];
			int right_wall = walls[3]; /*
			do {
				dir = Maze_dfs(up_wall, down_wall, left_wall, right_wall);
			} while (dir == -1 && !(Maze_isExplored()));
			if (Maze_isExplored()) {
				break;
			}

			Control_setDir(dir);
			*/
			Control_setDir(0);
		}

		diff = clock() - currentTime;
		printf("C %d\n", diff);
		//printf("Debug C\n");

		Kalman_update(dt, currentTime);

		diff = clock() - currentTime;
		printf("D %d\n", diff);
	}
	return 0;
}

int trash() {
	printf("Entering trash\n");
	// green button triggers explore
	int button_pressed = -1;
	while (button_pressed != BUTTON_GREEN) {
		button_pressed = Button_update();
	}
	Led_setColor(0, MAX_COLOR, 0); //green

	int dir = NORTH;

	while (1) {
		double sDist = Sensor_getShort(dir);
		Control_update2();

		if(!isnan(sDist) && sDist <= 90){
			printf("We're there!\n");
			// we're there!

			double dist = Sensor_getLong(NORTH);
			if(!isnan(dist) && dist > 180) {
				dir = NORTH;
				Control_setDir(dir);
				continue;
			}

			dist = Sensor_getLong(EAST);
			if(!isnan(dist) && dist > 180) {
				dir = EAST;
				Control_setDir(dir);
				continue;
			}

			dist = Sensor_getLong(WEST);
			if(!isnan(dist) && dist > 180) {
				dir = WEST;
				Control_setDir(dir);
				continue;
			}

			dist = Sensor_getLong(SOUTH);
			if(!isnan(dist) && dist > 180) {
				dir = SOUTH;
				Control_setDir(dir);
				continue;
			}
		}
	}
}

int main() {
	// initialize everything
	int pi = pigpio_start(NULL, NULL);
	Button_init(pi);
	Led_init(pi);
	Sensor_init(pi);
	Control_init();

	printf("IN MAIN\n");

	Led_setColor(MAX_COLOR, MAX_COLOR, 0); //orange

	int result = trash();

	// int result = explore();
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

	int button_pressed = -1;

	while (button_pressed != BUTTON_BLUE) {
		button_pressed = Button_update();
	}

	Led_setColor(0, 0, MAX_COLOR); //blue
	solveMaze(GOAL_SPACE);
	Led_off();
	return 0;
}
