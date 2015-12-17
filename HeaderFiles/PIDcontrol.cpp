/*
TITLE("TeamNACHO USB Power Supply");
SUBTITLE("PID Control Fxn");
BY("Adam S. Hastings && Nicholas P. Clark");
DATE("17 December 2015");

This code should be IMPLEMENTED if the output duty cycle is needed
to be controlled by a PID Loop. The fxn is passed the Desired output
voltage along with the Actual voltage from the feedback loop. The 
constants for the PID loop are also passed. 
*/

#include "PIDcontrol.h"
#include <arduino.h>
#include <avr/io.h>

float I = 0;
float PIDcontrol(float Desired, float Actual, float kP, float kI, float kD){
  //Set what the max output can be, helps protect circuitry
  float maxOut = 800;
  float minOut = 0;
  
  //Determine what the Error is
  float lastError;
  float Error = Desired - Actual;

  float P = (Error * kP);
  //Sum up the error with the last I term
  I = I + (Error * kI);
  //However if I term gets too big, reset I term
  if(I > maxOut) I = 0;
  if(I < minOut) I = 0;
  //Determine difference between last and current error
  float D = (Error - lastError) * kD;
  //Sum up PID terms
  float Out = P + I + D;
  //Limit the output to protect circuitry
  if(Out > maxOut) Out = maxOut;
  if(Out < minOut) Out = minOut;
  //Convert units 
  float Duty = map(Out, 0, 1023, 0, 255);
  
  lastError = Error;
  //Serial print used for debug
/*
  Serial.print("Des ");Serial.print(Desired);Serial.print(", ");
  Serial.print("Act ");Serial.print(Actual);Serial.print(", ");
  Serial.print("Error ");Serial.print(Error);Serial.print(", ");
  Serial.print("P ");Serial.print(P);Serial.print(", ");
  Serial.print("I ");Serial.print(I);Serial.print(", ");
  Serial.print("D ");Serial.print(D);Serial.print(", ");
  Serial.print("Out ");Serial.print(Out);Serial.print(", ");
  Serial.print("Duty ");Serial.println(Duty);
*/

  return Duty;
}