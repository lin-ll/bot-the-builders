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
#define EAST 2
#define WEST 3

#define TRUE 1
#define FALSE 0

#define MAZE_SIZE 16
#define SQUARE_SIZE 180 // 168 + 12

#define TWO_PI (2*3.1415926536)
#define DISTANCE_OFF 0
#define DISTANCE_ON 1

#define START_SPACE 0
#define GOAL_SPACE 0

#define MAX_COLOR 255

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

#define BUS 1
#define GYRO_ADDR 0x6B // 7 bit 1101011;
#define COMPASS_ADDR 0x1E // if it's wrong, try 1F
#define ORIG_ADDR 0x29 // the address the i2c listens to

#define GYRO_REGISTER_OUT_X_L 0x28
#define GYRO_REGISTER_OUT_X_H 0x29
#define GYRO_REGISTER_OUT_Y_L 0x2A
#define GYRO_REGISTER_OUT_Y_H 0x2B
#define GYRO_REGISTER_OUT_Z_L 0x2C
#define GYRO_REGISTER_OUT_Z_H 0x2D 
#define ACC_REGISTER_OUT_X_L_A 0x28
#define ACC_REGISTER_OUT_X_H_A 0x29
#define ACC_REGISTER_OUT_Y_L_A 0x2A
#define ACC_REGISTER_OUT_Y_H_A 0x2B
#define ACC_REGISTER_OUT_Z_L_A 0x2C
#define ACC_REGISTER_OUT_Z_H_A 0x2D 
#define COMPASS_REGISTER_OUT_X_H_M 0x08 // HEY! if these values are wrong, try adding 5 to each
#define COMPASS_REGISTER_OUT_X_L_M 0x09 // There's another version of this chip with those addresses
#define COMPASS_REGISTER_OUT_Y_H_M 0x0A
#define COMPASS_REGISTER_OUT_Y_L_M 0x0B
#define COMPASS_REGISTER_OUT_Z_H_M 0x0C
#define COMPASS_REGISTER_OUT_Z_L_M 0x0D

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


