# Macros
CC = gcc
IDIR = ./inc
CFLAGS = -I$(IDIR) 

# Pattern rule
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Dependency rules for non-file targets
all: robot
clean:
	rm -f *.o

# Dependency rules for file targets
robot: maze.o graph.o motors.o
	$(CC) -o $@ $^ $(CFLAGS)
motors.o: motors.h
sensors.o: sensors.h
graph.o: graph.h