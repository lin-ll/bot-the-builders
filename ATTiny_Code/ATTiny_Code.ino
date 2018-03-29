/*
 * The goal of the ATTiny is to read in the really fast motor encoder data and then give it to the pi over i2c.
 * 
 * The loop constantly checks the 8 inputs(one from each motor).
 * When one of the input changes, it updates the appropriate counter (one for each motor).
 * When the the i2c read request is sent by the pi, the ATTiny returns the values of the 4 motors, and clears them.
 * 
 * ATtiny Pin 1:  5V
 * ATtiny Pin 2:  D0  -- motor_pins[0]  -- Motor 1 In-phase  
 * ATtiny Pin 3:  D1  -- motor_pins[1]  -- Motor 1 Quadrature 
 * ATtiny Pin 4:  RESET  
 * ATtiny Pin 5:  D2  -- motor_pins[2]  -- Motor 2 In-phase 
 * ATtiny Pin 6:  D3  -- motor_pins[3]  -- Motor 2 Quadrature
 * ATtiny Pin 7:  D4  -- motor_pins[4]  -- Motor 3 In-phase 
 * ATtiny Pin 8:  D5  -- pi_i2c     -- PWM Pin
 * ATtiny Pin 9:  D6  -- motor_pins[5]  -- Motor 3 Quadrature
 * ATtiny Pin 10: D7  -- motor_pins[6]  -- Motor 4 In-phase
 * ATtiny Pin 11: D8  -- motor_pins[7]  -- Motor 4 Quadrature
 * ATtiny Pin 12: D9  -- UNUSED    
 * ATtiny Pin 13: D10 -- UNUSED    
 * ATtiny Pin 14: GND 
 * 
 * Ref Pinouts on https://camo.githubusercontent.com/d46f3f004aaf977040d933ae5eaf25d22d33eac1/687474703a2f2f6472617a7a792e636f6d2f652f696d672f50696e6f7574543834612e6a7067
 * or http://www.akafugu.jp/images/microcontroller-reference-sheet.png
 */

//Declare variables
int i = 0;
long counter[] = {0,0,0,0};
byte state[] = {0,0,0,0};
byte state_prev[] = {0,0,0,0};
boolean A,B;

//Declare QEM Array
int QEM[4][4] = {{0,-1,0,1},{1,0,-1,0},{0,1,0,-1},{-1,0,1,0}};

//Declare pins
int motor_pins[] = {0,1,2,3,4,6,7,8};   //InPhase pins are at even indexes, and Quadrature pins at odd inputs
int pi_i2c = 5;

void setup()
{
  Serial.begin(9600);
   
  for(i = 0; i<8; i++)
  {
    pinMode(motor_pins[i], INPUT);    
  }
  
  pinMode(pi_i2c, OUTPUT);

  //**** Check for interrupt values for ATTiny
  attachInterrupt(digitalPinToInterrupt(motor_pins[0]), Motor0_InPhase_Changed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[1]), Motor0_Quad_Changed,    CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[2]), Motor1_InPhase_Changed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[3]), Motor1_Quad_Changed,    CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[4]), Motor2_InPhase_Changed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[5]), Motor2_Quad_Changed,    CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[6]), Motor3_InPhase_Changed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_pins[7]), Motor3_Quad_Changed,    CHANGE);
}

byte getState(boolean In_Phase, boolean Quadrature)
{
  if((In_Phase)&&(Quadrature))  return 0;
  if((In_Phase)&&(!Quadrature)) return 1;
  if((!In_Phase)&&(!Quadrature)) return 2;
  if((!In_Phase)&&(Quadrature)) return 3;   
}

void Motor0_InPhase_Changed() {InputChanged(0);}
void Motor0_Quad_Changed()    {InputChanged(0);}
void Motor1_InPhase_Changed() {InputChanged(1);}
void Motor1_Quad_Changed()    {InputChanged(1);}
void Motor2_InPhase_Changed() {InputChanged(2);}
void Motor2_Quad_Changed()    {InputChanged(2);}
void Motor3_InPhase_Changed() {InputChanged(3);}
void Motor3_Quad_Changed()    {InputChanged(3);}

void InputChanged(byte motor)
{
  A = digitalRead(motor_pins[2*motor]);       //Read the In_Phase value from the even numbered pin
  B = digitalRead(motor_pins[2*motor + 1]);   //Read the Quadrature value from the odd pin

  state[motor] = getState(A,B);                       //Get the current state
  counter[motor] = counter[motor] + QEM[state[motor]][state_prev[motor]];   //Increment the count accordingly
  state_prev[motor] = state[motor];                   //Set the store the current state value to the prev. state
 
}

void loop() 
{
  //**** Temp: communicate with serial monitor instead of the PI
  for(i = 0; i <4; i++)
  {
    //**** Temp: communicate with serial monitor instead of the PI
    Serial.print("Motor ");
    Serial.print(i+1);
    Serial.print(" : "); 
    Serial.println(counter[i]);
  }
  
  delay(1000);
}



