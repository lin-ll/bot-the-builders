// constants
#ifndef CONSTANT_H
#define CONSTANT_H

//-------------------

#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

#define NORTH 0
#define SOUTH 1
#define WEST 2
#define EAST 3

#define TRUE 1
#define FALSE 0

#define MAZE_SIZE 16
#define SQUARE_SIZE 180 // 168 + 12
#define HALF_CHASSIS_SIZE 40 // mm from robot center to distance sensor

// multiply by these to convert motor setting (0-255) into meters/radians per second
#define MOTOR_TO_MPS (180.0/127.0)
#define MOTOR_TO_RPS (6.28/(3.0*127.0))

#define PI 3.1415926536
#define TWO_PI (2*3.1415926536)
#define DISTANCE_OFF 0
#define DISTANCE_ON 1

#define START_SPACE 0
#define GOAL_SPACE 0

#define MAX_COLOR 255

#define BUS 1
#define ORIG_ADDR 0x29
#define GYRO_ADDR 0x6B
#define COMPASS_ADDR 0x1E

//-------------------

#define BUTTON_RED 16
#define BUTTON_BLUE 20
#define BUTTON_GREEN 19
#define BUTTON_YELLOW 26
#define BUTTON_GROUND 24

//-------------------

#define UPPER_LEFT_FORWARD 23
#define UPPER_LEFT_BACKWARD 22
#define UPPER_RIGHT_FORWARD 25
#define UPPER_RIGHT_BACKWARD 9
#define LOWER_LEFT_FORWARD 12
#define LOWER_LEFT_BACKWARD 6
#define LOWER_RIGHT_FORWARD 11
#define LOWER_RIGHT_BACKWARD 8
#define MOTOR_RANGE 255

//-------------------

#define UPPER_LEFT 0
#define UPPER_RIGHT 1
#define LOWER_LEFT 2
#define LOWER_RIGHT 3

//-------------------

#define ACC_CTRL1 0x10
#define GYRO_CTRL2 0x11
#define COMMON_CTRL3 0x12
#define COMMON_CTRL4 0x13
#define COMMON_CTRL5 0x14
#define COMMON_CTRL6 0x15
#define GYRO_CTRL7 0x16
#define ACC_CTRL8 0x17
#define ACC_CTRL9 0x18
#define COMMON_CTRL10 0x19

#define GYRO_OUT_X_L 0x22
#define GYRO_OUT_X_H 0x23
#define GYRO_OUT_Y_L 0x24
#define GYRO_OUT_Y_H 0x25
#define GYRO_OUT_Z_L 0x26
#define GYRO_OUT_Z_H 0x27

#define ACC_OUT_X_L 0x28
#define ACC_OUT_X_H 0x29
#define ACC_OUT_Y_L 0x2A
#define ACC_OUT_Y_H 0x2B
#define ACC_OUT_Z_L 0x2C
#define ACC_OUT_Z_H 0x2D 

//-------------------

#define COMPASS_CTRL1 20
#define COMPASS_CTRL2 21
#define COMPASS_CTRL3 22
#define COMPASS_CTRL4 23
#define COMPASS_CTRL5 24

#define COMPASS_OUT_X_L 0x28
#define COMPASS_OUT_X_H 0x29
#define COMPASS_OUT_Y_L 0x2A
#define COMPASS_OUT_Y_H 0x2B
#define COMPASS_OUT_Z_L 0x2C
#define COMPASS_OUT_Z_H 0x2D

//-------------------

#define SHORT_PIN_FRONT 5
#define SHORT_PIN_BACK 27
#define SHORT_PIN_LEFT 17
#define SHORT_PIN_RIGHT 21

#define LONG_PIN_FRONT 7
#define LONG_PIN_BACK 10
#define LONG_PIN_LEFT 4
#define LONG_PIN_RIGHT 13

//-------------------

#define LED_BLUE 14
#define LED_RED 15
#define LED_GREEN 18

//-------------------

#endif


