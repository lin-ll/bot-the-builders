/*----------------------------------------------------------------------------*/
/* Maze solving and traversal. Calls on general.c methods for sensor reading  */
/* and motor control.                                                         */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include "inc/graph.h"
#include "inc/heap.h"
#include "inc/constant.h"

static Graph maze; // graph recording walls and such
static int traversed[16][16] = {0}; // record of traversed squares
static int unexplored = 256; // number of unexplored squares
static int goalSpace = 120; // target space
static int startSpace = 0; // starting space

heap_t *currentPath; // path currently being traversed

// Visualize the maze and parse it into a graph. Descritize each unit of maze
// into a vertex, and connect them with edges of unit length. Uses DFS.
Graph discoverMaze() {
		maze = graph_create(mazeSize * mazeSize);
		// connect center squares
		graph_add_edge(maze, 119, 120);
		graph_add_edge(maze, 119, 135);
		graph_add_edge(maze, 120, 136);
		graph_add_edge(maze, 135, 136);
}

// implements depth first search
int dfs(int row, int col, int leftWall, int rightWall, int upWall, int downWall) {
		if (row < 0 || row >= mazeSize) {
				return -1; // invalid row
		}
		if (col < 0 || col >= mazeSize) {
				return -1; // invalid col
		}

		// if square has already been travelled to
		if (traversed[row][col] == 1) {
				return -1;
		}

		traversed[row][col] = 1;
		unexplored--;
		int nodeRef = getIntFromCoordinates(row, col);

		/* assumes there is an isWall function which senses if a wall is present
		*  between each adjacent node and current node */
		// left
		if (col != 0 && leftWall == 0) {
				graph_add_edge(maze, nodeRef, nodeRef - 1);
				return nodeRef - 1;
		}
		// right
		if (col != mazeSize - 1 && rightWall == 0) {
				graph_add_edge(maze, nodeRef, nodeRef + 1);
				return nodeRef + 1;
		}
		// up
		if (row != 0 && upWall == 0) {
				graph_add_edge(maze, nodeRef, nodeRef - mazeSize);
				return nodeRef - mazeSize;
		}
		// down
		if (row != mazeSize - 1 && downWall == 0) {
				graph_add_edge(maze, nodeRef, nodeRef + mazeSize);
				return nodeRef + mazeSize;
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
int **findShortestPath(Graph g, int start, int finish) {
		int distances[16][16] = {0};
		int i;
		int j;
		for (i = 0; i < mazeSize; i++) {
				for (j = 0; j < mazeSize; j++) {
						distances[i][j] = 10000;
				}
		}

		int startRow = getRowFromInt(startSpace);
		int startCol = getColFromInt(startSpace);
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

		return distances;
}

// Traverse the shortest path.
heap_t *getShortestPath(int **d, int finish) {
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

		return path;
}

// Find the maze, solve it for shortest path, and traverse this path.
int main(int argc, char *argv[]) {
}
