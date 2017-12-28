/*----------------------------------------------------------------------------*/
/* Maze solving and traversal. Calls on general.c methods for sensor reading  */
/* and motor control.                                                         */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <graph.h>

typedef struct Stack_t{
	int *values;
	int length;
	int length_allocated;
} Stack;

// Visualize the maze and parse it into a graph. Descritize each unit of maze
// into a vertex, and connect them with edges of unit length. Uses DFS.
Graph discoverMaze() {

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
