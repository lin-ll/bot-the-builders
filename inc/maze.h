/*----------------------------------------------------------------------------*/
/* maze.h 																      */
/* Goes through the maze logic -- Use Dijkstra's algorithm					  */
/* Contains the main method that will be used for the competition			  */
/*----------------------------------------------------------------------------*/
#ifndef MAZE_INCLUDED
#define MAZE_INCLUDED

void Maze_init();

// if robot crashes, reset maze solving algorithm
void Maze_reset();

// if robot is at START_SPACE
int Maze_isAtStart();

// if robot has fully explored the maze
int Maze_isExplored();

// returns direction to search maze
int Maze_dfs(int row, int col, int leftWall, int rightWall, int upWall, int downWall);

int Maze_getIntFromCoordinates(int row, int col);

heap_t *getShortestPath(int **d, int finish);

// assigns current path
void Maze_assignPath(int goalSpace);

// follows current path
int Maze_followPath();

// returns true if maze is fully explored
int Maze_isExplored();

#endif
