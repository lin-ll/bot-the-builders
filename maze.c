/*----------------------------------------------------------------------------*/
/* Maze solving and traversal. Calls on general.c methods for sensor reading  */
/* and motor control.                                                         */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include "graph.h"
#include "heap.h"
#include "constants.h"

//TODO: create heap_destroy method?

static graph_p maze; // graph recording walls and such
static int traversed[MAZE_SIZE][MAZE_SIZE] = {0}; // record of traversed squares
#define MAZE_AREA (MAZE_SIZE * MAZE_SIZE) // size of maze
static int currPriority = MAZE_AREA + 1;

static int currRow = 0; // current row
static int currCol = 0; // current col

heap_t *stepTrace;

heap_t *currentPath; // path currently being traversed
static int distances[MAZE_SIZE][MAZE_SIZE] = {0}; // distances used in findShortestPath

int Maze_isAtStart() {
	return currRow == 0 && currCol == 0;
}

void Maze_partialReset() {
	currRow = 0;
	currCol = 0;
}

// converts row and column to label of node in graph
int Maze_getIntFromCoordinates(int row, int col) {
	return row * MAZE_SIZE + col;
}

// converts node to row and column
int getRowFromInt(int nodeRef) {
	int row = nodeRef / MAZE_SIZE;
	return row;
}

int getColFromInt(int nodeRef) {
	int col = nodeRef % MAZE_SIZE;
	return col;
}

void Maze_clearPath() {
	while (!isEmpty(currentPath)) {
		pop(currentPath);
	}
}

// Visualize the maze and parse it into a graph. Descritize each unit of maze
// into a vertex, and connect them with edges of unit length. Uses DFS.
void Maze_init() {
	maze = createGraph(MAZE_AREA, UNDIRECTED);
	// connect center squares
	addEdge(maze, 119, 120);
	addEdge(maze, 119, 135);
	addEdge(maze, 120, 136);
	addEdge(maze, 135, 136);
	currRow = 0;
	currCol = 0;
	stepTrace = heap_init();
	push(stepTrace, currPriority, START_SPACE);
}

void Maze_reset() {
	currRow = 0;
	currCol = 0;
	for (int i = 0; i < MAZE_SIZE; i++) {
		for (int j = 0; j < MAZE_SIZE; j++) {
			traversed[i][j] = 0;
		}
	}
	destroyGraph(maze);
}

void add_edges (int upWall, int downWall, int leftWall, int rightWall, int nodeRef) {
	if (currCol != 0 && leftWall == 0) {
		if (!hasEdge(maze, nodeRef, nodeRef - 1)) {
			addEdge(maze, nodeRef, nodeRef - 1);
		}
	}
	// right
	if (currCol != MAZE_SIZE - 1 && rightWall == 0) {
		if (!hasEdge(maze, nodeRef, nodeRef + 1)) {
			addEdge(maze, nodeRef, nodeRef + 1);
		}
	}
	// up
	if (currRow != 0 && upWall == 0) {
		if (!hasEdge(maze, nodeRef, nodeRef - MAZE_SIZE)) {
			addEdge(maze, nodeRef, nodeRef - MAZE_SIZE);
		}
	}
	// down
	if (currRow != MAZE_SIZE - 1 && downWall == 0) {
		if (!hasEdge(maze, nodeRef, nodeRef + MAZE_SIZE)) {
				addEdge(maze, nodeRef, nodeRef + MAZE_SIZE);
		}
	}
}

// implements depth first search
int Maze_dfs(int upWall, int downWall, int leftWall, int rightWall) {
	if (currRow < 0 || currRow >= MAZE_SIZE || currCol < 0 || currCol >= MAZE_SIZE) {
		return -1; // invalid row or col
	}

	if (traversed[currRow][currCol] == 0) {
		traversed[currRow][currCol] = 1;
		currPriority--;
	}

	int nodeRef = Maze_getIntFromCoordinates(currRow, currCol);

	add_edges(upWall, downWall, leftWall, rightWall, nodeRef);

	// left
	if (currCol != 0 && hasEdge(maze, nodeRef, nodeRef - 1)) {
		if (traversed[currRow][currCol - 1] == 0) {
			currCol--;
			push(stepTrace, currPriority, nodeRef);
			return LEFT;
		}
	}
	// right
	if (currCol != MAZE_SIZE - 1 && hasEdge(maze, nodeRef, nodeRef + 1)) {
		if (traversed[currRow][currCol + 1] == 0) {
			currCol++;
			push(stepTrace, currPriority, nodeRef);
			return RIGHT;
		}
	}
	// up
	if (currRow != 0 && hasEdge(maze, nodeRef, nodeRef - MAZE_SIZE)) {
		if (traversed[currRow - 1][currCol] == 0) {
			currRow--;
			push(stepTrace, currPriority, nodeRef);
			return UP;
		}
	}
	// down
	if (currRow != MAZE_SIZE - 1 && hasEdge(maze, nodeRef, nodeRef + MAZE_SIZE)) {
		if (traversed[currRow + 1][currCol] == 0) {
			currRow++;
			push(stepTrace, currPriority, nodeRef);
			return DOWN;
		}
	}

	int nextNode = pop(stepTrace);

	if (nextNode == nodeRef - MAZE_SIZE) {
		currRow--;
		return UP;
	} else if (nextNode == nodeRef + MAZE_SIZE) {
		currRow++;
		return DOWN;
	} else if (nextNode == nodeRef + 1) {
		currCol++;
		return RIGHT;
	} else if (nextNode == nodeRef - 1) {
		currCol--;
		return LEFT;
	}

	return -1;

}

// returns whether maze has been fully explored
int Maze_isExplored() {
	return Maze_isAtStart() && isEmpty(stepTrace);
}

// Find the shortest path given the graph representaion of the maze. Use
// Dijkstra's algorithm.
void findShortestPath(graph_p g, int start, int finish) {
	for (int i = 0; i < MAZE_SIZE; i++) {
		for (int j = 0; j < MAZE_SIZE; j++) {
			distances[i][j] = 10000;
		}
	}

	int startRow = getRowFromInt(start);
	int startCol = getColFromInt(start);
	distances[startRow][startCol] = 0;

	heap_t *unvisited = heap_init();

	int nodeRef = start;
	int row = getRowFromInt(nodeRef);
	int col = getColFromInt(nodeRef);

	while (nodeRef != finish) {

		// left
		if (col != 0 && hasEdge(maze, nodeRef, nodeRef - 1) == 0) {
			if (distances[row][col - 1] > distances[row][col] + 1) {
				distances[row][col - 1] = distances[row][col] + 1;
				push(unvisited, distances[row][col - 1], nodeRef - 1);
			}
		}
		// right
		if (col != MAZE_SIZE - 1 && hasEdge(maze, nodeRef, nodeRef + 1) == 0) {
			if (distances[row][col + 1] > distances[row][col] + 1) {
				distances[row][col + 1] = distances[row][col] + 1;
				push(unvisited, distances[row][col + 1], nodeRef + 1);
			}
		}
		// up
		if (row != 0 && hasEdge(maze, nodeRef, nodeRef - MAZE_SIZE) == 0) {
			if (distances[row - 1][col] > distances[row][col] + 1) {
				distances[row - 1][col] = distances[row][col] + 1;
				push(unvisited, distances[row - 1][col], nodeRef - MAZE_SIZE);
			}
		}
		// down
		if (row != MAZE_SIZE - 1 && hasEdge(maze, nodeRef, nodeRef + MAZE_SIZE) == 0) {
			if (distances[row + 1][col] > distances[row][col] + 1) {
				distances[row + 1][col] = distances[row][col] + 1;
				push(unvisited, distances[row + 1][col], nodeRef + MAZE_SIZE);
			}
		}

		nodeRef = pop(unvisited);
		row = getRowFromInt(nodeRef);
		col = getColFromInt(nodeRef);
	}
}

// Traverse the shortest path.
void getShortestPath(int **d, int finish) {
	int nodeRef = finish;
	int row = getRowFromInt(finish);
	int col = getRowFromInt(finish);
	int currDistance = d[row][col];
	heap_t *path = heap_init();
	push(path, currDistance, nodeRef);
	while (currDistance > 0) {
		row = getRowFromInt(nodeRef);
		col = getColFromInt(nodeRef);

		// left
		if (col != 0 && hasEdge(maze, nodeRef, nodeRef - 1) == 0) {
			if (d[row][col - 1] < d[row][col]) {
				push(path, d[row][col - 1], nodeRef - 1);
				currDistance = d[row][col - 1];
				nodeRef = nodeRef - 1;
				continue;
			}
		}

		// right
		if (col != MAZE_SIZE - 1 && hasEdge(maze, nodeRef, nodeRef + 1) == 0) {
			if (d[row][col + 1] < d[row][col]) {
				push(path, d[row][col + 1], nodeRef + 1);
				currDistance = d[row][col + 1];
				nodeRef = nodeRef + 1;
				continue;
			}
		}

		// up
		if (row != 0 && hasEdge(maze, nodeRef, nodeRef - MAZE_SIZE) == 0) {
			if (d[row - 1][col] < d[row][col]) {
				push(path, d[row - 1][col], nodeRef - MAZE_SIZE);
				currDistance = d[row - 1][col];
				nodeRef = nodeRef - MAZE_SIZE;
				continue;
			}
		}

		// down
		if (row != MAZE_SIZE - 1 && hasEdge(maze, nodeRef, nodeRef + MAZE_SIZE) == 0) {
			if (d[row + 1][col] < d[row][col]) {
				push(path, d[row + 1][col], nodeRef + MAZE_SIZE);
				currDistance = d[row + 1][col];
				nodeRef = nodeRef + MAZE_SIZE;
				continue;
			}
		}
	}

	currentPath = path;
}

void Maze_assignPath(int finish) {
	int currNode = Maze_getIntFromCoordinates(currRow, currCol);
	Maze_clearPath();
	findShortestPath(maze, currNode, finish);
	getShortestPath((int **)distances, finish);
}

// Follow current path, returns direction of next node
int Maze_followPath() {
	if (isEmpty(currentPath)) {
		return -1;
	}

	int nextNode = pop(currentPath);
	int currNode = Maze_getIntFromCoordinates(currRow, currCol);

	if (nextNode == currNode - MAZE_SIZE) {
		currRow--;
		return UP;
	} else if (nextNode == currNode + MAZE_SIZE) {
		currRow++;
		return DOWN;
	} else if (nextNode == currNode + 1) {
		currCol++;
		return RIGHT;
	} else if (nextNode == currNode - 1) {
		currCol--;
		return LEFT;
	}

	return -1;
}
