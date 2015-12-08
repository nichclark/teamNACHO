/* TeamNACHO's "NACHO Power" USB Power Supply- Above and Beyond
 *  By: Nicholas Clark & Adam Hastings
 *  Date Complete: 07 December 2015

 *  This program will be used for above&beyond perf board circuit. 
 *  This code will include the implementation of the sevenseg display 
 *  and PBs as the user interface. It will also be reading a digital 
 *  pin as the feedback instead of an analog. This is done as the 
 *  feedback is being transmitted through an opto coupler so the circuit 
 *  will have isolation.
 *  
 */
//Include the following header files
#include "inits.h"
/*#include "display.h"*/
#include "userinput.h"
#include "PIDcontrol.h"
#include "feedback.h"

#include "Wire.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
Adafruit_7segment matrix = Adafruit_7segment();

//These variables correspond to pins on the Arduino
#define Vopto 4
#define PWMOutput OCR0A //pin D6
#define ConstOutput OCR0B //pin D5
//pin D7 = PB1
//pin D8 = PB2
//pin A4 = DAT for 7segDisplay
//pin A5 = CLK for 7segDisplay

//Define variables that set the max range of the power supply (x100)
#define maxValue 1250
#define minValue 50
#define SetDuty 160
// These global variables are initialized and set to zero
int DutyCycle = 0;
long LastTime = 0;
int Desired = 0;
int Actual = 0;

//The follow varables are what get used in the PID loop
#define kP 0.1 //proportional constant
#define kI 0 //integral constant
#define kD 0 //derivative constant
#define IntegralRange 500 //sets when the intergral term start taking effect
#define SampleTime 500 //how often PID loop runs in miliseconds

//Adafruit_7segment matrix = Adafruit_7segment();
void setup() {
  initializations(SampleTime);
  matrix.begin(0x70);
}
void loop() {
  //Desired = userInput(minValue,maxValue);
  Desired = map(5,0,1250,0,1024);
  Actual = calcFeedback(Vopto); 
  displayValue(Desired);
  PWMOutput = DutyCycle;
  ConstOutput = SetDuty;
}
void displayValue (int value){
  int A;int B;int C;int D;
  //value will have a max value of 1250
  value = map(value,0,1024,minValue,maxValue);
  A = value/1000;
  B = (value/100) - (A*10);
  C = (value/10) - (A*100) - (B*10);
  D = (value/1) - (A*1000) - (B*100) - (C*10);
  
  matrix.writeDigitNum(0,A);
  matrix.writeDigitNum(1,B,true);
  matrix.writeDigitNum(3,C);
  matrix.writeDigitNum(4,D);
  matrix.writeDisplay();
}
ISR(TIMER1_COMPA_vect){
  DutyCycle = PIDcontrol(kP,kI,kD,IntegralRange,Desired,Actual);
}

