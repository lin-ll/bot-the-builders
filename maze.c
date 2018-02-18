/*----------------------------------------------------------------------------*/
/* Maze solving and traversal. Calls on general.c methods for sensor reading  */
/* and motor control.                                                         */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <graph.h>
#include <controls.h>

typedef struct Stack_t{
	int *values;
	int length;
	int length_allocated;
} Stack;

// typedef struct{
// 		Graph maze;
// 		int **traversed; // 0 for false, 1 for true
// } mazeSolver;

static int mazeSize = 16;

// Visualize the maze and parse it into a graph. Descritize each unit of maze
// into a vertex, and connect them with edges of unit length. Uses DFS.
Graph discoverMaze() {
		Graph maze = graph_create(mazeSize * mazeSize);
		int traversed[mazeSize][mazeSize];
		for (int i = 0; i < mazeSize; i++) {
				for (int j = 0; j < mazeSize; j++) {
						if (traversed[i][j] == 0) {
								dfs(maze, row, col);
						}
				}
		}
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
		}
		// right
		if (col != mazeSize - 1 && isWall(row, col, row, col + 1) == 0) {
				graph_add_edge(maze, nodeRef, nodeRef + 1);
		}
		// up
		if (row != 0 && isWall(row, col, row - 1, col) == 0) {
				graph_add_edge(maze, nodeRef, nodeRef - mazeSize);
		}
		// down
		if (row != mazeSize - 1 && isWall(row, col, row + 1, col) == 0) {
				graph_add_edge(maze, nodeRef, nodeRef + mazeSize);
		}

		dfs(maze, row, col + 1);
		dfs(maze, row + 1, col);
		dfs(maze, row, col - 1);
		dfs(maze, row - 1, col);
}

// converts row and column to label of node in graph
int getIntFromCoordinates(int row, int col) {
		return row * 16 + col;
}

// Find the shortest path given the graph representaion of the maze. Use
// Dijkstra's algorithm.
Stack findShortestPath(Graph g) {

}

// Traverse the shortest path.
void traverseShortestPath(Stack s) {

}

// Find the maze, solve it for shortest path, and traverse this path.
int main(int argc, char *argv[]) {

}
