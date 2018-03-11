// The central file for controlling the robot

#include "inc/maze.h"
#include "inc/controls.h"

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

int followPath() {

}

void main() {
    explore();
}
