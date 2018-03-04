/*----------------------------------------------------------------------------*/
/* Maze solving and traversal. Calls on general.c methods for sensor reading  */
/* and motor control.                                                         */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <graph.h>
#include <controls.h>
#include <heap.h>

// typedef struct{
// 		Graph maze;
// 		int **traversed; // 0 for false, 1 for true
// } mazeSolver;

static int mazeSize = 16;

// Visualize the maze and parse it into a graph. Descritize each unit of maze
// into a vertex, and connect them with edges of unit length. Uses DFS.
Graph discoverMaze() {
		Graph maze = graph_create(mazeSize * mazeSize);
		// connect center squares
		graph_add_edge(maze, 119, 120);
		graph_add_edge(maze, 119, 135);
		graph_add_edge(maze, 120, 136);
		graph_add_edge(maze, 135, 136);
		int traversed[mazeSize][mazeSize];
		dfs(maze, 0, 0);
}

// implements depth first search
void dfs(Graph maze, int row, int col) {
		if (row < 0 || row >= mazeSize) {
				return; // invalid row
		}
		if (col < 0 || col >= mazeSize) {
				return; // invalid col
		}

		// if square has already been travelled to
		if (traversed[row][col] == 1) {
				return;
		}

		traversed[row][col] = 1;
		int nodeRef = getIntFromCoordinates(row, col);

		/* assumes there is an isWall function which senses if a wall is present
		*  between each adjacent node and current node */
		// left
		if (col != 0 && isWall(row, col, row, col - 1) == 0) {
				graph_add_edge(maze, nodeRef, nodeRef - 1);
				moveTo(nodeRef, nodeRef - 1);
				dfs(maze, row, col - 1);
		}
		// right
		if (col != mazeSize - 1 && isWall(row, col, row, col + 1) == 0) {
				graph_add_edge(maze, nodeRef, nodeRef + 1);
				moveTo(nodeRef, nodeRef + 1);
				dfs(maze, row, col + 1);
		}
		// up
		if (row != 0 && isWall(row, col, row - 1, col) == 0) {
				graph_add_edge(maze, nodeRef, nodeRef - mazeSize);
				moveTo(nodeRef, nodeRef + mazeSize);
				dfs(maze, row - 1, col);
		}
		// down
		if (row != mazeSize - 1 && isWall(row, col, row + 1, col) == 0) {
				graph_add_edge(maze, nodeRef, nodeRef + mazeSize);
				moveTo(nodeRef, nodeRef - mazeSize);
				dfs(maze, row + 1, col);
		}
}

// converts row and column to label of node in graph
int getIntFromCoordinates(int row, int col) {
		return row * mazeSize + col;
}

// converts node to row and column
int *getCoordFromInt(int nodeRef) {
		int col = nodeRef % mazeSize;
		int row = nodeRef / mazeSize;
		int *coords = {row, col};
		return coords;
}

// Find the shortest path given the graph representaion of the maze. Use
// Dijkstra's algorithm.
heap_t findShortestPath(Graph g) {
		int distances[mazeSize][mazeSize];
		for (int i = 0; i < mazeSize; i++) {
				for (int j = 0; j < mazeSize; j++) {
						distances[i][j] = INT_MAX;
				}
		}
		distances[0][0] = 0;

		heap_t visited;
		heap_t unvisited;

		for (int i = 0; i < mazeSize; i++) {
				for (int j = 0; j < mazeSize; j++) {
						int nodeRef = getIntFromCoordinates(i,j);
						char *s = nodeRef;
						push(unvisited, distances[i][j], s);
				}
		}

		while (true) {
				char *insert = pop(unvisited);
				int nodeRef = (int) (insert);
				int *coords = getCoordFromInt(nodeRef);
				int row = coords[0];
				int col = coords[1];
				push(visited, distances[row][col], insert);

				// left
				if (col != 0 && graph_has_edge(g, nodeRef, nodeRef - 1) == 0) {
						if (distances[row][col - 1] > distances[row][col] + 1) {
								distances[row][col - 1] = distances[row][col] + 1;
						}
				}
				// right
				if (col != mazeSize - 1 && graph_has_edge(g, nodeRef, nodeRef - 1) == 0) {
						if (distances[row][col + 1] > distances[row][col] + 1) {
								distances[row][col + 1] = distances[row][col] + 1;
						}
				}
				// up
				if (row != 0 && graph_has_edge(g, nodeRef, nodeRef - mazeSize) == 0) {
						if (distances[row - 1][col] > distances[row][col] + 1) {
								distances[row - 1][col] = distances[row][col] + 1;
						}
				}
				// down
				if (row != mazeSize - 1 && graph_has_edge(g, nodeRef, nodeRef + mazeSize) == 0) {
						if (distances[row + 1][col] > distances[row][col] + 1) {
								distances[row + 1][col] = distances[row][col] + 1;
						}
				}
		}

		return visited;
}

// Traverse the shortest path.
void traverseShortestPath(heap_t h) {
		char* currNode = pop(h);
		int curr = (int) (currNode);
		while (!isEmpty(h)) {
				char *nextNode = pop(h);
				int next = (int) (nextNode);
				moveTo(curr, next);
				curr = next;
		}
}

// Find the maze, solve it for shortest path, and traverse this path.
int main(int argc, char *argv[]) {
}
