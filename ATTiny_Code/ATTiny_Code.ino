/*
 * The goal of the ATTiny is to read in the really fast motor encoder data and then give it to the pi over i2c.
 * 
 * The loop constantly checks the 8 inputs(one from each motor).
 * When one of the input changes, it updates the appropriate counter (one for each motor).
 * When the the i2c read request is sent by the pi, the ATTiny returns the values of the 4 motors, and clears them.
 * 
 * Run on 8Mz Oscillator
 * 
 * ATtiny Pin 1:  5V
 * ATtiny Pin 2:  D0  -- motor_pins[0]  -- Motor 1 In-phase  
 * ATtiny Pin 3:  D1  -- motor_pins[1]  -- Motor 1 Quadrature 
 * ATtiny Pin 4:  RESET  
 * ATtiny Pin 5:  D2  -- motor_pins[2]  -- Motor 2 In-phase 
 * ATtiny Pin 6:  D3  -- motor_pins[3]  -- Motor 2 Quadrature
 * ATtiny Pin 7:  D4  -- i2c SDA Pin
 * ATtiny Pin 8:  D5  -- motor_pins[4]  -- Motor 3 In-phase 
 * ATtiny Pin 9:  D6  -- i2c SCL Pin
 * ATtiny Pin 10: D7  -- motor_pins[5]  -- Motor 3 Quadrature
 * ATtiny Pin 11: D8  -- motor_pins[6]  -- Motor 4 In-phase
 * ATtiny Pin 12: D9  -- motor_pins[7]  -- Motor 4 Quadrature
 * ATtiny Pin 13: D10  -- Debug   
 * ATtiny Pin 14: GND    
 * 
 * Ref Pinouts on https://camo.githubusercontent.com/d46f3f004aaf977040d933ae5eaf25d22d33eac1/687474703a2f2f6472617a7a792e636f6d2f652f696d672f50696e6f7574543834612e6a7067
 * or http://www.akafugu.jp/images/microcontroller-reference-sheet.png
 */
 
#define I2C_SLAVE_ADDRESS 0x4 // the 7-bit address

#include "TinyWireS.h" 

//Declare variables
int i = 0, j = 0;
long counter[4];
byte state[4];
byte state_prev[4];
byte counter_bytes[16];
boolean A,B;

//Declare QEM Array
int QEM[4][4] = {{0,-1,0,1},{1,0,-1,0},{0,1,0,-1},{-1,0,1,0}};

//Declare pins
int motor_pins[] = {0,1,2,3,5,7,8,9};   //InPhase pins are at even indexes, and Quadrature pins at odd inputs
int pi_i2c_SCL = 6;
int pi_i2c_SDA = 4;
int debug = 10;

void setup()
{
  //-----------------------
  // SETUP FOR THE MOTORS
  //-----------------------
  for(i = 0; i<8; i++)
  {
    pinMode(motor_pins[i], INPUT);    
  }

  //**** Check for interrupt values for ATTiny
  /*attachInterrupt(digitalPinToInterrupt(motor_pins[0]), Motor0_InPhase_Changed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[1]), Motor0_Quad_Changed,    CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[2]), Motor1_InPhase_Changed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[3]), Motor1_Quad_Changed,    CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[4]), Motor2_InPhase_Changed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[5]), Motor2_Quad_Changed,    CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[6]), Motor3_InPhase_Changed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[7]), Motor3_Quad_Changed,    CHANGE);
  */

  //-----------------------
  // SETUP FOR THE i2c
  //-----------------------
  pinMode(debug, OUTPUT); // Debug PIN is HIGH when sending data
  digitalWrite(debug, LOW); 

  TinyWireS.begin(I2C_SLAVE_ADDRESS);
  TinyWireS.onRequest(requestEvent);
}

//Get state of a motor given the 2 encoder inputs
byte getState(boolean In_Phase, boolean Quadrature) 
{
  if((In_Phase)&&(Quadrature))  return 0;
  if((In_Phase)&&(!Quadrature)) return 1;
  if((!In_Phase)&&(!Quadrature)) return 2;
  if((!In_Phase)&&(Quadrature)) return 3;   
}

//ISR for all the Motors call the generic ISR passing the motor number as a parameter
void Motor0_InPhase_Changed() {InputChanged(0);}
void Motor0_Quad_Changed()    {InputChanged(0);}
void Motor1_InPhase_Changed() {InputChanged(1);}
void Motor1_Quad_Changed()    {InputChanged(1);}
void Motor2_InPhase_Changed() {InputChanged(2);}
void Motor2_Quad_Changed()    {InputChanged(2);}
void Motor3_InPhase_Changed() {InputChanged(3);}
void Motor3_Quad_Changed()    {InputChanged(3);}

//Generic ISR for all Motors
void InputChanged(byte motor)
{
  A = digitalRead(motor_pins[2*motor]);       //Read the In_Phase value from the even numbered pin
  B = digitalRead(motor_pins[2*motor + 1]);   //Read the Quadrature value from the odd pin

  state[motor] = getState(A,B);                       //Get the current state
  counter[motor] = counter[motor] + QEM[state[motor]][state_prev[motor]];   //Increment the count accordingly
  state_prev[motor] = state[motor];                   //Set the store the current state value to the prev. state
 
}

//ISR for i2c request from the PI
void requestEvent()
{  
    digitalWrite(debug, HIGH); //Set the DEBUG Pin HIGH for Sending Data

    //Split each counter variable into bytes
    for(j=0;j<4; j++)   //Iterates through motors
    {
      for(i=0;i<4;i++)  //Iterates through bytes of each counter
      {
        counter_bytes[i + (j*4)] = counter[j] >> (i*8);
      }
      counter[j] = 0;   //Clears the corresponding counter.
    }

    for(i=0;i<16;i++)
    {
      TinyWireS.send(counter_bytes[i]);   //Send all 16 bytes
    }
    digitalWrite(debug, LOW);            //Set the DEBUG Pin back to LOW
}

void loop() 
{
  while (true) {
      
  }
}



