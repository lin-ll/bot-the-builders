/*----------------------------------------------------------------------------*/
/* maze.h 																      */
/* Goes through the maze logic -- Use Dijkstra's algorithm					  */
/* Contains the main method that will be used for the competition			  */
/*----------------------------------------------------------------------------*/
#ifndef MAZE_INCLUDED
#define MAZE_INCLUDED

Graph initializeMaze();

// if robot crashes, reset maze solving algorithm
void maze_reset();

int dfs(int row, int col, int leftWall, int rightWall, int upWall, int downWall);

int getIntFromCoordinates(int row, int col);

int getRowFromInt(int nodeRef);
int getColFromInt(int nodeRef);

int **findShortestPath(Graph g, int start, int finish);
heap_t *getShortestPath(int **d, int finish);

// assigns current path
void assignPath(int goalSpace);

// follows current path
int followPath();

// returns true if maze is fully explored
int isExplored();

#endif
