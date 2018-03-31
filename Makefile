# Macros
CC = gcc
IDIR = ./inc
CFLAGS = -I$(IDIR) -Wall -pthread -lpigpiod_if2 -lrt -lm 

# Pattern rule
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Dependency rules for non-file targets
all: robot
clean:
	rm -f *.o

# Dependency rules for file targets
robot: maze.o graph.o motors.o kalman.o constants.h
	$(CC) -o $@ $^ $(CFLAGS)

test_sensors: test_sensors.o sensors.o adafruit_distance.o constants.h
	$(CC) -o $@ $^ $(CFLAGS)

test_motors: test_motors.o constants.h
	$(CC) -o $@ $^ $(CFLAGS)

test_distance: test_distance.o adafruit_distance.o constants.h
	$(CC) -o $@ $^ $(CFLAGS)

test_leds: test_leds.o leds.o
	$(CC) -o $@ $^ $(CFLAGS)

test_buttons: test_buttons.o buttons.o
	$(CC) -o $@ $^ $(CFLAGS)

#motors.o: motors.h
#sensors.o: sensors.h
#graph.o: graph.h
