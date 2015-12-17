/*
TITLE("TeamNACHO USB Power Supply");
SUBTITLE("Decoding Feedback Fxn");
BY("Adam S. Hastings && Nicholas P. Clark");
DATE("17 December 2015");

This code should be IMPLEMENTED to decode the PWM signal from
the ATtiny. To do this, the time the signal is low and high is
determined in microseconds. The duty cycle is then calculated
from these values. What voltage the ATtiny read is determined
by multiplying the duty cycle by 1023. This gives an "Actual"
voltage that can be IMPLEMENTED in TeamNACHO's PID fxn.
*/

#include "feedback.h"
#include <arduino.h>
#include <avr/io.h>

//The fxn needs to know what pin to be reading
float calcFeedback(int pin){

  long lowTime; long highTime; long period;
  float dutyPercent; float value;

  lowTime = pulseIn(pin, LOW, 10000);
  highTime = pulseIn(pin, HIGH, 10000);
  period = highTime + lowTime;
  dutyPercent = (float) highTime / period;
  if(lowTime == 0) value = 0;
  else value = 1023*dutyPercent;
  //Serial print used for debug
/* 
  Serial.print("Low = ");Serial.print(lowTime);Serial.print(", ");
  Serial.print("High = ");Serial.print(highTime);Serial.print(", ");
  Serial.print("Period = ");Serial.print(period);Serial.print(", ");
  Serial.print("% = ");Serial.print(dutyPercent);Serial.print(", ");
  Serial.print("value = ");Serial.print(value);Serial.print(", ");
*/
  return value;
}