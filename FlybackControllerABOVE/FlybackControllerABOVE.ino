/*
TITLE("TeamNACHO USB Power Supply");
SUBTITLE("Design Review Main Code");
BY("Adam S. Hastings && Nicholas P. Clark");
DATE("17 December 2015");

This code is to be used with design review circuit. This circuit
takes 5VDC input and the output ranges between 1VDC and 12VDC.
For the design review circuit, the output ground is isolated from
the main supply ground. To do this, a second flyback circuit was
IMPLEMENTED to create a secondary side 5VDC supply, which was
used to power an ATtiny85. This second microcontroller takes the
circuit's output voltage and transmits back to the main controller
by varying the duty cycle of a PWM fuction. 
*/
// Header files created and used by TeamNACHO
#include "inits.h"
#include "PIDcontrol.h"
#include "feedback.h"
#include "userinput.h"
// Header files needed to communicate with Adafruit 7seg Display
#include "Wire.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
Adafruit_7segment matrix = Adafruit_7segment();

//These are the pins being used on the Arduino Uno
#define Switch 4 //pin D5
#define Vfeedback 6 //pin D6
#define PBa 7 //pin D7
#define PBb 8 //pin D8
#define varyOutput  9 //pin D9
#define constOutput  10 //pin D10
//pin A4 = DAT for 7segDisplay
//pin A5 = CLK for 7segDisplay

// The duty cycle that controls the varyOutput (the main flyback)
float DutyCycle;

void setup() {
  //Call TeamNACHO initialization fxn to intialize clock and pins
  initializations();
  //Start communication with 7seg
  matrix.begin(0x70);
}

void loop() {
/// PID Loop related variables
  float kP = 0.08; //proportional constant
  float kI = 0.08; //integral constant
  float kD = 0.00; //derivative constant
  
/// Time related variables
  long CurrentTime = millis();
  long LastTime;
  long SampleTime = 10; //how often PID loop runs in miliseconds
/// PWM related variables
  int SetDuty = 160; //used for constOutput flyback
  //Determine desired output voltage with TeamNACHO fxn
  float UserPick = userInput(PBa,PBb); 

  //Output the two PWM signals to drive flybacks
  analogWrite(varyOutput, DutyCycle);
  analogWrite(constOutput, SetDuty);
  //Display desired output voltage with TeamNACHO fxn
  displayValue(UserPick);
  
  float Desired = map(UserPick,0,1500,0,1023); //unit conversion
  //Decode transmitted PWM signal from ATtiny with TeamNACHO fxn
  float Actual = calcFeedback(Vfeedback);
  //Run PID Loop at a desired sample time
  if(CurrentTime > LastTime + SampleTime){
    //Calculate what duty cycle is for varyOutput with TeamNACHO fxn
    DutyCycle = PIDcontrol(Desired,Actual,kP,kI,kD);
    LastTime = CurrentTime;
  }
}

//TeamNACHO fxn to display desired voltage to 7seg
void displayValue (int value){
  int A;int B;int C;int D;
  //Need to determine what digit to write to each position of display
  A = value/1000;
  B = (value/100) - (A*10);
  C = (value/10) - (A*100) - (B*10);
  D = (value/1) - (A*1000) - (B*100) - (C*10);
  //Following fxns are from Adafruit Backpack library
  matrix.writeDigitNum(0,A);
  matrix.writeDigitNum(1,B,true);
  matrix.writeDigitNum(3,C);
  matrix.writeDigitNum(4,D);
  matrix.writeDisplay();
}


