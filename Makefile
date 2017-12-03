# Macros
CC = gcc
CFLAGS = 

# Pattern rule
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Dependency rules for non-file targets
all: robot
clean:
	rm -f *.o

# Dependency rules for file targets
robot: maze.o graph.o motors.o
	$(CC) $(CFLAGS) -o $@
motors.o: motors.h
graph.o: graph.h