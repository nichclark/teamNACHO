#include "feedback.h"
#include <arduino.h>
#include <avr/io.h>


float calcFeedback(int pin){

  long lowTime; long highTime; long period;
  float dutyPercent; float value;

  lowTime = pulseIn(pin, LOW, 10000);
  highTime = pulseIn(pin, HIGH, 10000);
  period = highTime + lowTime;
  dutyPercent = (float) highTime / period;
  if(lowTime == 0) value = 0;
  else value = 1023*dutyPercent;
/* 
  Serial.print("Low = ");Serial.print(lowTime);Serial.print(", ");
  Serial.print("High = ");Serial.print(highTime);Serial.print(", ");
  Serial.print("Period = ");Serial.print(period);Serial.print(", ");
  Serial.print("% = ");Serial.print(dutyPercent);Serial.print(", ");
  Serial.print("value = ");Serial.print(value);Serial.print(", ");
*/
  return value;
}