// controls.h

int mazeSize;

int findDirection(int oldNode, int newNode);

// assuming forward orientation, positive translation defined to the right,
// and positive rotations defined in clockwise direction
void adjust(int direction, double mainVelocity, double translate, double rotate);

void moveTo(int oldNode, int newNode);
