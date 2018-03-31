/*----------------------------------------------------------------------------*/
/* Maze solving and traversal. Calls on general.c methods for sensor reading  */
/* and motor control.                                                         */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include "graph.h"
#include "heap.h"
#include "constant.h"

static Graph maze; // graph recording walls and such
static int traversed[16][16] = {0}; // record of traversed squares
static int unexplored = 256; // number of unexplored squares

static int currRow = 0; // current row
static int currCol = 0; // current col

heap_t *currentPath; // path currently being traversed
static int distances[16][16] = {0}; // distances used in findShortestPath

// Visualize the maze and parse it into a graph. Descritize each unit of maze
// into a vertex, and connect them with edges of unit length. Uses DFS.
Graph initializeMaze() {
		maze = graph_create(mazeSize * mazeSize);
		// connect center squares
		graph_add_edge(maze, 119, 120);
		graph_add_edge(maze, 119, 135);
		graph_add_edge(maze, 120, 136);
		graph_add_edge(maze, 135, 136);
}

void maze_reset() {
		currRow = 0;
		currCol = 0;
		unexplored = 256;
		for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
						traversed[i][j] = 0;
				}
		}
		graph_destroy(maze);
		initializeMaze();
}

void add_edges (int upWall, int downWall, int leftWall, int rightWall, int nodeRef) {
		if (col != 0 && leftWall == 0) {
			if (!graph_has_edge(maze, nodeRef, nodeRef - 1)) {
					graph_add_edge(maze, nodeRef, nodeRef - 1);
			}
		}
		// right
		if (col != mazeSize - 1 && rightWall == 0) {
			if (!graph_has_edge(maze, nodeRef, nodeRef + 1)) {
					graph_add_edge(maze, nodeRef, nodeRef + 1);
			}
		}
		// up
		if (row != 0 && upWall == 0) {
			if (!graph_has_edge(maze, nodeRef, nodeRef - mazeSize)) {
					graph_add_edge(maze, nodeRef, nodeRef - mazeSize);
			}
		}
		// down
		if (row != mazeSize - 1 && downWall == 0) {
				if (!graph_has_edge(maze, nodeRef, nodeRef + mazeSize)) {
						graph_add_edge(maze, nodeRef, nodeRef + mazeSize);
				}
		}
}

// implements depth first search
int dfs(int upWall, int downWall, int leftWall, int rightWall) {
		if (currRow < 0 || currRow >= mazeSize) {
				return -1; // invalid row
		}
		if (currCol < 0 || currCol >= mazeSize) {
				return -1; // invalid col
		}

		// if square has already been travelled to
		if (traversed[currRow][currCol] == 1) {
				return -1;
		}

		traversed[currRow][currCol] = 1;
		unexplored--;

		int nodeRef = getIntFromCoordinates(currRow, currCol);

		add_edges(upWall, downWall, leftWall, rightWall, nodeRef);

		if (isExplored()) {
				return -2;
		}

		// left
		if (col != 0 && leftWall == 0) {
				currCol--;
				return LEFT;
		}
		// right
		if (col != mazeSize - 1 && rightWall == 0) {
				currCol++;
				return RIGHT;
		}
		// up
		if (row != 0 && upWall == 0) {
				currRow++;
				return UP;
		}
		// down
		if (row != mazeSize - 1 && downWall == 0) {
				currRow--;
				return DOWN;
		}
}

// converts row and column to label of node in graph
int getIntFromCoordinates(int row, int col) {
		return row * mazeSize + col;
}

// converts node to row and column
int getRowFromInt(int nodeRef) {
		int row = nodeRef / mazeSize;
		return row;
}

int getColFromInt(int nodeRef) {
		int col = nodeRef % mazeSize;
		return col;
}

// returns whether maze has been fully explored
int isExplored() {
		return unexplored == 0;
}

// Find the shortest path given the graph representaion of the maze. Use
// Dijkstra's algorithm.
void findShortestPath(Graph g, int start, int finish) {
		for (int i = 0; i < mazeSize; i++) {
				for (int j = 0; j < mazeSize; j++) {
						distances[i][j] = 10000;
				}
		}

		int startRow = getRowFromInt(start);
		int startCol = getColFromInt(start);
		distances[startRow][startCol] = 0;

		heap_t *visited;
		heap_t *unvisited;

		int nodeRef = start;
		int row = getRowFromInt(nodeRef);
		int col = getColFromInt(nodeRef);

		while (nodeRef != finish) {

				// left
				if (col != 0 && graph_has_edge(g, nodeRef, nodeRef - 1) == 0) {
						if (distances[row][col - 1] > distances[row][col] + 1) {
								distances[row][col - 1] = distances[row][col] + 1;
								push(unvisited, distances[row][col - 1], nodeRef - 1);
						}
				}
				// right
				if (col != mazeSize - 1 && graph_has_edge(g, nodeRef, nodeRef + 1) == 0) {
						if (distances[row][col + 1] > distances[row][col] + 1) {
								distances[row][col + 1] = distances[row][col] + 1;
								push(unvisited, distances[row][col + 1], nodeRef + 1);
						}
				}
				// up
				if (row != 0 && graph_has_edge(g, nodeRef, nodeRef - mazeSize) == 0) {
						if (distances[row - 1][col] > distances[row][col] + 1) {
								distances[row - 1][col] = distances[row][col] + 1;
								push(unvisited, distances[row - 1][col], nodeRef - mazeSize);
						}
				}
				// down
				if (row != mazeSize - 1 && graph_has_edge(g, nodeRef, nodeRef + mazeSize) == 0) {
						if (distances[row + 1][col] > distances[row][col] + 1) {
								distances[row + 1][col] = distances[row][col] + 1;
								push(unvisited, distances[row + 1][col], nodeRef + mazeSize);
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
		heap_t *path;
		push(path, currDistance, nodeRef);
		while (currDistance > 0) {
			row = getRowFromInt(nodeRef);
			col = getColFromInt(nodeRef);

			// left
			if (col != 0 && graph_has_edge(g, nodeRef, nodeRef - 1) == 0) {
					if (d[row][col - 1] < d[row][col]) {
							push(path, d[row][col - 1], nodeRef - 1);
							currDistance = d[row][col - 1];
							nodeRef = nodeRef - 1;
							continue;
					}
			}

			// right
			if (col != mazeSize - 1 && graph_has_edge(g, nodeRef, nodeRef + 1) == 0) {
					if (d[row][col + 1] < d[row][col]) {
							push(path, d[row][col + 1], nodeRef + 1);
							currDistance = d[row][col + 1];
							nodeRef = nodeRef + 1;
							continue;
					}
			}
			// up
			if (row != 0 && graph_has_edge(g, nodeRef, nodeRef - mazeSize) == 0) {
				if (d[row - 1][col] < d[row - 1][col]) {
						push(path, d[row - 1][col], nodeRef - mazeSize);
						currDistance = d[row - 1][col];
						nodeRef = nodeRef - mazeSize;
						continue;
				}
			}
			// down
			if (row != mazeSize - 1 && graph_has_edge(g, nodeRef, nodeRef + mazeSize) == 0) {
				if (d[row + 1][col] < d[row][col]) {
						push(path, d[row + 1][col], nodeRef + mazeSize);
						currDistance = d[row + 1][col];
						nodeRef = nodeRef + mazeSize;
						continue;
				}
			}
		}

		currentPath = path;
}

void assignPath(int finish) {
		int currNode = getIntFromCoordinates(currRow, currCol);
		findShortestPath(maze, currNode, finish);
		getShortestPath(distances, finish);
}

// Follow current path, returns direction of next node
int followPath() {
		if (isEmpty(currentPath)) {
				return -1;
		}

		int nextNode = pop(currentPath);
		int currNode = getIntFromCoordinates(currRow, currCol);

		if (nextNode == currNode - mazeSize) {
				return UP;
		} else if (nextNode == currNode + mazeSize) {
				return DOWN;
		} else if (nextNode == currNode + 1) {
				return RIGHT;
		} else if (nextNode == currNode - 1) {
				return LEFT;
		}
}

// Find the maze, solve it for shortest path, and traverse this path.
int main(int argc, char *argv[]) {
}
