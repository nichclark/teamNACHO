// PID Loop Program

#include "inits.h"
#include "display.h"

#define constFive 5
//#define PWMOutput 6

//#define feedback 13


int PWMOutput = 6;
int pot = A0;
int feedback = A1;

// A4 = DAT
// A5 = CLC


/*-------------------------------------------*/
int kP = 1.9;
int kD = .95;
int kI = 0.25;
int IntegralRange = 25;
long SampleTime = 25; //miliseconds
/*-------------------------------------------*/





void setup() {
  void inits();
}
  
void loop() {
  double DutyCycle;
  long LastTime;
  int CurrentTime = millis()/64;

  //int num = WhatTheNumber();  //num is in "centi"volts
  //displayValue(num);
  //double Desired = (((num/100)/3)*1023)/5; //Convert num to 1024
  int Desired = analogRead(0);
  double Actual = calcFeedback();
  //Serial.println(feedback);
  Serial.println(analogRead(0));
  
  
  if(CurrentTime >= LastTime + SampleTime){
    DutyCycle = PIDcontroller(Desired,Actual);
    LastTime = CurrentTime;
  }
  analogWrite(PWMOutput, DutyCycle);
  //analogWrite(constFive, (165));


}




int calcFeedback(){

  //Reads a PWM signal's duty cycle and frequency.
  static double duty;
  static double Vfb;
  static long highTime = 0;
  static long lowTime = 0;
  static long tempPulse;
  
  tempPulse = pulseIn(feedback,HIGH);
  if(tempPulse>highTime){
    highTime = tempPulse;
  }
  tempPulse = pulseIn(feedback,LOW);
  if(tempPulse>lowTime){
    lowTime = tempPulse;
  }
  duty = highTime/(double (lowTime+highTime));

  Vfb = 255*duty;
  Vfb = map(Vfb, 0, 255, 0, 1024);
  
  return Vfb;
}

int PIDcontroller(double Desired, double Actual){
  double Last;
  double Integral;
  double Error = Desired - Actual;   //calculate the error
  
  if(abs(Error) < IntegralRange){
    Integral = Integral + Error;
  }
  else{
    Integral = 0;
  }
  
  double P = Error * kP;   //multiply error by proportional term
  double I = Integral * kI;
  double D = (Last - Actual) * kD;
  double Duty = P + I + D;
 
  Duty = map(Duty, 0, 1024, 0, 255);
  if(Duty > 255){
    Duty = 255;
  }
  if(Duty < 0){
    Duty = 0;
  }
  Last = Actual;
 //Serial.println(Duty);

  return Duty;
}

