

Graph discoverMaze();

int dfs(int row, int col, int leftWall, int rightWall, int upWall, int downWall);

int getIntFromCoordinates(int row, int col);

int getRowFromInt(int nodeRef);
int getColFromInt(int nodeRef);

heap_t findShortestPath(Graph g, int start, int finish);

void traverseShortestPath(heap_t h);

void returnToStart(Graph g, int currNode);

void solveMaze(Graph g);

int main(int argc, char *argv[]);
