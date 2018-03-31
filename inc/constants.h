// constants
#ifndef CONSTANTS_INC
#define CONSTANTS_INC
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

#define TRUE 1
#define FALSE 0

#define mazeSize 16
#define SQUARE_SIZE 180 // 168 + 12

#define dt 0.1

#define TWO_PI (2*3.1415926536)
#define DISTANCE_OFF 0
#define DISTANCE_ON 1

#define START_SPACE 0
#define GOAL_SPACE 0

//-------------------

#define BUTTON_RED 16
#define BUTTON_BLUE 20
#define BUTTON_GREEN 19
#define BUTTON_YELLOW 26

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
#define COMPASS_ADDR 0x1D // if it's wrong, try 1F

//-------------------

#define ORIG_SHORT_DIST_ADDR 0x29
#define ORIG_LONG_DIST_ADDR 0x2A

#define SHORT_PIN_FRONT -1
#define SHORT_PIN_BACK 4
#define SHORT_PIN_LEFT 17
#define SHORT_PIN_RIGHT 7

#define LONG_PIN_FRONT 10
#define LONG_PIN_BACK 5
#define LONG_PIN_LEFT 13
#define LONG_PIN_RIGHT 21

//-------------------

#define LED_BLUE 14
#define LED_RED 15
#define LED_GREEN 18

#endif


