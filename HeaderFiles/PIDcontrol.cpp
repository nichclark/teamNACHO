#include "PIDcontrol.h"
#include <arduino.h>
#include <avr/io.h>

float I = 0;
float PIDcontrol(float Desired, float Actual, float kP, float kI, float kD){
  float maxOut = 800;
  float minOut = 0;
  
  
  float lastError;
  float Error = Desired - Actual;

  float P = (Error * kP);
  
  I = I + (Error * kI);
  if(I > maxOut) I = maxOut;
  if(I < minOut) I = minOut;
  
  float D = (Error - lastError) * kD;
  
  float Out = P + I + D;
  if(Out > maxOut) Out = maxOut;
  if(Out < minOut) Out = minOut;
  
  float Duty = map(Out, 0, 1023, 0, 255);
  
  lastError = Error;

  Serial.print("Des ");Serial.print(Desired);Serial.print(", ");
  Serial.print("Act ");Serial.print(Actual);Serial.print(", ");
  Serial.print("Error ");Serial.print(Error);Serial.print(", ");
  Serial.print("P ");Serial.print(P);Serial.print(", ");
  Serial.print("I ");Serial.print(I);Serial.print(", ");
  Serial.print("D ");Serial.print(D);Serial.print(", ");
  Serial.print("Out ");Serial.print(Out);Serial.print(", ");
  Serial.print("Duty ");Serial.println(Duty);


  return Duty;
}