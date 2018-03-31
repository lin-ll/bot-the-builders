// The central file for controlling the robot

#include <stdio.h>
#include <stdlib.h>
#include "inc/maze.h"
#include "inc/controls.h"
#include "inc/kalman.h"
#include "inc/heap.h"

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

void traverseShortestPath(heap_t *path, int start) {
		int curr = start;
		while (!isEmpty(path)) {
				int next = pop(path);
				moveTo(curr, next);
				curr = next;
		}
}

void returnToStart(Graph g, int currNode) {
		int **distances = findShortestPath(g, currNode, startSpace);
		heap_t *path = getShortestPath(distances, startSpace);
		traverseShortestPath(path, currNode);
}

void solveMaze(Graph g) {
		int **distances = findShortestPath(g, startSpace, goalSpace);
		heap_t *path = getShortestPath(distances, goalSpace);
		traverseShortestPath(path, startSpace);
}

void main() {
	explore();
	returnToStart();
	solveMaze();



	// from danstan
	float destX;
	float destY;


	int controls_finished = 1;




	while(1) {
		// check buttons


		// do controls thing
		if(controls_finished){
			int *up_wall, *down_wall, *left_wall, *right_wall;
			contols.find_walls(up_wall, down_wall, left_wall, right_wall);
			dir = maze.dfs(up_wall, down_wall, left_wall, right_wall);

			if(dir == -1){
				// we did it!
				controls.stop();
				break;
			}

			controls.set_new_dest(dir);
		}
		

		controls_finished = controls.update();

		//// do Kalman thing
		// get all sensor values
		// call Kalman with sensor values
	}
}

