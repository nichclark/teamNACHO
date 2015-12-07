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
#include "display.h"
#include "userinput.h"
#include "PIDcontrol.h"
#include "feedback.h"

#include "Wire.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

// These variables correspond to pins on the Arduino
#define PWMOutput   6
#define pot        A0
#define feedback   A1
// These global variables are initialized and set to zero
long LastTime = 0;

//The follow varables are what get used in the PID loop
#define kP                0.2 //proportional constant
#define kI                  1 //integral constant
#define kD                  0 //derivative constant
#define IntegralRange     500 //sets when the intergral term start taking effect
#define SampleTime         50 //how often PID loop runs in miliseconds

void setup() {
  initializations();
}
  
void loop() {
  int DutyCycle;
  //Since clock was changed to 62.5kHz, had to divide clock by 64(0x04) to
  //be able to look at the clock in milliseconds
  long CurrentTime = millis()/64;
  //Reading the user input
  int Desired = userInput();
  //Reading the output voltage 
  int Actual = calcFeedback(feedback); 

  //How oftem the PID loop runs is set by the SampleTime Variable.
  if(CurrentTime - LastTime > SampleTime){
    //Run the PID loop with the defined constants from above
    DutyCycle = PIDcontrol(kP,kI,kD,IntegralRange);
    //Save the current time to make sure the PID loop runs at the correct
    //sample time
    LastTime = CurrentTime;
  }
  //The duty cycle controls the output of this pin. Since this command is
  //outside of the PID loop, the pin with have a constant duty cycle for 
  //every clock cycle until the PID loop is run. When the PID loop is run,
  //the output duty cycle is changed and will now be constant till the next time.
  analogWrite(PWMOutput, DutyCycle);
}

